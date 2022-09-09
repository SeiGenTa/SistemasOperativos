//No Tiene orden de llegada
pthread_mutex_t m = pthread_mutex_initializer;

void ocupar(){

    pthread_mutex_lock(&m);
}

void desocupar(){
    pthread_mutex_unlock(&m);
}
//Los mutex se otorgan en un orden no especificado.

pthread_mutex_t m = pthread_mutex_initializer;

void ocupar(){
    pthread_mutex_lock(&m);
    while(busy){
        pthread_cond_wait(&c,&m);
    }
    busy=1;
    pthread_mutex_unlock(&m);
}


/////////////////////////////////
pthread_mutex_t m = pthread_mutex_initializer;
pthread_cond_t c = pthread_cond_initializer;
int ticket_dist = 0, display = 0;//Ticket distrib & display

void ocupar(){
    pthread_mutex_lock(&m);
    //Mientras esperamos tenemos una variable local que
    //sirve como ticket de espera
    int my_num = ticket_dist++;
    while(my_num != display){
        //Este modo espera se parece mucho al lock
        //Pero es recomendable para esperas mas largas.
        pthread_cond_wait(&c,&m);
    }
    pthread_mutex_unlock(&m);
}

void desocupar(){
    pthread_mutex_unlock(&m);
    //Nos aumenta la variable global
    display++;
    //Es realmente util llamar a todas los pthread??
    //se crear una lista de mutex??
    pthread_cond_broadcast(&c);
    pthread_mutex_unlock(&m);
}

//////////////////////////////////////
///NUEVA SOLUCION DEL PROBLEMA LECT/ESCRI
pthread_mutex_t m = pthread_mutex_initializer;
pthread_cond_t c = pthread_cond_initializer;
int ticket_dist = 0, display = 0;//Ticket distrib & display
int readers = 0; //Ya no es necesario un writing

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
    //Ahora no puede entrar un lector cuando hay un
    //Editor esperando
    while (my_num != display || readers>0){
        pthread_cond_wait(&c,&m);
    }
    pthread_mutex_unlock(&m);
}

void exitRead(){
    pthread_mutex_lock(&m);
    readers--;
    //Optimizacion
    if (readers == 0){
        pthread_cond_broadcast(&c);
        }
    pthread_mutex_unlock(&m);
}

void exitWrite(){
    pthread_mutex_lock(&m);
    writing = FALSE;
    pthread_cond_broadcast(&c);
    pthread_mutex_unlock(&m);
}

///////////////////////////
//Volviendo al problema de recursos compartidos

typedef pilPthread{
   int ready;
   pthread_cond_t w;
}Request;

pthread_mutex_t m = pthread_mutex_initializer;
int busy = 0;
Queue *q; 

void ocupar(){
    pthread_mutex_lock(&m);
    if(!busy){
        busy = 1;
    }
    else {
        Request req= {0,
            pthread_cond_initializer};
        put(q,&req);
        while(!req.ready){
            pthread_cond_wait(&req.w,&m)
        }
    }
    pthread_mutex_unlock(&m);
}

void desocupar(){
    pthread_mutex_lock(&m);
    if(emptyQueue(q)){
        busy = 0;
    }
    else{
        Request *preq = get(q);
        preq->ready = 1;
        pthread_cond_signal(&preq -> w);
    }
    pthread_mutex_unlock(&m);
}
