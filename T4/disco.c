#include <stdlib.h>
#include <stdio.h>
#include <nthread.h>
#include <string.h>

#include "pss.h"
#include "disco.h"

typedef struct{
	char* name;
	char** nameParnet;
	pthread_cond_t cond;
} Person;

Person *InitPerson(char *name){
	Person *p = malloc(sizeof(Person));
	p->name = name;
	p->nameParnet = NULL;
	pthread_cond_init(&p->cond,NULL);
	return p;
}

//we create the rows
Queue *row_men;
Queue *row_women;

//we create a mutex
static pthread_mutex_t t = PTHREAD_MUTEX_INITIALIZER;

void DiscoInit(void){
	row_men = makeQueue();
	row_women = makeQueue();
}

void DiscoDestroy(void) {
	destroyQueue(row_men);
	destroyQueue(row_women);
}

char *dama(char *nom) {
	printf("mi nombre es: %s \n", nom);
	Person* me =InitPerson(nom);
	printf("mi nombre es: %s \n", nom);
	char* nameParnet = *me->nameParnet;

	pthread_mutex_lock(&t);
	
	if (emptyQueue(row_women)){
		
		printf("Se iniciara el guardado del nombre\n");
		put(row_women,me);
		
	}
	pthread_mutex_unlock(&t);
	free(me);
	return nameParnet;
}

char *varon(char *nom) {
	printf("mi nombre es: %s \n", nom);
	return nom;
}
