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

#define VAZIO 74710
#define PAI 941
#define MAE 843
#define FILHA 71784
#define FILHO 71780
#define POLICIAL 90715147
#define LADRAO 745840

pthread_t pais[QTD_PAI];
pthread_t maes[QTD_MAE];
pthread_t policiais[QTD_POLICIAL];
pthread_t ladroes[QTD_LADRAO];
pthread_t filhos[QTD_FILHOS];
pthread_t filhas[QTD_FILHAS];

sem_t free_pos_boat;
pthread_cond_t condicao_parada_criancas = PTHREAD_COND_INITIALIZER;

pthread_mutex_t barco = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_adultos = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_criancas = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t lock_barco = PTHREAD_MUTEX_INITIALIZER;

int qtd_total_pessoas = QTD_FILHOS + QTD_FILHAS + QTD_PAI + QTD_MAE + QTD_POLICIAL + QTD_LADRAO + QTD_LUGARES_BARCO;
int pessoas_do_outro_lado = 0;
int adultos_no_barco = 0;

int barcaca[QTD_LUGARES_BARCO];

void showPosicoes(){
    int i;
    printf("Posicoes do barco: ");
    for(i=0;i<QTD_LUGARES_BARCO;i++){
        if(barcaca[i] == FILHA){
            printf(" [FILHA] ");
        } else if (barcaca[i] == FILHO){
            printf(" [FILHO] ");
        } else if (barcaca[i] == PAI){
            printf(" [PAI] ");
        } else if (barcaca[i] == MAE){
            printf(" [MAE] ");
        } else if (barcaca[i] == POLICIAL){
            printf(" [POLICIAL] ");
        } else if(barcaca[i] == LADRAO){
            printf(" [LADRAO] ");
        } else {
            printf(" [VAZIO] ");
        }
    }
    printf("\n");
}

void showBarco(int descolamento){
  int i,j,n,b;
  float k;
  n = QTD_LUGARES_BARCO;
  b=n-1;


  for(i=0;i<n;i++)
  {
    for(j=0;j<((b+(n*2))+descolamento);j++)
    {
      printf(" ");
    }
    b--;

    for(k=0;k<=i;k+=0.5)
    {
      printf("*");
    }
    printf("\n");
  }

  int l,p=0,r;
  r=(n*6);
  for(i=0;i<n;i++)
  {
    for(j=0;j<(p+descolamento);j++)
    {
      printf(" ");
    }
    p++;

    for(l=0;l<r;l++)
    {
      printf("*");
    }
    r-=2;
    printf("\n");
  }

}

void showTransicao(){
    printf("\n");
    showPosicoes();
    printf("Atravessando...\n");
    /*showBarco(0);
    sleep(1);
    //system("clear");
    showBarco(5);
    sleep(1);
    //system("clear");
    showBarco(10);
    sleep(1);
    //system("clear");
    showBarco(15);
    sleep(1);
    //system("clear");
    showBarco(20);
    sleep(1);
    //system("clear");
    showBarco(25);
    printf("\n");*/
}

void * travessia_filhas(void * arg){    
    int id = *((int*)arg);
    while(TRUE){
        pthread_mutex_lock(&lock_criancas);
        //diminui as posições permitidas do barco
        sem_wait(&free_pos_boat);
        printf("Filha %d: Consegui entrar no barco...\n",id);
        if(barcaca[0] == VAZIO){
            barcaca[0] = FILHA;
        } else {
            barcaca[1] = FILHA;
        }
        //enquanto nao tem adultos no barco, filhas esperam
        while(adultos_no_barco == 0){
            printf("Filha %d: Nenhum adulto pra dirigir o barco...Vou esperar\n",id);
            pthread_cond_wait(&condicao_parada_criancas,&barco);
        }

        //pthread_mutex_lock(&lock_barco);
        showTransicao();
        //pthread_mutex_unlock(&lock_barco);
        sleep(2);

        printf("Filha %d: Cheguei do outro lado!\n",id);
        //aumenta as posições permitidas do barco
        if(barcaca[0] == FILHA){
            barcaca[0] = VAZIO;
        } else {
            barcaca[1] = VAZIO;
        }
        //Libera posicao
        sem_post(&free_pos_boat);

        pthread_mutex_unlock(&lock_criancas);
        
        sleep(2);
    }
}

