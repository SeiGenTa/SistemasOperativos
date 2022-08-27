void filosofo(int i){
    for(;;){
        comer(i,(i+1)%5);
        pensar();
    }
}

///////////SEGUNDA IDEA//////////
pthread_mutex_t m = pthread_mutex_initializer;
void filosofo(int i){
    for(;;){
        lock(&m);
        comer(i,(i+1)%5);
        unlock(&m);
        pensar();
    }
}

//Pero esta es errada

//////////////TERCERA SOLUCION///////////////
pthread_mutex_t m[5] = {pthread_mutex_initializer,pthread_mutex_initializer,
                        pthread_mutex_initializer,pthread_mutex_initializer};

void filosofo(int i){
    for(;;){
        lock(m[i]);
        lock(m[(i+1)%5]);
        comer(i,(i+1)%5);
        unlock(m[i]);
        unlock(m[(i+1)%5]);
        pensar();
    }
}
//TAMBIEN ES INCORRECTA

//Ocurre un deadlock

pthread_mutex_t m[5] = {pthread_mutex_initializer,pthread_mutex_initializer,
                        pthread_mutex_initializer,pthread_mutex_initializer};

void filosofo(int i){
    for(;;){
        int j = min(i,(i+1)%5);k = max(i,(i+1)%5);
        lock(m[j]);
        lock(m[k]);
        comer(i,(i+1)%5);
        unlock(m[j]);
        unlock(m[k]);
        pensar();
    }
}

////SOLUCION OPTIMA////
int pals[5] = {0,0,0,0,0};
pthread_mutex_t m = pthread_mutex_initializer;
pthread_cond_t c = pthread_cond_initializer;

void filosofo(int i){
    for(;;){
        pedir(i,(i+1)%5);
        comer(i,(i+1)%5);
        devolver(i,(i+1)%5)
        pensar();
    }
}

void pedir(int j,int k){
    lock(&m);
    while(pals[j]||pals[k]){
        wait(&c,&m);
    }
    pals[j] = pals[k] = 1;
    unlock(&m);
}

void devolver(int j,int k){
    lock(&m);
    pals[j] = pals[k] = 0;
    broadcast(&c);
    unlock(&m);
}


