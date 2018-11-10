//Estudo dirigido 7 - Semáforos
// Nicholas Nishimoto Marques - 150019343

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TRUE 1
#define QTD_BUFFER 10
#define QTD_PRODUTORES 2
#define QTD_CONSUMIDORES 5

typedef int semaphore;
semaphore mutex = 1;
semaphore empty = QTD_BUFFER;
semaphore full = 0;

pthread_t consumidores[QTD_CONSUMIDORES];
pthread_t produtores[QTD_PRODUTORES];
pthread_mutex_t mutex_buffer;

int buffer[QTD_BUFFER];

sem_t free_pos;
sem_t filled_pos;

int cont_dados = 0;

void showBuffer(){
    int i;
    printf("Buffer: ");
    for(i=0;i<cont_dados;i++){
        printf("%d, ",buffer[i]);
    }
    printf("\n\n");
}

void * produzir(void * arg){
    int id = *((int*)arg);

    while(TRUE){
        int item = (rand()%10);

        sem_wait(&free_pos);
        pthread_mutex_lock(&mutex_buffer);
        buffer[cont_dados] = item;
        cont_dados++;

        printf("Item %d adicionado ao buffer\n",item);
        showBuffer();

        pthread_mutex_unlock(&mutex_buffer);
        sem_post(&filled_pos);
        printf("Produtor %d produziu item %d \n",id,item);

        sleep(5);

    }
}

void * consumir(void * arg){
    int id = *((int*)arg);

    while(TRUE){
        int item;

        sem_wait(&filled_pos);
        pthread_mutex_lock(&mutex_buffer);
        item = buffer[cont_dados];
        cont_dados--;

        printf("Item %d consumido do buffer\n",item);
        showBuffer();

        pthread_mutex_unlock(&mutex_buffer);
        sem_post(&free_pos);
        printf("Consumidor %d consumiu item %d... \n",id,item);
        showBuffer();

        sleep(8);

    }
}

int main(){

    int i, *id;
    pthread_t consumidores[QTD_CONSUMIDORES];
    pthread_t produtores[QTD_PRODUTORES];

    pthread_mutex_init(&mutex_buffer,NULL);
    sem_init(&free_pos,0,QTD_BUFFER);
    sem_init(&filled_pos,0,0);


    for (i = 0; i < QTD_PRODUTORES; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&produtores[i], NULL, produzir, (void *)(id));
    }

    for (i = 0; i < QTD_CONSUMIDORES; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&consumidores[i], NULL, consumir, (void *)(id));
    }

    for(i = 0; i < QTD_CONSUMIDORES; i++)
    {
        if(pthread_join(consumidores[i], NULL))
        {
            printf("Could not join thread %d\n", i);
            return -1;
        }
    }

    for(i = 0; i < QTD_PRODUTORES; i++)
    {
        if(pthread_join(produtores[i], NULL))
        {
            printf("Could not join thread %d\n", i);
            return -1;
        }
    }

    return 0;
}