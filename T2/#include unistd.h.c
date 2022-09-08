#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "prod.h"

typedef struct{
    int *a;
    int i;
    int j;
    int p;
    BigNum *resultado;
}pap_param;

void *thread_func(void *tr){
    pap_param *arg = (pap_param *) tr;

    int *a = arg->a;
    int i = arg->i;
    int j = arg->j;
    int p = arg->p;

    arg->resultado = parArrayProd(a,i,j,p);
    return NULL;
}

BigNum *parArrayProd(int a[], int i, int j, int p) {
    if (i == j) return smallNum(a[i]);
    if (i>j) {
        fprintf(stderr, "Asercion fallida: i > j\n");
        exit(1);
    }
    if (p == 1) return seqArrayProd(a,i,j);
    int div_list = (i+j)/2;
    int div_cores = p/2;

    pap_param *arg = malloc(sizeof(pap_param));
    arg->a = a;
    arg->i = i;
    arg->j = div_list;
    arg->p = div_cores;

    pthread_t pid;
    pthread_create(&pid, NULL, thread_func,(void *) arg);

    BigNum *right = parArrayProd(a,div_list+1,j,p-div_cores);

    pthread_join(pid,NULL);

    BigNum *resultado_total = bigMul(right,arg->resultado);

    freeBigNum(right);

    freeBigNum(arg->resultado);
    free(arg);

    return resultado_total;
}