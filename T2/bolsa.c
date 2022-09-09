#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#include "bolsa.h"


char nombreVoid[] = " ";

//pthread config
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

//Informacion del vendedoe actual
char *nombre_vendedor;
int precio_merc = -1; //Val init sera -1, (-1 indicara que no hay a la venta)

char *nombre_comprador;

int vendo(int precio, char *vendedor, char *comprador) {
    pthread_mutex_lock(&m);
    printf("inicio vendedor: %s a un precio de: %i \n", vendedor, precio);
    //No hay nadie vendiendo o el precio del mercado es mas alto que el precio que ofrece
    if ((precio_merc == -1) || (precio_merc > precio)){
        //Cambiamos la informacion global
        precio_merc = precio;
        nombre_vendedor = vendedor;
        pthread_cond_broadcast(&c);//Despertamos a los vendedores que esten esperando vender
    }
    else{ //No puede ofrecerlo, ahi alguien con mejor precio
        pthread_mutex_unlock(&m);//Quitamos bloqueo
        return 0;
    }

    pthread_cond_wait(&c,&m); //Iniciamos la espera
    //Solo puede despertar porque aparecio otro vendedor y lo remplazo
    //O vendio su producto

    //Por lo que solicitamos que el bloqueo de la seccion
    if (nombre_vendedor != vendedor){
        pthread_mutex_unlock(&m); //Esto nos indicara que aparecio alguien con mejor precio
        //por lo que retornaremos el falso
        return 0;
        
    }
    //Aqui solo se llega cuando el vendedor si vende su accion
    precio_merc = -1;
    strcpy(comprador,nombre_comprador);
    pthread_mutex_unlock(&m); //No usaremos mas la memoria global
    
    return 1;
}

int compro(char *comprador, char *vendedor) {
    printf("inicia comprado: %s\n",comprador);
    pthread_mutex_lock(&m);
    if (precio_merc != -1){ //Hay un vendedor disponible, 
        strcpy(vendedor,nombre_vendedor);
        nombre_comprador = comprador; //Cambiamos el nombre del comprador
        int precioPagado = precio_merc;
        pthread_cond_broadcast(&c);
        pthread_mutex_unlock(&m);
        return precioPagado;
    }
    pthread_mutex_unlock(&m);
    //En caso de que no haya nadie vendiendo retornamos false
    return 0;

}
