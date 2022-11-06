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
    printf("Ingreso a f\n");
    nth_delQueue(filAcces,th);
    setReady(th);
}

void *nAcceder(int max_millis) {
    nThread th = nSelf();
    START_CRITICAL
    amountAcces++;
    if(ComGlobal == NULL){
        nth_putBack(filAcces,th);
        if (max_millis > 0){
            th->status = WAIT_ACCEDER_TIMEOUT;
            nth_programTimer(max_millis * 1000000, f);
            printf("se configuro el programTimer \n");
        }
        else{
            suspend(WAIT_ACCEDER);
        }
        schedule();
    }

    void *memory = ComGlobal ? (*ComGlobal)->ptr : NULL;
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
