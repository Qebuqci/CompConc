#include <stdio.h>
#include <pthread.h>

#define NTHREADS 2
#define TAM_V 10000
#define LIM_T1 5000

// Divide a tarefa de incrementar o vetor em dois seguimentos
// T1 incrementa de 0 a 5000 e T2 incrementa de 5000 a 9999
int vetor[TAM_V];
int t1_i = 0;
int t2_i = 5000;

void* pow2(void *arg)
{
    int tid = *(int*) arg;
    if (tid == 0)
        for (; t1_i < LIM_T1; t1_i++)
            vetor[t1_i] = vetor[t1_i] * vetor[t1_i];
    else
        for (; t2_i < TAM_V; t2_i++)
            vetor[t2_i] = vetor[t2_i] * vetor[t2_i];
    pthread_exit(NULL);   
}

int main()
{
    int thread, i, tid_local[NTHREADS];
    pthread_t tid[NTHREADS];
        
    // 1. Inicializa o vetor
    for (i = 0; i < TAM_V; i++)
        vetor[i] = i;
    
    // 2. Cria as threads e passa seus ids para função pow2
    for (thread = 0; thread < NTHREADS; thread++) {
        tid_local[thread] = thread;
        if (pthread_create(&tid[thread], NULL, pow2, (void*) &tid_local[thread]))
            puts("Erro no pthread_create()");
    }
        
    // Espera as threads terminarem de elevar os elementos ao quadrado
    for (thread = 0; thread < NTHREADS; thread++) {
        if (pthread_join(tid[thread], NULL))
            puts("Erro no pthread_join()");
    }
    
    // Imprimindo vetor    
    printf("# VETOR IMPRESSO #\ni \t i^2\n");
    for (i = 0; i < TAM_V; i++)
        printf("%d \t %d\n", i, vetor[i]);
    
    pthread_exit(NULL);
    return 0;
    
    
}