void * travessia_filhos(void * arg){    
    int id = *((int*)arg);
    while(TRUE){
        pthread_mutex_lock(&lock_criancas);
        //diminui as posições permitidas do barco
        sem_wait(&free_pos_boat);
        printf("Filho %d: Consegui entrar no barco...\n",id);
        
        if(barcaca[0] == VAZIO){
            barcaca[0] = FILHO;
        } else {
            barcaca[1] = FILHO;
        }

        //enquanto nao tem adultos no barco, filhos esperam
        while(adultos_no_barco == 0){
            printf("Filho %d: Nenhum adulto pra dirigir o barco...Vou esperar\n",id);
            pthread_cond_wait(&condicao_parada_criancas,&barco);
        }

        //pthread_mutex_lock(&lock_barco);
        showTransicao();
        //pthread_mutex_unlock(&lock_barco);
        sleep(2);

        printf("Filho %d: Cheguei do outro lado!\n",id);

        //aumenta as posições permitidas do barco
        if(barcaca[0] == FILHO){
            barcaca[0] = VAZIO;
        } else {
            barcaca[1] = VAZIO;
        }
        //Libera posicao
        sem_post(&free_pos_boat);

        pthread_mutex_unlock(&lock_criancas);

        sleep(2);
    }
}

void * travessia_pai(void * arg){    
    int id = *((int*)arg);
    while(TRUE){
        int pos_free = 0;
        //diminui as posições permitidas do barco
        sem_wait(&free_pos_boat);
        printf("Pai %d: consegui entrar no barco!\n",id);
        if(barcaca[0] == VAZIO){
            barcaca[0] = PAI;
        } else {
            barcaca[1] = PAI;
            pos_free = 1;
        }
        //Se conseguiu pegar uma das posições do barco, incrementa o número de adultos para mostrar que pode-se atravessar as crianças
        pthread_mutex_lock(&lock_adultos);
        adultos_no_barco++;
        if(adultos_no_barco == 1){
            pthread_cond_broadcast(&condicao_parada_criancas);
        }
        //showPosicoes();

        //printf("Pai %d: Fui de moto...\n",id);
        
        //pthread_mutex_lock(&lock_barco);
        showTransicao();
        //pthread_mutex_unlock(&lock_barco);
        sleep(2);

        adultos_no_barco--;
        printf("Pai %d: Cheguei do outro lado, ha\n",id);
        barcaca[pos_free] = VAZIO;
        pthread_mutex_unlock(&lock_adultos);
        //aumenta as posições permitidas do barco
        sem_post(&free_pos_boat);
        sleep(10);
    }
}

void * travessia_mae(void * arg){    
    int id = *((int*)arg);
    while(TRUE){
        //diminui as posições permitidas do barco
        sem_wait(&free_pos_boat);
        printf("Mae %d: consegui entrar no barco!\n",id);
        if(barcaca[0] == VAZIO){
            barcaca[0] = MAE;
        } else {
            barcaca[1] = MAE;
        }
        //Se conseguiu pegar uma das posições do barco, incrementa o número de adultos para mostrar que pode-se atravessar as crianças
        pthread_mutex_lock(&lock_adultos);
        adultos_no_barco++;
        if(adultos_no_barco == 1){
            pthread_cond_broadcast(&condicao_parada_criancas);
        }
        
        //printf("Mae %d: Fui de moto...\n",id);
        //pthread_mutex_lock(&lock_barco);
        showTransicao();
        //pthread_mutex_unlock(&lock_barco);
        sleep(2);
        adultos_no_barco--;

        printf("Mae %d: Cheguei do outro lado, ha\n",id);
        pthread_mutex_unlock(&lock_adultos);
        //aumenta as posições permitidas do barco
        if(barcaca[0] == MAE){
            barcaca[0] = VAZIO;
        } else {
            barcaca[1] = VAZIO;
        }
        sem_post(&free_pos_boat);
        sleep(10);
    }
}


