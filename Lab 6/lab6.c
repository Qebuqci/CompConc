#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 4
#define ERRO -1

// Variáveis Globais
int vetor[NTHREADS];

int contador_barreira = 0;
pthread_mutex_t lock;
pthread_cond_t cond_barreira;

void barreira(int id_thread)
{
    // Entra na seção crítica
    pthread_mutex_lock(&lock);
    
    if (contador_barreira == NTHREADS - 1) {
        contador_barreira = 0;
        pthread_cond_broadcast(&cond_barreira);
        printf("Thread: %d liberou a barreira\n", id_thread);
    }
    else {
        printf("Thread: %d na barreira\n", id_thread);
        contador_barreira++;
        pthread_cond_wait(&cond_barreira, &lock);
    }
    // Sai da seção crítica
    pthread_mutex_unlock(&lock);
}

void* soma(void* arg)
{
    long int *acc; 
    long int id = (long int) arg;
   
    
    acc = (long int *) malloc(sizeof(long int));    
    
    for (int i = 0; i < NTHREADS; i++) {
        printf("Iteração: %d\tThread: %d\n", i, id);
        for (int j = 0; j < NTHREADS; j++) 
            *acc += vetor[j];
        
        barreira(id);
        vetor[id] = 3 * i;
        barreira(id);
    }
    
    pthread_exit((void *) acc);
}

int main(void)
{
    pthread_t threads[NTHREADS];
    long int acc_thread;
        
    // 1. Inicialização do vetor
    for (int i = 0; i < NTHREADS; i++)
        vetor[i] = 2 * i;
    
    // 2. Criação das threads
    for (long int i = 0; i < NTHREADS; i++) {
        if (pthread_create(threads + i, NULL, soma, (void *) i)) {
            puts("ERRO: pthread_create()");
            return ERRO;
        }
    }
    
    // 3. Espera as threads terminarem
   for (int i = 0; i < NTHREADS; i++) {
        if (pthread_join(threads[i], (void **) &acc_thread)) {
            puts("ERRO: pthread_join()");
            return ERRO;            
        }
        printf("Thread: %d \t Valor recebido: %d\n", i, *((int *) acc_thread));
    }
    
    // 4. Desalocação de recursos
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond_barreira);
    
    return 0;
}
