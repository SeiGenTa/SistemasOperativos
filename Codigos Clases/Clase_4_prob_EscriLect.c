/////////////Programa correcto////////////

int readers = 0;
int writing = false;
pthread_mutex_t m = pthread_mutex_initializer;
pthread_cond_t c = pthread_cond_initializer;

void enterRead(){
    lock(&m);
    while (writing){
        wait(&c,&m);
    }
    readers++;
    unlock(&m);
}

void enterWrite(){
    lock(&m);
    while (writing||readers>0){
        wait(&c,&m);
    }
    writing = TRUE;
    unlock(&m);
}

void exitRead(){
    lock(&m);
    readers--;
    //Optimizacion
    if (readers == 0){ 
        broadcast(&c);
        }
    unlock(&m);
}

void exitWrite(){
    lock(&m);
    writing = FALSE;
    broadcast(&c);
    unlock(&m);
}