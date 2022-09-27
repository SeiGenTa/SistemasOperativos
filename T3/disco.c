#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#include "pss.h"
#include "disco.h"

typedef struct{
	char *name;
	char *nameParnet;
	pthread_cond_t *cond;
} Person;

Person *InitPerson(char *name){
	Person *p = malloc(sizeof(Person));
	p->name = name;
	p->nameParnet = NULL;
	pthread_cond_t c = PTHREAD_COND_INITIALIZER;
	p->cond = &c;
	return p;
}

//we create the rows
Queue *row_men;
Queue *row_women;

pthread_mutex_t t = PTHREAD_MUTEX_INITIALIZER;

void DiscoInit(void){
	row_men = makeQueue();
	row_women = makeQueue();
}

void DiscoDestroy(void) {
	destroyQueue(row_men);
	destroyQueue(row_women);
}

char *dama(char *nom) {
	Person *me = InitPerson(nom);
	
	pthread_mutex_lock(&t);
	if (!emptyQueue(row_men)){
		Person *parnet = get(row_men);
		pthread_mutex_unlock(&t);
		strcpy(parnet->nameParnet,nom);
		pthread_cond_broadcast(&parnet->cond);
	}
	else{
		
	}
	
}

char *varon(char *nom) {
	Person *me = InitPerson(nom);
}
