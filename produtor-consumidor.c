//Estudo dirigido 5
// Nicholas Nishimoto Marques - 150019343

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TRUE 1
#define TAMANHO_BUFFER 5
#define NConsumidores 3
#define NProdutores 2
#define VAZIO 24710

int cont_dados = 0;
int buffer[TAMANHO_BUFFER];

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condicao_parada_produtor = PTHREAD_COND_INITIALIZER;
pthread_cond_t condicao_parada_consumidor = PTHREAD_COND_INITIALIZER;

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
        pthread_mutex_lock(&lock);

        int item = (rand()%10);

        while(cont_dados == TAMANHO_BUFFER){
            printf("Buffer cheio, produtores esperando...\n");
            pthread_cond_wait(&condicao_parada_produtor,&lock);
        }
        printf("Produtor %d fazendo item...\n",id);
        sleep(3);
        buffer[cont_dados] = item;
        cont_dados++;
        
        printf("Item %d adicionado ao buffer\n",item);
        showBuffer();
        if(cont_dados == 1){
            printf("Buffer agora com itens, acordando consumidores...\n\n");
            pthread_cond_broadcast(&condicao_parada_consumidor);
        }

        pthread_mutex_unlock(&lock);
        sleep(5);
    }

    pthread_exit(0);
}

void * consumir(void * arg){
    int id = *((int*)arg);

    while(TRUE){
        pthread_mutex_lock(&lock);

        int item;
        while(cont_dados == 0){
            printf("Buffer vazio, esperando mais...\n");
            pthread_cond_wait(&condicao_parada_consumidor,&lock);
        }
        printf("Consumidor %d consumindo dados...\n",id);
        sleep(3);
        item = buffer[cont_dados];
        buffer[cont_dados] = VAZIO;
        cont_dados--;
        if(cont_dados == TAMANHO_BUFFER-1){
            printf("buffer não está mais cheio. Acordando produtores...\n\n");
            pthread_cond_broadcast(&condicao_parada_produtor);
        }

        printf("Item consumido\n");
        showBuffer();

        pthread_mutex_unlock(&lock);
        sleep(30);
    }
    pthread_exit(0);
}

int main(){
    
    int i, *id;
    pthread_t consumidores[NConsumidores];
    pthread_t produtores[NProdutores];


    for (i = 0; i < NProdutores; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&produtores[i], NULL, produzir, (void *)(id));
    }

    for (i = 0; i < NConsumidores; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&consumidores[i], NULL, consumir, (void *)(id));
    }

    for(i = 0; i < NConsumidores; i++)
    {
        if(pthread_join(consumidores[i], NULL))
        {
            printf("Could not join thread %d\n", i);
            return -1;
        }
    }

    for(i = 0; i < NProdutores; i++)
    {
        if(pthread_join(produtores[i], NULL))
        {
            printf("Could not join thread %d\n", i);
            return -1;
        }
    }

    return 0;
}