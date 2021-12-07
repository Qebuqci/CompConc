#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

#define ERRO -1

#define Boolean int
#define TRUE 1
#define FALSE 0

// Variáveis Globais
float *vetor;
long long int N; // nº de elementos do vetor
int nthreads;
float limiar_inferior, limiar_superior;

// Funções
Boolean verifica_encontrados(int enc_seq, int enc_conc);

/** Encontra Valores num intervalo Concorretemente 
 * Abordagem em bloco: Cada thread é responsável por
 * varrer um bloco do vetor e então contabilizar a
 * quantidade de valores encontrados nesse vetor que
 * pertencem ao intervalo definido.  
 **/
void *encontra_valores(void *arg)
{
    long int id_thread = (long int) arg;
    long int *encontrados_locais;

    encontrados_locais = (long int *) malloc(sizeof(long int));
    if (encontrados_locais == NULL) {
        fprintf(stderr, "ERRO: malloc\n");
        exit(ERRO);        
    }
    *(encontrados_locais) = 0;
    
    long long int tamBloco = N / nthreads;                      // Tamanho do bloco de cada thread
    long long int ini_bloco_thread = id_thread * tamBloco;      // Inicio do bloco de cada thread
    long long int fim_bloco_thread;                             // Fim do bloco de cada thread
    
    // Calcula o fim do bloco de cada thread
    if (id_thread == nthreads - 1) fim_bloco_thread = N;
    else fim_bloco_thread = ini_bloco_thread + tamBloco;
    
    // Encontra valores 
    for (long long int i = ini_bloco_thread; i < fim_bloco_thread; i++) {
        if (vetor[i] > limiar_inferior && vetor[i] < limiar_superior)
           *(encontrados_locais) += 1;
    }
    pthread_exit((void *) encontrados_locais);
}


int main(int argc, char *argv[])
{
    pthread_t *threads;
    long int *encontrados_thread;   // Nº de valores encontrados por thread
    long int encontrados_conc;      // Nº de valores encontrados total
    
    int encontrados_seq;
    
    // Variáveis de controle de tempo
    double Tseq, inicioS, fimS; // Variáveis de controle de tempo sequencial
    double Tconc, inicioC, fimC; // Variáveis de controle de tempo concorrente
    double ganho;
    
    // 1. Inicialização do programa
    if (argc < 3) {
        fprintf(stderr, "Uso do programa: %s <nº de elementos do vetor> <nº de threads>\n", argv[0]);
        return ERRO;
    }
    
    N = atoll(argv[1]);
    nthreads = atoi(argv[2]);
    
    // 2. Alocação de recursos (memória)
    vetor = malloc(sizeof(float) * N);
    threads = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if (vetor == NULL || threads == NULL) {
        fprintf(stderr, "ERRO : malloc\n");
        return ERRO;
    }
    
    // 3. Inicialização do vetor
    float j = 0;
    for (int i = 0; i < N; j++, i++)
        vetor[i] = j * 0.5;       // Foi colocado j * 0.5 para ficar facil conferir a corretude
        
    // 4. Leitura do intervalo da entrada padrão (stdin)
    puts("Informe os extremos dos intervalos separados por espaço");
    fscanf(stdin, "%f %f", &limiar_inferior, &limiar_superior);
    
    // 5. Encontra valores encontrados nesse intervalo (L_i, L_s)
    // 5.1 Implementação Sequencial 
    GET_TIME(inicioS);
    encontrados_seq = 0;
    
    for (int i = 0; i < N; i++)
        if (vetor[i] > limiar_inferior && vetor[i] < limiar_superior)
            encontrados_seq += 1;
    
    GET_TIME(fimS);
    
    // 5.2 Implementação Concorrente
    // Criação das threads
    GET_TIME(inicioC);
    encontrados_conc = 0;
    
    for (long int i = 0; i < nthreads; i++) {
        if (pthread_create(threads + i, NULL, encontra_valores, (void *) i)) {
            fprintf(stderr, "ERRO: pthread_create\n");
            return ERRO;
        }        
    }
    // Aguarda o término das threads
    for (long int i = 0; i < nthreads; i++) {
        if (pthread_join(threads[i], (void **) &encontrados_thread)) {
            fprintf(stderr, "ERRO: pthread_join\n");
            return ERRO;
        }
        encontrados_conc += *encontrados_thread;
        free(encontrados_thread);
    }
    GET_TIME(fimC);
    
    // 6. Exibição dos resultados em formato de tabela, cujas colunas são:
    // (Li, Ls): nº encontrados, Tseq, thread Principal, Tconc, nº Threads, ganho
    if (verifica_encontrados(encontrados_seq, encontrados_conc)) {
        Tseq  = fimS - inicioS;
        Tconc = fimC - inicioC;
        ganho = (Tseq / Tconc);
        fprintf(stdout, "(%.2f,%.2f):%d\t\t%lf\t\t\t1\t\t\t\t%lf\t\t%d\t\t%lf\n", limiar_inferior, limiar_superior, encontrados_conc, 
                Tseq, Tconc, nthreads, ganho);
    }
    else {
        fprintf(stderr, "Os valores encontrados sequencial e concorrente não conferem!\n");
        return ERRO;
    }
    
    // 7. Desalocação de recursos
    free(vetor);
    free(threads);
    
    return 0;
    
}
/** Confere se a quantidade de valores no intervalo
 *  são iguais, pela implementação sequencial e 
 *  pela implementação concorrente.
 **/
Boolean verifica_encontrados(int enc_seq, int enc_conc)
{
    if (enc_seq == enc_conc) return TRUE;
    return FALSE;
}
