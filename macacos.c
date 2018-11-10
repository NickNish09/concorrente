#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MACACOS_ESQUERDA 20
#define MACACOS_DIREITA 20
#define TRUE 1
#define ESQUERDA 13
#define DIREITA 17

pthread_mutex_t mutex_esquerda;
pthread_mutex_t mutex_direita;
pthread_mutex_t entrada;
pthread_mutex_t turno;
pthread_mutex_t mutex_aux;

int esquerda = 0;
int direita = 0;
int *dir;

void * passagem (void *m, int direction){
    int otherside;
    int i = *((int *) m);
    if(direction == ESQUERDA){
        mutex_aux = mutex_esquerda;
        dir = &esquerda;
        otherside = 5173174;
    } else {
        mutex_aux = mutex_direita;
        dir = &direita;
        otherside = 35903734;
    }
    while(TRUE){
        pthread_mutex_lock(&turno);
        pthread_mutex_lock(&mutex_aux); //
        *(dir)++; // mais um macaco da esquerda saiu
        if(*(dir) > 0){
            pthread_mutex_lock(&entrada); // impede novas entradas
        }
        pthread_mutex_unlock(&mutex_aux);
        pthread_mutex_unlock(&turno);
        printf("Macaco indo da %d para %d",direction,otherside);
        sleep(1);
        pthread_mutex_lock(&mutex_aux);
        *(dir)--;
        printf("Encerrada passagem do macaco %d",i);
        if(*(dir) == 0){
            pthread_mutex_unlock(&entrada);
        }
        pthread_mutex_unlock(&mutex_aux);
    }

    pthread_exit(0);
}

int main(int argc, char * argv[])
{
    pthread_t m_o[MACACOS_DIREITA+MACACOS_ESQUERDA];

    pthread_mutex_init(&mutex_esquerda, NULL);
    pthread_mutex_init(&mutex_direita, NULL);
    pthread_mutex_init(&entrada, NULL);
    pthread_mutex_init(&turno, NULL);

    int *id;
    int i = 0;
    for(i = 0; i < MACACOS_DIREITA+MACACOS_ESQUERDA; ++i)
    {
        id = (int *) malloc(sizeof(int));
        *id = i;
        if(i%2 == 0){
          if(pthread_create(&m_o[i], NULL, passagem((void*)id,ESQUERDA), (void*)id))
          {
		
            printf("Não pode criar a thread %d\n", i);
            return -1;
          }
        }else{
         if(pthread_create(&m_o[i], NULL, passagem((void*)id,DIREITA), (void*)id))
         {
            printf("Não pode criar a thread %d\n", i);
            return -1;
         }
        }
	id++;
    }

   

    for(i = 0; i < MACACOS_DIREITA+MACACOS_ESQUERDA; ++i)
    {
        if(pthread_join(m_o[i], NULL))
        {
            printf("Could not join thread %d\n", i);
            return -1;
        }
    }



    printf("TERMINOU!\n");

    return 0;
}