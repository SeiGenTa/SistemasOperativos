pthread_mutex_t m = pthread_mutex_initializer;
pthread_cond_t c = pthread_cond_initializer;
int readers = 0; writing = 0; //Ya no es necesario un writing
Queue *q;//


void enterRead(){
    pthread_mutex_lock(&m);
    int my_num = ticket_dist++;
    while (writing != display){
        pthread_cond_wait(&c,&m);
    }
    readers++;
    display++;
    pthread_cond_broadcast(&c);
    pthread_mutex_unlock(&m);
}

void enterWrite(){
    pthread_mutex_lock(&m);
    int my_num = ticket_dist++;
    if(readders == 0 && !writing) writing = 1;
    else enqueue(WRITER);
    pthread_mutex_unlock(&m);
}

void exitRead(){
    pthread_mutex_lock(&m);
    writing = 0;
    wakeup();
    pthread_mutex_unlock(&m);
}

void exitWrite(){
    pthread_mutex_lock(&m);
    writing = FALSE;
    pthread_cond_broadcast(&c);
    pthread_mutex_unlock(&m);
}