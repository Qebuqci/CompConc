#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

#define ERRO -1

#define Boolean int
#define TRUE 1
#define FALSE -1

// VARIÁVEIS GLOBAIS
int dim, nthreads;

// Matrizes de entrada : A,B
float *matrizA;
float *matrizB;

// Matrizes de saída : S - sequencial , C - concorrente
float *matrizS;
float *matrizC;

// Funções de Exibição dos resultados
void print_matrix();
void gera_tabela(double Tseq, double Tconc, int nthreads, double ganho);

// Função que verifica a corretude da multiplicação feita sequencialmente e concorrentemente
Boolean verifica_mult(float *matrizS, float *matrizC);

// Estrutura de argumentos para as threads
typedef struct _targs {
    int id;     // identificador da linha que thread irá processar
    int dim;    // dimensão das matrizes que as threads irão processar
} targs;

/* Função de Multiplicação de Matrizes por thread *
 * Cada thread é responsável por calcular uma linha da matriz S de saída
 * Abordagem intercalada, ou seja, cada thread executa essa função
 * de maneira intercalada com as demais threads, até que a matriz de saída seja
 * construída.
 */
void *multmatrix_t(void *arg)
{
    targs *args = (targs *) arg;
    for(int i = args->id; i < args->dim; i += nthreads)
        for (int j = 0; j < args->dim; j++)
            for (int k = 0; k < args->dim; k++)
                matrizC[i * dim + j] += matrizA[k * dim + j] * matrizB[k * dim + i];
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t *threads;
    targs *args;
        
    double inicioS, fimS; // Variáveis de controle de tempo da multiplicação sequencial
    double inicioC, fimC; // Variáveis de controle de tempo da multiplicação concorrente
    
    double Tseq, Tconc, ganho;
    
    // 1. Inicialização do programa
    if (argc < 2) {
        printf("Digite: %s <dimensão da matrizes A,B> <número de threads>\n", argv[0]);
        return ERRO;
    }
    
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    if (nthreads > dim) nthreads = dim;
    
    // 2. Alocação de recursos (memória)
    matrizA = malloc(sizeof(float) * dim * dim);
    matrizB = malloc(sizeof(float) * dim * dim);
    matrizS = malloc(sizeof(float) * dim * dim);
    matrizC = malloc(sizeof(float) * dim * dim);
    threads = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    args = (targs *) malloc(sizeof(targs) * nthreads);
    
    if (matrizA == NULL || matrizB == NULL || matrizS == NULL || matrizC == NULL || threads == NULL || args == NULL) {
        printf("ERRO: malloc\n");
        return ERRO;
    }
    
    // 3. Inicialização das matrizes
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            matrizA[i * dim + j] = matrizB[i * dim + j] = i + j;
            matrizS[i * dim + j] = matrizC[i * dim + j] = 0;
        }
    }
    // 4. Multiplicação das Matrizes
    
    // 4.1 Multiplicação Sequencial das matrizes
    GET_TIME(inicioS);
    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++) 
            for (int k = 0; k < dim; k++)
                matrizS[i * dim + j] += matrizA[k * dim + j] * matrizB[k * dim + i];

    GET_TIME(fimS);

    // 4.2 Criação das threads para Multiplicação Concorrente das matrizes
    // Abordagem intercalada: cada thread processará uma linha da matriz C de maneira intercalada c/ as demais
    GET_TIME(inicioC);    
    for (int i = 0; i < nthreads; i++) {
        args[i].id = i;
        args[i].dim = dim;
        if (pthread_create(threads+i, NULL, multmatrix_t, (void *) (args + i))) {
            puts("ERRO: pthread_create");
            return ERRO;
        }
    }
    
    // 4.2 Aguarda o término de todas as threads 
    for (int i = 0; i < nthreads; i++)
        pthread_join(threads[i], NULL);
    
    GET_TIME(fimC);
    
    // 5. Verificação dos resultados
    if (verifica_mult(matrizS, matrizC) == TRUE) {
        Tseq = fimS - inicioS;
        Tconc = fimC - inicioC;
        ganho = (Tseq/Tconc);
    }
    else {
        puts("ERRO: multiplicação sequencial e concorrente não conferem!");
        return ERRO;
    }
            
    // 6. Exibição dos resultados
    //print_matrix();
    gera_tabela(Tseq, Tconc, nthreads, ganho);
    
    // 7. Desalocação dos recursos
    free(matrizA);
    free(matrizB);
    free(matrizS);
    free(matrizC);
    free(threads);
    free(args);
    
}

/* Verifica a igualdade entre cada elemento da matriz sequencial S
 * com cada elemento da matriz concorrente C
 * Se ambas as matrizes forem iguais, retorna TRUE
 * Caso contrário, retorna FALSE
*/
Boolean verifica_mult(float *matrizS, float *matrizC)
{
    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
            if (matrizS[i * dim + j] != matrizC[i * dim + j])
                return FALSE;
    return TRUE;
}

/* Imprime o nº de threads e as 
 * matrizes de entrada (A,B) e saída (S,C)
 */
void print_matrix()
{
    printf("Número de threads: %d\n", nthreads);
    // Threads 
    for (int i = 0; i < nthreads; i++)
        printf("Thread %d\n", i);
    
    puts("");
    
    // Matriz A
    puts("Matriz A");
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) 
            printf("%.1f ", matrizA[i * dim + j]); 
        puts("");
    }
    // Matriz B
    puts("Matriz B");
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) 
            printf("%.1f ", matrizB[i * dim + j]); 
        puts("");
    }
    // Matriz S
    puts("Matriz S");
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) 
            printf("%.1f ", matrizS[i * dim + j]); 
        puts("");
    }
    
    // Matriz C
    puts("Matriz C");
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) 
            printf("%.1f ", matrizC[i * dim + j]); 
        puts("");
    }
}

/* Formata uma saída tabelada de dados (dataset) para serem plotados, 
 * cujas colunas são:
 * #Tseq, tP, Tconc, nT, ganho
 * onde tP é o indicativo de thread principal e nT é número de threads. 
 * 
 * tP é sempre igual a 1, para indicar que é apenas a thread principal
 * executando a multiplicação.
 */
void gera_tabela(double Tseq, double Tconc, int nthreads, double ganho)
{
    printf("\t%lf\t\t\t1\t\t\t%lf\t\t%d\t\t%lf\n", Tseq, Tconc, nthreads, ganho);
}

