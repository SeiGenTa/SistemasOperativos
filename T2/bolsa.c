#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#include "bolsa.h"

//generaremos un estructura que sera como un producto en venta
//en conjunto con su vendedor y precio

pthread_mutex_t cont_merc = PTHREAD_MUTEX_INITIALIZER; //Para modificar el mercado
pthread_cond_t cond_merc = PTHREAD_COND_INITIALIZER; //Sera para el modo esperar del vendedor

//VENDEDOR
char nombre_vendedor[100]; //Nombre del vendedor
int precioInMercado = -1; //-1 indicara que no hay nadie vendiendo

char compro[100] = ""; //Nombre del comprador

int vendo(int precio, char *vendedor, char *comprador) {
    //Para leer la informacion y modificar bloqueamos la informacion
    for(;;){
        pthread_mutex_lock(&cont_merc);
        if (precioInMercado < precio){ //Si se cumple que es el precio menor se coloca en la lista
            nombre_vendedor = vendedor;
            precioInMercado = precio;
        }
        pthread_mutex_unlock(&cont_merc);

        while(nombre_vendedor == vendedor){
            if(compro != ""){
                break;
            }
            pthread_cond_wait(&cond_merc,NULL);
        }
        
        pthread_mutex_lock(&cont_merc);
        if(vendedor == nombre_vendedor){
            if (compro != ""){
                char comp = compro;

                comprador = comp
                nombre_vendedor = "";
                precioInMercado = 1;
                break;
            }
        }
        pthread_mutex_lock(&cont_merc);
    }
    if 
}



int compro(char *comprador, char *vendedor) {
    if (precio == -1){
        return 0;
    }
}
