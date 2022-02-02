#ifndef __INTEGRAL_H
#define __INTEGRAL_H

#define ERRO -1

#define RIEMANN 1
#define TRAPEZOIDAL 2
#define SIMPSONS 3
#define QUAD_GAUSSIANA 4

// Estrutura (Tipo abstrato) de dados para Integral
typedef struct _integral {    
    long double Ln, Rn, Mn;                     // Left sum, Right sum, Mid sum - usado em Riemann
    long double sum;                            // Global sum - resultado da Integral (geral) dos outros métodos
    long long int n;                                 // number of partitions (intervalos ou polígonos embutidos na região limitada)
} Integral;

// Estrutura de dados das threads
typedef struct _parametros_threads {
    int id;                                     // ID da thread
    long double a, b, h;                             // Intervalo [a,b] e stepsize h
    long double (*f)(double x, double *param);  // função a ser integrada
    double *param;                              // array com os parâmetros dessa função f
} param_t;

// Variáveis globais
int nthreads;
Integral *integral_concorrente;    // Estrutura de dados compartilhada entre as threads. Armazenar os resultados da integral concorrente
pthread_mutex_t lock;              // Variável de controle da exclusão mútua para escrever em integral_concorrente
pthread_cond_t cond_barreira;       // Variável de condição para barreira

// Funções de Integração Sequencial

/** Seleciona o modo de integração sequencial baseado no <MI> informado
 * Entrada: Modo de integração, função f a ser integrada, vetor de parametros de f, extremos <a,b> e stepsize h
 * Saída: Um ponteiro para uma estrutura que armazena os dados do resultado da integral computada
 **/
Integral *modo_integracao_seq(int modo_integracao, long double (*f)(double x, double *param), double *param, 
                              long double a, long double b, long double h );

/** Calcula a soma de Riemann de f no intervalo considerado com passo h (stepsize) escolhido. 
 * A soma insere n retângulos a esquerda (Ln), a direita (Rn) e nos pontos do meio (Mn) dos subintervalos da região limitada
 * Entrada: função f, extremos <a,b> e stepsize h
 * Saída: Um ponteiro para uma estrutura que armazena os dados do resultado da integral computada
 **/
Integral *riemann_sequencial(long double (*f)(double x, double *param), double *param, 
                             long double a, long double b, long double h);

/** Método trapezoidal de integração. Interpola os extremos dos subintervalos por um polinômio de grau 1 (reta) 
 * A soma insere n trapézios na região limitada (que são polinômios interpoladores de grau 1 em cada subintervalo)
 * Entrada: função f, extremos <a,b> e stepsize h
 * Saída: Um ponteiro para uma estrutura que armazena os dados do resultado da integral computada
 **/
Integral *trapezoidal_sequencial(long double (*f)(double x, double *param), double *param, 
                                 long double a, long double b, long double h);

/** Regra de Simpson para integração. Interpola os extremos dos subintervalos por um polinômio de grau 2 (parábola) 
 * A soma insere n polinômios quadráticos na região limitada 
 * Entrada: função f, extremos <a,b> e stepsize h
 * Saída: Um ponteiro para uma estrutura que armazena os dados do resultado da integral computada
 **/
Integral *simpsons_sequencial(long double (*f)(double x, double *param), double *param, 
                              long double a, long double b, long double h);

//Integral *quadGaussiana_sequencial

// Funções de Integração Concorrentes 
void barreira(int id_thread);       // Barreira para sincronização das threads

/** Seleciona o modo de integração concorrente baseado no <MI> escolhido para o modo sequencial
 * Entrada: Modo de integração
 * Saída: Um ponteiro para uma função a ser chamada pela thread
 **/
void *(*integracao_conc(int modo_int))(void *args);

/** Versão concorrente dos métodos descritos acima **/
void riemann_concorrente(void *args);
void trapezoidal_concorrente(void *args);
void simpsons_concorrente(void *args);
//void quadGaussiana_concorrente(void *args);

#endif
