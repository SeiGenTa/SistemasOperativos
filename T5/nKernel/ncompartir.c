#define _XOPEN_SOURCE 500

#include "nthread-impl.h"

// Use los estados predefinidos WAIT_ACCEDER, WAIT_ACCEDER_TIMEOUT y
// WAIT_COMPARTIR
// El descriptor de thread incluye el campo ptr para que Ud. lo use
// a su antojo.


static NthQueue *filAcces;
static nThread *ComGlobal;
static int amountAcces;

// nth_compartirInit se invoca al partir nThreads para que Ud. inicialize
// sus variables globales

void nth_compartirInit(void) {
    filAcces = nth_makeQueue();
    ComGlobal = NULL;
    amountAcces = 0;
}

void nCompartir(void *ptr) {
    START_CRITICAL
    nThread th = nSelf();
    th->ptr = ptr;
    ComGlobal = &th;
    while(!nth_emptyQueue(filAcces)){
        nThread thA = nth_getFront(filAcces);
        if(thA -> status == WAIT_ACCEDER_TIMEOUT)
            nth_cancelThread(thA);
        setReady(thA);
        schedule();
    }
    suspend(WAIT_COMPARTIR);
    schedule();
    END_CRITICAL
    return;
}

static void f(nThread th) {
<<<<<<< HEAD
    printf("Ingreso a f\n");
    nth_delQueue(filAcces,th);
    setReady(th);
=======
    START_CRITICAL
    cant_acc--;
    int* dir =(int*) (th->ptr);
    *dir = 1;
    if(nth_queryThread(enFil,th)){
        nth_delQueue(enFil,th);
    }
    END_CRITICAL
    return;
>>>>>>> 1d29005cc5b26f6a5bafa2a79b01c7d11d965f2d
}

void *nAcceder(int max_millis) {
    nThread th = nSelf();
    START_CRITICAL
<<<<<<< HEAD
    amountAcces++;
    if(ComGlobal == NULL){
        nth_putBack(filAcces,th);
        if (max_millis > 0){
            th->status = WAIT_ACCEDER_TIMEOUT;
            nth_programTimer(max_millis * 1000000, f);
            printf("se configuro el programTimer \n");
=======
    cant_acc++;
    int a = 0;

    nThread thisTh= nSelf();
    thisTh->ptr = &a;
    if (nthreadG == NULL){
        nth_putBack(enFil,thisTh);
        if (max_millis > 0){
            suspend(WAIT_ACCEDER_TIMEOUT);
            long long nano =(long long) max_millis*1000000;
            nth_programTimer(nano,f);
>>>>>>> 1d29005cc5b26f6a5bafa2a79b01c7d11d965f2d
        }
        else{
            suspend(WAIT_ACCEDER);
        }
<<<<<<< HEAD
=======
        
>>>>>>> 1d29005cc5b26f6a5bafa2a79b01c7d11d965f2d
        schedule();
    }
    void* memory;

<<<<<<< HEAD
    void *memory = ComGlobal ? (*ComGlobal)->ptr : NULL;
=======
    if (a == 0)
        memory = (*nthreadG)->ptr;
    else 
        memory = NULL;
>>>>>>> 1d29005cc5b26f6a5bafa2a79b01c7d11d965f2d
    END_CRITICAL
    return memory;
}

void nDevolver(void) {
    START_CRITICAL
    amountAcces--;

    if (amountAcces == 0){
        setReady(*ComGlobal);
        ComGlobal = NULL;
    }
    END_CRITICAL
    return;
}
