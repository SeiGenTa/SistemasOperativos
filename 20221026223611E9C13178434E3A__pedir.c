#include <stdio.h>
#include <stdlib.h>
#include "pss.h"
#include "spin-locks.h"
#include "pedir.h"

// Defina aca sus variables globales
int sl; // SpinLock para controlar el acceso a la zona crítica.
Queue *q[2]; // Dos colas, una para cada categoría.

// Variable global para indicar el estado del recurso
int ocupado; // 0: Ocupado por core de categoria 0
             // 1: Ocupado por core de categoria 1
             //-1: desocupado

void iniciar() { 
  sl = OPEN;  // Spinlock para mutex se inicializa OPEN
  ocupado = -1; // Se inicializa el recurso como desocupado
  q[0] = makeQueue(); // Se crean ambas colas
  q[1] = makeQueue();
}

void terminar() {
  // Se chequea si la cola todavía tiene un core en espera. Notar que esto puede ser peligroso dado que estamos usando spinlocks un thread en espera significa que el core estará inutilizable.
  if (!emptyQueue(q[0])) 
    fprintf(stderr, "Cola 0 no vacía!");
  
  if(!emptyQueue(q[1]))
    fprintf(stderr, "Cola 1 no vacía!");
   
  // Destruímos ambas colas
  destroyQueue(q[0]); 
  destroyQueue(q[1]);
}

void pedir(int cat) {
  // Se chequea si la categoría es correcta
  if(cat > 1 || cat < 0)
    nFatalError("pedir", "Categoria %d es incorrecta\n", cat);
  	
  spinLock(&sl); // Garantizar exclusión mutua

  if(ocupado == -1){
    // Si el recurso está desocupado, se asigna inmediatamente.
    ocupado = cat;
  } else {
    // Si el recurso está ocupado, debo colocar el core en la cola de espera respectiva

    int w = CLOSED; // Se inicializa un spinlock como cerrado para realizar la espera. Esto es equivalente a un semaforo con 0 tickets
    put(q[cat], &w); // Se pone un puntero del spinlock a la cola de espera, este será utilizado para despertar este thread cuando se le de acceso.
    spinUnlock(&sl); // suelto el spinlock mutex para que otro core pueda entrar a la zona de exclusión mutua.
    spinLock(&w); // espero sobre w. 
    return;
  }
  spinUnlock(&sl); // se libera exclusión mutua
}

void devolver() {
  
  spinLock(&sl); // Garantizar exclusión mutua
  int opuesto = ocupado == 1 ? 0 : 1; // variable para indicar la categoría opuesta a la de quien está devolviendo el recurso.
  // int opuesto = !ocupado; //equivalente a lo de arriba

  // Si es que hay cores del tipo opuesto esperando, debo asignarle el recurso al que lleva más tiempo esperando.
  if(!emptyQueue(q[opuesto])){
    int *pw = get(q[opuesto]); // puntero al spinlock
    ocupado = opuesto; //  Ahora el recurso lo estará ocupando el core de categoría opuesta
    spinUnlock(pw);  // despertar al core que estaba esperando. Esto es equivalente a depositar un ticket en el semaforo.

  }
  // Si es que hay cores del tipo opuesto esperando pero si del mismo tipo, debo asignarle el recurso al que lleva más tiempo esperando de mi mismo tipo.
  else if(!emptyQueue(q[ocupado])){
    int *pw = get(q[ocupado]); // puntero al spinlock
    spinUnlock(pw); // despertar al core que estaba esperando.
    // No es necesario cambiar ocupando, dado que seguirá siendo usado por el mismo tipo.
  } else {
    // No hay cores esperando en ninguna cola, el recurso se debe marcar como desocupado.
    ocupado = -1;
  }

  spinUnlock(&sl); // se libera exclusión mutua
  
}
