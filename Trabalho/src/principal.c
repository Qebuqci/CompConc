#include <stdio.h>
#include <stdlib.h>
#include "felementar.h"
#include "integral.h"
#include "timer.h"
#include <pthread.h>

#define ERRO -1

#define MAXLINT_NEG -9223372036854775807

// Declaração de funções auxiliares

/** Le parâmetros de um arquivo de entrada e coloca-os num vetor de double
 * Entrada: Um ponteiro para um arquivo
 * Saída: Um vetor de double com os parâmetros lidos. 
 *  Tais parâmetros são os argumentos das funções elementares do programa 
 * **/
double *leParametrosArquivo(FILE *parq);  

/** Imprime de maneira tabular informações sobre o tempo de execução do programa
 * Entrada: nº de Particionamentos da Integral, Tseq, nThreads, Tconc, ganho (aceleração) 
 * Saída: É uma tabela com esses argumentos de entrada 
 **/
void logTempo(int nParticionamentos, double Tseq, int nthreads, double Tconc, double ganho);

int main(int argc, char *args[])
{
    //puts("# Programa Concorrente para integrar funções elementares");
    if (argc < 8 ) {
        puts("Uso: ./integralc <MI> <N> <FE> <a> <b> <h> <nome_arquivo_parametros_funcao>");
        return ERRO;
    }
    
    // 1. Declaração das variáveis iniciaiS
    FILE *parquivo;                                 // Arquivo com os parâmetros das funções
    long double a, b, h;                            // intervalo [a,b] e passo (stepsize) h
    int modo_integracao, nmagico_funcao;            // <MI> e <FE>
    long double (*f)(double x, double *param);      // Ponteiro para uma função genérica
    double *param;                                  // Vetor para os parâmetros das funções vindas dos arquivos
    
    Integral *integral_sequencial;                  // Armazena os resultados da integral sequencial
        
    // 1.1 Declaração das variáveis de marcação de tempo
    double inicioS, fimS;                           // Variáveis de controle de tempo da integração sequencial
    double inicioC, fimC;                           // Variáveis de controle de tempo da integração concorrente
    double Tseq, Tconc, ganho;
    
    // 1.2 Declaração das variáveis de threads
    pthread_t *threads;                             // Vetor de threads do sistema
    param_t *targs;                                 // Argumentos das threads de um tipo especial param_t definido
    void *(*integracao_concorrente)(void *args);     // Ponteiro para uma função de thread (seleção do <MI> concorrente)
    
    // 2. Definição das variáveis iniciais
    modo_integracao = atoi(args[1]);
    nthreads = atoi(args[2]);
    nmagico_funcao = atoi(args[3]);
    a = strtold(args[4], NULL);
    b = strtold(args[5], NULL);
    h = strtold(args[6], NULL);
        
    // 2.1 Verificação das entradas do programa, para a prevenção de erros
    if (a > b) {
        puts("Erro: Extremo <a> deve ser menor do que o Extremo <b> no intervalo [a,b]");
        return ERRO;
    }
    
    if ( (long long int) ((b - a)/h) <= MAXLINT_NEG ) {        
        puts("Erro: Stepsize muito pequeno para esse intervalo");
        return ERRO;
    }
    
    parquivo = fopen(args[7], "r");             // Abre o arquivo
    if (parquivo == NULL) {
        puts("Erro: Arquivo inválido");
        return ERRO;
    }
    param = leParametrosArquivo(parquivo);
    fclose(parquivo);                           // Libera o arquivo
    
    if (param == NULL) {                        // Erro no arquivo de parâmetros
        return ERRO;                            
    }
    
    f = funcao(nmagico_funcao, param);          // Seleciona uma função elementar
    if (f == NULL) {
        printf("Erro no número mágico de entrada. Nenhuma função pode ser selecionada\n");
        return ERRO;
    }
        
    // 2.2 Inicialização das variáveis de thread
    threads = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    targs = (param_t *) malloc(sizeof(param_t) * nthreads);
    integral_concorrente = (Integral *) malloc(sizeof(Integral));
    
    if (threads == NULL || targs == NULL || integral_concorrente == NULL) {
        printf("ERRO: malloc\n");
        return ERRO;
    }
    
    // 2.3 Inicializa a estrutura de dados compartilhada para armazenar os resultados da integral concorrente
    integral_concorrente->Ln = integral_concorrente->Rn = integral_concorrente->Mn = integral_concorrente->sum = 0;
    integral_concorrente->n = 0;
    
    // 2.4 Define as threads do sistema e atribui os parâmetros de cada thread
    for (int i = 0; i < nthreads; i++) {
        threads[i] = i;
        targs[i].a = a;
        targs[i].b = b;
        targs[i].h = h;
        targs[i].f = f;
        targs[i].param = param;
        targs[i].id = i;       
    }
    
    // 3. Calcula a Integral Sequencial de acordo com o modo de integração (<MI>) escolhido
    GET_TIME(inicioS);
    integral_sequencial = modo_integracao_seq(modo_integracao, f, param, a, b, h);
    GET_TIME(fimS);
    if (integral_sequencial == NULL) {
        printf("Erro no modo de integração de entrada. Nenhuma integral pode ser calculada\n");
        return ERRO;
    }
        
    // Imprime os resultados sequenciais
    printf("Número de partições %lld\n", integral_sequencial->n);
    printf("Resultado da integral Sequencial: %.13LF\n", integral_sequencial->sum);
    
    // 4. Calcula a Integral Concorrente pelo método escolhido sequencialmente
    pthread_mutex_init(&lock, NULL);            // Inicializa o lock para exclusão mútua entre as threads
    pthread_cond_init (&cond_barreira, NULL);   // Inicializa a variável de condição de barreira
    
    // 4.1 Seleciona o método de Integração Concorrente de acordo com o modo de integração (<MI>) escolhido sequencialmente
    integracao_concorrente = integracao_conc(modo_integracao); // ! Verificação se o modo de integração é válido foi feita acima

    // 4.2 Criação das threads. Abordagem intercalada. Cada thread processará uma partição de soma local para compito da Integral
    GET_TIME(inicioC);
    for (int i = 0; i < nthreads; i++) 
        if (pthread_create(threads + i, NULL, integracao_concorrente, (void *) (targs + i))) {
            puts("Erro: pthread_create");
            return ERRO;
        }
    
    // 4.3 Aguarda o término de todas as threads 
    for (int i = 0; i < nthreads; i++)
        pthread_join(threads[i], NULL);
        
    GET_TIME(fimC);
    
    pthread_cond_destroy(&cond_barreira);   // Destroi a variável de condição de barreira
    pthread_mutex_destroy(&lock);           // Destroi o lock 
    
    // Imprime os resultados concorrentes
    printf("Número de partições %lld\n", integral_concorrente->n);
    printf("Resultado da integral Concorrente: %.13LF\n", integral_concorrente->sum);

    // 5. Compito dos tempos e impressão dos resultados de perfomance
    Tseq = fimS - inicioS;
    Tconc = fimC - inicioC;
    ganho = Tseq/Tconc;
    logTempo(integral_concorrente->n, Tseq, nthreads, Tconc, ganho);
    
    // 6. Libera recursos alocados do programa
    free(param);
    free(integral_sequencial);
    free(integral_concorrente);
    free(threads);
    free(targs);
    
    return 0;
}

double* leParametrosArquivo(FILE *parq)
{
    double *param;
    int qntParam;
    fscanf(parq, "%d", &qntParam);
       
    param = (double *) malloc(sizeof(double) * (qntParam + 2) );
    
    param[0] = qntParam;
    for (int i = 1; i <= qntParam + 1; i++)
        fscanf(parq, "%lf", param + i);
    
    
    return param;
}

void logTempo(int nPart, double Tseq, int nthreads, double Tconc, double ganho)
{
    int nDigitos = 1;
    printf("# LOG TEMPO #\n");
    printf("PARTIÇÕES\tTseq\t\tTHREADS\t\tTconc\t\tGANHO\n");
    if (Tseq > nDigitos)
        printf("%d\t%lf\t%d\t\t%lf\t%lf\n", nPart, Tseq, nthreads, Tconc, ganho);
    else
        printf("%d\t\t%lf\t%d\t\t%lf\t%lf\n", nPart, Tseq, nthreads, Tconc, ganho);

}

