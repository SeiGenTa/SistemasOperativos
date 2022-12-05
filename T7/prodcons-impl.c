/* Implemente aqui el driver para /dev/prodcons */
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

MODULE_LICENSE("Dual BSD/GPL");

/* Declaration of memory.c functions */

static KMutex mutex;
static KCondition cond;

static int prodCons_open();
static ssize_t prodCons_read(struct file *filp, char *buf,
                             size_t count, loff_t *f_pos);
static ssize_t prodCons_write(struct file *filp, char *buf,
                             size_t count, loff_t *f_pos);
int prodCons_init();
void prodCons_init();

/* Major number */
static int memory_major = 61;
/* Buffer to store data */
#define MAX_SIZE 8192

static struct file_operations memory_fops ={
        open = prodCons_open,
        read = prodCons_read,
        write = prodCons_write
};

module_init(prodCons_init);
module_exit(prodCons_exit);

int prodCons_init(){
    int result;
    result = register_chrdev(memory_major, "memory", &memory_fops);
    if (result < 0) {
        printk(
                "<1>memory: cannot obtain major number %d\n", memory_major);
        return result;
    }

    m_init(&mutex);
    c_init(&cond);s

    memory_buffer = kmalloc(MAX_SIZE, GFP_KERNEL);
    if (!memory_buffer) {
        result = -ENOMEM;
        goto fail;
    }

    memset(memory_buffer, 0, MAX_SIZE);
    curr_size= 0;

    printk("<1>Inserting memory module\n");
    return 0;

    fail:
    memory_exit();
    return result;
}

int prodCons_exit(){
    unregister_chrdev(memory_major, "memory");

    /* Freeing buffer memory */
    if (memory_buffer) {
        kfree(memory_buffer);
    }

    printk("<1>Removing memory module\n");
};

static ssize_t prodCons_read(struct file *filp, char *buf,
                             size_t count, loff_t *f_pos){
    ssize_t rc;
    down(&mutex);

    if (count > curr_size-*f_pos) {
        count= curr_size-*f_pos;
    }

    printk("<1>read %d bytes at %d\n", (int)count, (int)*f_pos);

    /* Transfering data to user space */
    if (copy_to_user(buf, memory_buffer+*f_pos, count)!=0) {
        /* el valor de buf es una direccion invalida */
        rc= -EFAULT;
        goto epilog;
    }

    *f_pos+= count;
    rc= count;

    epilog:
    up(&mutex);
    return rc;
}

static ssize_t prodCons_write(struct file *filp, char *buf,
                             size_t count, loff_t *f_pos){
    ssize_t rc;
    rc= count;
    down(&mutex);

    last= *f_pos + count;
    if (last>MAX_SIZE) {
        count -= last-MAX_SIZE;
    }
    printk("<1>write %d bytes at %d\n", (int)count, (int)*f_pos);

    if (copy_from_user(memory_buffer+*f_pos, buf, count)!=0) {
        /* el valor de buf es una direccion invalida */
        rc= -EFAULT;
        goto epilog;
    }

    *f_pos += count;
    curr_size= *f_pos;

    epilog:
    up(&mutex);
    return rc;
}