#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "disk.h"
#include "priqueue.h"
#include "spinlocks.h"

// Le sera de ayuda la clase sobre semáforos:
// https://www.u-cursos.cl/ingenieria/2022/2/CC4302/1/novedades/detalle?id=431689
// Le serviran la solucion del productor/consumidor resuelto con el patron
// request y la solucion de los lectores/escritores, tambien resuelto con
// el patron request.  Puede substituir los semaforos de esas soluciones
// por spin-locks, porque esos semaforos almacenan a lo mas una sola ficha.


// Declare los tipos que necesite
PriQueue* data[2];


// Declare aca las variables globales que necesite
int slMemory;

int nowUse;
int occupied;
//si ocupado = -1, entonces estamos ocupando el spin

// Agregue aca las funciones requestDisk y releaseDisk

void iniDisk(void) {
    data[0] = makePriQueue();
    data[1] = makePriQueue();
    nowUse = 0;
    occupied = -1; //it's the direction of memory actually
    slMemory = OPEN;
}

void requestDisk(int track) {
    spinLock(&slMemory);
    if(occupied == -1){
        occupied = track; //Since we're now in the direction track
        spinUnlock(&slMemory); //we modified the memory
        return;

    }
    int w = CLOSED;

    if (occupied > track)
        priPut(data[(nowUse+1)%2],&w,track);
    else
        priPut(data[(nowUse)],&w,track);

    spinUnlock(&slMemory); //we added this request in memory
    spinLock(&w);

    spinLock(&slMemory);
    occupied = track;

    spinUnlock(&slMemory);
    return;
}

void releaseDisk() {
    spinLock(&slMemory);
    if(emptyPriQueue(data[nowUse])){
        nowUse = (nowUse+1)%2;
        occupied = -1;
    }

    if(emptyPriQueue(data[nowUse]) ) {

        nowUse = 0;
        occupied = -1;
    }
    else{
        int *ug = (int*) priGet(data[nowUse]);
        spinUnlock(ug);
    }
    spinUnlock(&slMemory);
    return;
}
