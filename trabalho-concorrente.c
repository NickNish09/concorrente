//Trabalho - Travessia no Rio
// Nicholas Nishimoto Marques - 150019343

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define QTD_FILHOS 3
#define QTD_FILHAS 3
#define QTD_PAI 1
#define QTD_MAE 1
#define QTD_POLICIAL 1
#define QTD_LADRAO 1
#define QTD_LUGARES_BARCO 2

typedef int semaphore;

semaphore sem_filosofos[QTD_FILOSOFOS];
semaphore estado_filosofos[QTD_FILOSOFOS];
sem_t garfos[QTD_FILOSOFOS];

pthread_t pais[QTD_PAI];
pthread_t maes[QTD_MAE];
pthread_t policiais[QTD_POLICIAL];
pthread_t ladroes[QTD_LADRAO];
pthread_t filhos[QTD_FILHOS];
pthread_t filhas[QTD_FILHAS];

sem_t free_pos_boat;
pthread_cond_t condicao_parada_filhas = PTHREAD_COND_INITIALIZER;
pthread_cond_t condicao_parada_filhos = PTHREAD_COND_INITIALIZER;
pthread_cond_t condicao_parada_ladrao = PTHREAD_COND_INITIALIZER;

pthread_mutex_t barco = PTHREAD_MUTEX_INITIALIZER;

int qtd_total_pessoas = QTD_FILHOS + QTD_FILHAS + QTD_PAI + QTD_MAE + QTD_POLICIAL + QTD_LADRAO + QTD_LUGARES_BARCO;
int pessoas_do_outro_lado = 0;
int adultos_no_barco = 0;

void * travessia_filhas(int id){    
    while(TRUE){
        //diminui as posições permitidas do barco
        sem_wait(&free_pos_boat);


        //aumenta as posições permitidas do barco
        sem_post(&free_pos_boat);
    }
}

void * travessia_filhos(int id){    
    while(TRUE){
        //diminui as posições permitidas do barco
        sem_wait(&free_pos_boat);


        //aumenta as posições permitidas do barco
        sem_post(&free_pos_boat);
    }
}

void * travessia_pai(int id){    
    while(TRUE){
        //diminui as posições permitidas do barco
        sem_wait(&free_pos_boat);


        //aumenta as posições permitidas do barco
        sem_post(&free_pos_boat);   
    }
}

void * travessia_mae(int id){    
    while(TRUE){
        //diminui as posições permitidas do barco
        sem_wait(&free_pos_boat);


        //aumenta as posições permitidas do barco
        sem_post(&free_pos_boat);
    }
}


void * travessia_policial(int id){    
    while(TRUE){
        //diminui as posições permitidas do barco
        sem_wait(&free_pos_boat);


        //aumenta as posições permitidas do barco
        sem_post(&free_pos_boat);
    }
}

void * travessia_ladrao(int id){    
    while(TRUE){
        //diminui as posições permitidas do barco
        sem_wait(&free_pos_boat);


        //aumenta as posições permitidas do barco
        sem_post(&free_pos_boat);
    }
}

int main(){
    int i, *id;

    sem_init(&free_pos_boat,0,QTD_LUGARES_BARCO);

    for (i = 0; i < QTD_FILHAS; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&filhas[i], NULL, travessia_filhas, (void *)(id));
    }

    for(i = 0; i < QTD_FILHAS; i++)
    {
        if(pthread_join(filhas[i], NULL))
        {
            printf("Falha ao juntar thread %d\n", i);
            return -1;
        }
    }

    for (i = 0; i < QTD_FILHOS; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&filhos[i], NULL, travessia_filhos, (void *)(id));
    }

    for(i = 0; i < QTD_FILHOS; i++)
    {
        if(pthread_join(filhos[i], NULL))
        {
            printf("Falha ao juntar thread %d\n", i);
            return -1;
        }
    }

    for (i = 0; i < QTD_PAI; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&pais[i], NULL, travessia_pai, (void *)(id));
    }

    for(i = 0; i < QTD_PAI; i++)
    {
        if(pthread_join(pais[i], NULL))
        {
            printf("Falha ao juntar thread %d\n", i);
            return -1;
        }
    }

    for (i = 0; i < QTD_MAE; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&maes[i], NULL, travessia_mae, (void *)(id));
    }

    for(i = 0; i < QTD_MAE; i++)
    {
        if(pthread_join(maes[i], NULL))
        {
            printf("Falha ao juntar thread %d\n", i);
            return -1;
        }
    }

    for (i = 0; i < QTD_POLICIAL; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&policiais[i], NULL, travessia_policial, (void *)(id));
    }

    for(i = 0; i < QTD_LADRAO; i++)
    {
        if(pthread_join(policiais[i], NULL))
        {
            printf("Falha ao juntar thread %d\n", i);
            return -1;
        }
    }

    for(i = 0; i < QTD_LADRAO; i++)
    {
        if(pthread_join(ladroes[i], NULL))
        {
            printf("Falha ao juntar thread %d\n", i);
            return -1;
        }
    }

    return 0;
}