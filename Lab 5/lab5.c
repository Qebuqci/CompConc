#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BOOLEAN int
#define TRUE 1
#define FALSE -1

#define NTHREADS 5

#define ERRO -2

// Variáveis Globais
BOOLEAN t5_estado = FALSE;      // Indica se a thread 5 já imprimiu
int t234_acc = 0;               // Se o valor de t234 for 3 então as threads 2,3,4 imprimiram
pthread_mutex_t lock;
pthread_cond_t  estado_t5;  
pthread_cond_t  estado_t234;

void* thread5(void* arg) 
{
    // Entrada na Seção Crítica
    pthread_mutex_lock(&lock);
    
    // Seção Crítica
    printf("Seja bem-vindo!\n");
    t5_estado = TRUE;

    // Saída da Seção Crítica
    pthread_cond_broadcast(&estado_t5);
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

void* thread1(void* arg)
{
    // Entrada na Seção Crítica
    pthread_mutex_lock(&lock);
    
    if (t5_estado == FALSE)
        pthread_cond_wait(&estado_t5, &lock);
    
    if (t234_acc < 3)
        pthread_cond_wait(&estado_t234, &lock);
    
    printf("Volte sempre!\n");
    t5_estado = FALSE;
    t234_acc = 0;
    
    // Saída da Seção Crítica
    pthread_mutex_unlock(&lock);
       
    pthread_exit(NULL);
}

void* thread2(void* arg)
{
    // Entrada na Seção Crítica
    pthread_mutex_lock(&lock);
    
    // Seção crítica
    if (t5_estado == FALSE)
        pthread_cond_wait(&estado_t5, &lock);
    
    printf("Fique a vontade.\n");
    t234_acc++;
    if (t234_acc == 3)
        pthread_cond_signal(&estado_t234);
    
    // Saída da Seção Crítica
    pthread_mutex_unlock(&lock);
}

void* thread3(void* arg)
{
    // Entrada na Seção Crítica
    pthread_mutex_lock(&lock);
    
    // Seção crítica
    if (t5_estado == FALSE)
        pthread_cond_wait(&estado_t5, &lock);
    
    printf("Sente-se por favor.\n");
    t234_acc++;
    if (t234_acc == 3)
        pthread_cond_signal(&estado_t234);
    
    // Saída da Seção Crítica
    pthread_mutex_unlock(&lock); 
}

void* thread4(void* arg)
{
    // Entrada na Seção Crítica
    pthread_mutex_lock(&lock);
    
    // Seção crítica
    if (t5_estado == FALSE)
        pthread_cond_wait(&estado_t5, &lock);
    
    printf("Aceita um copo d'água?.\n");
    t234_acc++;
    if (t234_acc == 3)
        pthread_cond_signal(&estado_t234);
    
    // Saída da Seção Crítica
    pthread_mutex_unlock(&lock);
}

int main(void)
{
    pthread_t threads[NTHREADS];
    
    // Inicializa os recursos de Sincronização
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&estado_t234, NULL);
    pthread_cond_init(&estado_t5, NULL);
    
    // Cria as Threads do programa
    pthread_create(&threads[0], NULL, thread1, NULL);
    pthread_create(&threads[1], NULL, thread2, NULL);
    pthread_create(&threads[2], NULL, thread3, NULL);
    pthread_create(&threads[3], NULL, thread4, NULL);
    pthread_create(&threads[4], NULL, thread5, NULL);

    // Espera as Threads do programa acabarem
    for (int i = 0; i < NTHREADS; i++)
        pthread_join(threads[i], NULL);
    
    // Desalocação dos recursos
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&estado_t5);
    pthread_cond_destroy(&estado_t234);

    
}
