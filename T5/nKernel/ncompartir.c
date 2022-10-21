#define _XOPEN_SOURCE 500

#include "nthread-impl.h"

// Use los estados predefinidos WAIT_ACCEDER y WAIT_COMPARTIR
// El descriptor de thread incluye el campo ptr para que Ud. lo use
// a su antojo.

static nThread *nthreadG;
static int cant_acc;
static NthQueue *enFil;

// nth_compartirInit se invoca al partir nThreads para Ud. inicialize
// sus variables globales

void nth_compartirInit(void) {
    nthreadG = NULL;
    cant_acc = 0;
    enFil = nth_makeQueue();
}

void nCompartir(void *ptr) {
    START_CRITICAL

    nThread thisTh= nSelf();
    thisTh->ptr = ptr;
    nthreadG = &thisTh;
    while(!nth_emptyQueue(enFil)){
        nThread th = nth_getFront(enFil);
        if (th->status == WAIT_ACCEDER_TIMEOUT)
            nth_cancelThread(th);
        setReady(th);
    }
    suspend(WAIT_COMPARTIR);
    schedule();
    END_CRITICAL
    return;
}

static void f(nThread th) {
    START_CRITICAL
    cant_acc--;
    int* dir =(int*) (th->ptr);
    *dir = 1;
    if(nth_queryThread(enFil,th)){
        nth_delQueue(enFil,th);
    }
    END_CRITICAL
    return;
}

void *nAcceder(int max_millis) {
    START_CRITICAL
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
        }
        else{
            suspend(WAIT_ACCEDER);
        }
        
        schedule();

        if (thisTh->status == WAIT_ACCEDER_TIMEOUT){
            END_CRITICAL
            return NULL;
        }
    }
    void* memory;

    if (a == 0)
        memory = (*nthreadG)->ptr;
    else 
        memory = NULL;
    END_CRITICAL
    return memory;
}

void nDevolver(void) {
    START_CRITICAL
    cant_acc--;
    if (cant_acc != 0){
        END_CRITICAL
        return;
    }
    setReady(*nthreadG);
    //schedule();
    nthreadG = NULL;
    END_CRITICAL
    return;
}