void * travessia_policial(void * arg){    
    int id = *((int*)arg);
    while(TRUE){
        
        //diminui as posições permitidas do barco
        sem_wait(&free_pos_boat);
        printf("Policial %d: consegui entrar no barco!\n",id);
        if(barcaca[0] == VAZIO){
            barcaca[0] = POLICIAL;
        } else {
            barcaca[1] = POLICIAL;
        }
        //Se conseguiu pegar uma das posições do barco, incrementa o número de adultos para mostrar que pode-se atravessar as crianças
        pthread_mutex_lock(&lock_adultos);
        adultos_no_barco++;
        if(adultos_no_barco == 1){
            pthread_cond_broadcast(&condicao_parada_criancas);
        }

        //printf("Policial %d: Fui de moto...\n",id);

        //pthread_mutex_lock(&lock_barco);
        showTransicao();
        //pthread_mutex_unlock(&lock_barco);
        sleep(2);

        adultos_no_barco--;
        printf("Policial %d: Cheguei do outro lado, ha\n",id);
        pthread_mutex_unlock(&lock_adultos);
        //aumenta as posições permitidas do barco
        if(barcaca[0] == POLICIAL){
            barcaca[0] = VAZIO;
        } else {
            barcaca[1] = VAZIO;
        }
        sem_post(&free_pos_boat);
        sleep(10);
    }
}

void * travessia_ladrao(void * arg){    
    int id = *((int*)arg);
    while(TRUE){
        pthread_mutex_lock(&lock_criancas);
        //diminui as posições permitidas do barco
        sem_wait(&free_pos_boat);
        printf("Ladrao %d: Consegui entrar no barco...\n",id);

        if(barcaca[0] == VAZIO){
            barcaca[0] = LADRAO;
        } else {
            barcaca[1] = LADRAO;
        }
        //enquanto nao tem adultos no barco, filhas esperam
        while(adultos_no_barco == 0){
            printf("Ladrao %d: Nenhum adulto pra dirigir o barco...Vou esperar\n",id);
            pthread_cond_wait(&condicao_parada_criancas,&barco);
        }

        //pthread_mutex_lock(&lock_barco);
        showTransicao();
        //pthread_mutex_unlock(&lock_barco);
        sleep(2);

        printf("Ladrao %d: Cheguei do outro lado!\n",id);

        //aumenta as posições permitidas do barco
        if(barcaca[0] == LADRAO){
            barcaca[0] = VAZIO;
        } else {
            barcaca[1] = VAZIO;
        }
        sem_post(&free_pos_boat);

        pthread_mutex_unlock(&lock_criancas);

        sleep(2);
    }
}

int main(){
    int i, *id;

    sem_init(&free_pos_boat,0,QTD_LUGARES_BARCO);

    for(i=0;i<QTD_LUGARES_BARCO;i++){
        barcaca[i] = VAZIO;
    }
    
    for (i = 0; i < QTD_FILHAS; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&filhas[i], NULL, travessia_filhas, (void *)(id));
    }

    for (i = 0; i < QTD_FILHOS; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&filhos[i], NULL, travessia_filhos, (void *)(id));
    }

    for (i = 0; i < QTD_PAI; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&pais[i], NULL, travessia_pai, (void *)(id));
    }

    for (i = 0; i < QTD_MAE; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&maes[i], NULL, travessia_mae, (void *)(id));
    }

    for (i = 0; i < QTD_POLICIAL; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&policiais[i], NULL, travessia_policial, (void *)(id));
    }

    for (i = 0; i < QTD_LADRAO; i++)
    {
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&ladroes[i], NULL, travessia_ladrao, (void *)(id));
    }

    for(i = 0; i < QTD_PAI; i++)
    {
        if(pthread_join(pais[i], NULL))
        {
            printf("Falha ao juntar thread %d\n", i);
            return -1;
        }
    }

    for(i = 0; i < QTD_MAE; i++)
    {
        if(pthread_join(maes[i], NULL))
        {
            printf("Falha ao juntar thread %d\n", i);
            return -1;
        }
    }

    for(i = 0; i < QTD_POLICIAL; i++)
    {
        if(pthread_join(policiais[i], NULL))
        {
            printf("Falha ao juntar thread %d\n", i);
            return -1;
        }
    }

    for(i = 0; i < QTD_FILHOS; i++)
    {
        if(pthread_join(filhos[i], NULL))
        {
            printf("Falha ao juntar thread %d\n", i);
            return -1;
        }
    }

    for(i = 0; i < QTD_FILHAS; i++)
    {
        if(pthread_join(filhas[i], NULL))
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