#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

//primer codigo//
typedef struct{
    int count;
    void *queue;
}nSem;

int nSemWait(nSem *psem){
    START_CRITICAL
    if (psem->count > 0)
        psem->count--;
    esle{
        nThread thisTh = nSelf();
        nth_putBack(psem->queue, thisTh);
        suspend(WAIT_SEM);
        schedule();
    }
    END_CRITICAL
    return 0;
}

int nSemPost(nSem *psem){
    START_CRITICAL
    if (nth_emptyQueue(psem->queue))
        psem->count++;
    else{
        nThread w= nth_getFront(psem->queue);
        setReady(w);
        schedule();
    }
    END_CRITICAL
    return 0;
}

//Equivalencias

START_CRITCAL ->
sigset_t sigsetOld;
pthread_sigmask(SIG_BLOCK, &nth_sigsetCritical,&sigsetOld);

END_CRITICAL ->
sigset_t sigsetOld;
pthread_sigmask(SIG_BLOCK, &nth_sigsetCrtical, &sitgsetOld);

//implementacion de nSelf y nth_setSelft
nThread nSelf(){
    return nth_coreThreads[nth_coreld()];
}

void nth_setSelf(nThread th){
    nth_coreThreads[nth_coreld()] = th;
}