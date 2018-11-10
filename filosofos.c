//Estudo dirigido 8 - Filósofos
// Nicholas Nishimoto Marques - 150019343

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define QTD_FILOSOFOS 5
#define ESQ(i) ((i + QTD_FILOSOFOS)%QTD_FILOSOFOS)
#define DIR(i) ((i + 1)%QTD_FILOSOFOS)

#define COMENDO 101
#define PENSANDO 102
#define FAMINTO 103

typedef int semaphore;

semaphore sem_filosofos[QTD_FILOSOFOS];
semaphore estado_filosofos[QTD_FILOSOFOS];
sem_t garfos[QTD_FILOSOFOS];

pthread_t filosofos[QTD_FILOSOFOS];

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void initialize_sem(){
    int i;
    for(i=0;i<QTD_FILOSOFOS;i++){
        sem_init(&garfos[i],FALSE,TRUE);
        estado_filosofos[i] = PENSANDO;
    }
}

void * pegar_garfo(int id){

    pthread_mutex_lock(&lock);
    estado_filosofos[id] = FAMINTO;
    can_eat(id);
    pthread_mutex_unlock(&lock);
    sem_wait(&garfos[id]);
}

void * soltar_garfo(int id){

    pthread_mutex_lock(&lock);
    estado_filosofos[id] = PENSANDO;
    can_eat(ESQ(id));
    can_eat(DIR(id));
    pthread_mutex_unlock(&lock);

}

int can_eat (int id){

    if((estado_filosofos[id] == FAMINTO) && (estado_filosofos[ESQ(id)]) && (estado_filosofos[DIR(id)])){
        estado_filosofos[id] = COMENDO;
        sem_post(&garfos[id]);
    }

    return TRUE;
}

void showGarfos(){
    int i;
    printf("Garfos ocupados: ");
    for(i=0;i<QTD_FILOSOFOS;i++){
        if(sem_getvalue(&garfos[i],0) == FALSE){
            printf("Garfo %d ocupado, ",i);
        } else {
            printf("Garfo %d desocupado, ",i);
        }
    }
    printf("\n\n");
}

void * filosofar(void * arg){
    int id = *((int*)arg);

    while(TRUE){
        printf("Filósofo %d pensando...\n",id);
        sleep(4);

        printf("Filósofo %d esperando por garfos...\n",id);
        pegar_garfo(id);
        printf("Filósofo %d pegou garfo e está comendo...\n",id);
        sleep(5);
        showGarfos();
        soltar_garfo(id);
        printf("Filósofo %d terminou de comer e soltou garfo...\n",id);
    }

}

int main(){
    int i, *id;

    initialize_sem();

    for (i = 0; i < QTD_FILOSOFOS; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&filosofos[i], NULL, filosofar, (void *)(id));
    }

    for(i = 0; i < QTD_FILOSOFOS; i++)
    {
        if(pthread_join(filosofos[i], NULL))
        {
            printf("Could not join thread %d\n", i);
            return -1;
        }
    }

    return 0;
}