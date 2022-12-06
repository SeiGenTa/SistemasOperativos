/* Necessary includes for device drivers */
#include <linux/init.h>
/* #include <linux/config.h> */
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <linux/uaccess.h> /* copy_from/to_user */

#include "kmutex.h"

MODULE_LICENSE("Dual BSD/GPL");

/* Declaration of prodCons.c functions */
int prodCons_open(struct inode *inode, struct file *filp);
int prodCons_release(struct inode *inode, struct file *filp);
ssize_t prodCons_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
ssize_t prodCons_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
void prodCons_exit(void);
int prodCons_init(void);

/* Structure that declares the usual file */
/* access functions */
struct file_operations prodCons_fops = {
        read: prodCons_read,
        write: prodCons_write,
        open: prodCons_open,
        release: prodCons_release
};

/* Declaration of the init and exit functions */
module_init(prodCons_init);
module_exit(prodCons_exit);

/*** El driver para lecturas sincronas *************************************/

#define TRUE 1
#define FALSE 0

/* Global variables of the driver */

int prodCons_major = 61;     /* Major number */

/* Buffer to store data */
#define MAX_SIZE 8192

static char *prodCons_buffer;
static ssize_t curr_size;
static int readers;
static int writing;
static int pend_open_write;

/* El mutex y la condicion para prodCons */
static KMutex mutex;
static KCondition cond;

int prodCons_init(void) {
    int rc;

    /* Registering device */
    rc = register_chrdev(prodCons_major, "prodCons", &prodCons_fops);
    if (rc < 0) {
        printk(
                "<1>prodCons: cannot obtain major number %d\n", prodCons_major);
        return rc;
    }

    readers= 0;
    writing= FALSE;
    pend_open_write= 0;
    curr_size= 0;
    m_init(&mutex);
    c_init(&cond);

    /* Allocating prodCons_buffer */
    prodCons_buffer = kmalloc(MAX_SIZE, GFP_KERNEL);
    if (prodCons_buffer==NULL) {
        prodCons_exit();
        return -ENOMEM;
    }
    memset(prodCons_buffer, 0, MAX_SIZE);

    printk("<1>Inserting prodCons module\n");
    return 0;
}

void prodCons_exit(void) {
    /* Freeing the major number */
    unregister_chrdev(prodCons_major, "prodCons");

    /* Freeing buffer prodCons */
    if (prodCons_buffer) {
        kfree(prodCons_buffer);
    }

    printk("<1>Removing prodCons module\n");
}

int prodCons_open(struct inode *inode, struct file *filp) {
    int rc= 0;
    m_lock(&mutex);

    if (filp->f_mode & FMODE_WRITE) {
        int rc;
        printk("<1>open request for write\n");
        /* Se debe esperar hasta que no hayan otros lectores o escritores */
        pend_open_write++;
        while (writing || readers>0) {
            if (c_wait(&cond, &mutex)) {
                pend_open_write--;
                c_broadcast(&cond);
                rc= -EINTR;
                goto epilog;
            }
        }
        writing= TRUE;
        pend_open_write--;
        curr_size= 0;
        c_broadcast(&cond);
        printk("<1>open for write successful\n");
    }
    else if (filp->f_mode & FMODE_READ) {
        /* Para evitar la hambruna de los escritores, si nadie esta escribiendo
         * pero hay escritores pendientes (esperan porque readers>0), los
         * nuevos lectores deben esperar hasta que todos los lectores cierren
         * el dispositivo e ingrese un nuevo escritor.
         */
        while (!writing && pend_open_write>0) {
            if (c_wait(&cond, &mutex)) {
                rc= -EINTR;
                goto epilog;
            }
        }
        readers++;
        printk("<1>open for read\n");
    }

    epilog:
    m_unlock(&mutex);
    return rc;
}

int prodCons_release(struct inode *inode, struct file *filp) {
    m_lock(&mutex);

    if (filp->f_mode & FMODE_WRITE) {
        writing= FALSE;
        c_broadcast(&cond);
        curr_size = 0; //con esto deberiamos evitar la lectura
        printk("<1>close for write successful\n");
    }
    else if (filp->f_mode & FMODE_READ) {
        readers--;
        if (readers==0)
            c_broadcast(&cond);
        printk("<1>close for read (readers remaining=%d)\n", readers);
    }

    m_unlock(&mutex);
    return 0;
}

ssize_t prodCons_read(struct file *filp, char *buf,
                      size_t count, loff_t *f_pos) {
    ssize_t rc;
    m_lock(&mutex);

    /* Si llega un lector, de inmediado se coloca en la cola */
    if (c_wait(&cond, &mutex)) {
        printk("<1>read interrupted\n");
        rc= -EINTR;
        goto epilog;
    }

    if (count > curr_size-*f_pos) {
        count= curr_size-*f_pos;
    }

    printk("<1>read %d bytes at %d\n", (int)count, (int)*f_pos);

    /* Transfiriendo datos hacia el espacio del usuario */
    if (copy_to_user(buf, prodCons_buffer+*f_pos, count) != 0) {
        /* el valor de buf es una direccion invalida */
        rc= -EFAULT;
        goto epilog;
    }

    *f_pos+= 0;
    rc= count;

    epilog:
    m_unlock(&mutex);
    return rc;
}

ssize_t prodCons_write(struct file *filp, const char *buf,
                       size_t count, loff_t *f_pos) {
    ssize_t rc;
    loff_t last;

    m_lock(&mutex);

    last= *f_pos + count;
    if (last>MAX_SIZE) {
        count -= last-MAX_SIZE;
    }
    printk("<1>write %d bytes at %d\n", (int)count, (int)*f_pos);

    /* Transfiriendo datos desde el espacio del usuario */
    if (copy_from_user(prodCons_buffer+*f_pos, buf, count)!=0) {
        /* el valor de buf es una direccion invalida */
        rc= -EFAULT;
        goto epilog;
    }

    curr_size= count;
    rc= count;
    c_signal(&cond); // Con esto solo despertamos a uno de los lectores

    epilog:
    m_unlock(&mutex);
    return rc;
}
