#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "integral.h"

int contador_barreira = 0;      // Inicializa contador da barreira

// Funções Sequenciais

Integral *modo_integracao_seq(int modo_integracao, long double (*f)(double x, double *param), double *param, 
                              long double a, long double b, long double h )
{
    if (modo_integracao == RIEMANN)
        return riemann_sequencial(f, param, a, b, h);
    else if (modo_integracao == TRAPEZOIDAL)
        return trapezoidal_sequencial(f, param, a, b, h);
    else if (modo_integracao == SIMPSONS)
        return simpsons_sequencial(f,param, a, b, h);
    else if (modo_integracao == QUAD_GAUSSIANA)
        return NULL;
    
    return NULL;
}

Integral *riemann_sequencial(long double (*f)(double x, double *param), double *param, 
                             long double a, long double b, long double h)
{
    Integral *integral;
    long double i;                      // iésima coordenada x da partição
    long long int n, k;               // número de retângulos (subintervalos) n e k para indicar o késimo retangulo (subintervalo)
    
    integral = (Integral *) malloc(sizeof(Integral));
    if (integral == NULL)
        return NULL;
       
    printf("# Método de Somas de Riemann Sequencial. Particionado em retângulos...\n");
    integral->n = n = (b - a)/h;
    
    integral->Ln = integral->Rn = integral->Mn = 0;
    
    // Calcula a soma de Riemann 
    for (k = 0, i = a; k < n; k++, i+= h) {
        integral->Ln += f(i, param) * h;
        integral->Rn += f(i + h, param) * h;
        double mid = (i + i + h)/2;         // Ponto do meio 
        integral->Mn += f(mid, param) * h;
    }
    printf("Ln: %LF\nRn: %LF\nMn: %LF\n", integral->Ln, integral->Rn, integral->Mn);

    integral->sum = integral->Mn;
    
    return integral;
    
}

Integral *trapezoidal_sequencial(long double (*f)(double x, double *param), double *param, 
                                 long double a, long double b, long double h)
{
    long double xi, sum=0;
    long long int n;
    Integral *integral;
    
    integral = (Integral *) malloc(sizeof(Integral));
    if (integral == NULL)
        return NULL;
    
    printf("# Método trapezoidal Sequencial. Particionando em trapézios...\n");
    integral->n = n = (b - a)/h;
    for (int i = 1; i < n; i++) {
        xi = a + i * h;
        sum = sum + f(xi, param);
    }
       
    integral->Ln = integral->Rn = integral->Mn = 0;
    integral->sum = (h/2) * (f(a, param) + f(b, param) + 2 * sum);
        
    return integral;
}

Integral *simpsons_sequencial(long double (*f)(double x, double *param), double *param, 
                              long double a, long double b, long double h)
{
    long double xi, sum = 0;
    long long int n;
    Integral *integral;
    
    integral = (Integral *) malloc(sizeof(Integral));
    if (integral == NULL)
        return NULL;
    
    integral->n = n = (b - a)/h;
    for (int i = 1; i < n; i++) {
        xi = a + i * h;
        if ( i % 2 == 0)
            sum = sum + 2 * f(xi, param);
        else 
            sum = sum + 4 * f(xi, param);
    }
  
    integral->Ln = integral->Rn = integral->Mn = 0;
    integral->sum = (h/3) * ( f(a, param) + f(b, param) + sum);
    
    return integral;
}

// Funções Concorrentes

void barreira(int id_thread)
{
    // Entra na seção crítica
    pthread_mutex_lock(&lock);
    
    if (contador_barreira == nthreads - 1) {
        contador_barreira = 0;
        pthread_cond_broadcast(&cond_barreira);
        //printf("Thread: %d liberou a barreira\n", id_thread);
    }
    else {
        //printf("Thread: %d na barreira\n", id_thread);
        contador_barreira++;
        pthread_cond_wait(&cond_barreira, &lock);
    }
    // Sai da seção crítica
    pthread_mutex_unlock(&lock);
}

void *(*integracao_conc(int modo_int))(void *args)
{
    if ( modo_int == RIEMANN) {
        printf("# Método de Somas de Riemann Concorrente. Particionando em retângulos...\n");
        return (void *) &riemann_concorrente;
    }
    else if ( modo_int == TRAPEZOIDAL) {
        printf("# Método trapezoidal Concorrente. Particionando em trapézios...\n");
        return (void *) &trapezoidal_concorrente;
    }
    else if ( modo_int == SIMPSONS) {
        printf("# Concurrent Simpson's rule. \n");
        return (void *) &simpsons_concorrente;
    }
    
    return NULL;
}

void riemann_concorrente(void *args) 
{
    param_t *targs = (param_t *) args;
    
    // Variáveis locais de cada thread
    int id;
    long double (*g)(double x, double *param);
    double *param;
    long double a, i, b, h;
    Integral *integral;     // Estrutura de dados para armazenar os resultados locais de cada integração
    long long int n, k;
        
    integral = (Integral *) malloc(sizeof(Integral));
    if (integral == NULL)
        exit(ERRO);
    
    integral->n = n = (targs->b - targs->a)/targs->h;
    integral->Ln = integral->Rn = integral->Mn = 0;
    
    id = targs->id;
    g = targs->f;
    param = targs->param;
    a = targs->a;
    b = targs->b;
    h = targs->h;
    
    for (k = targs->id, i = a + (h * id); k < n; k += nthreads, i += h * nthreads) {
        //printf("ID: %d, k: %d\n", targs->id, k);
        integral->Ln += g(i, param) * h;
        integral->Rn += g(i + h, param) * h;
        double mid = (i + i + h)/2;         // Ponto do meio 
        integral->Mn += g(mid, param) * h;
    }
    integral->sum = integral->Mn;
    
    //printf("%d, %p, %lf %lf %lf %lf\n", id, integral, integral->Ln, integral->Rn, integral->Mn, integral->sum);
    
    integral_concorrente->n = integral->n;      // Aqui elas sobreescrevem para o mesmo valor
    
    pthread_mutex_lock(&lock);
    integral_concorrente->Ln += integral->Ln;
    integral_concorrente->Rn += integral->Rn;
    integral_concorrente->Mn += integral->Mn;
    integral_concorrente->sum += integral->sum;
    pthread_mutex_unlock(&lock);
       
    free(integral);
    
    pthread_exit(NULL);
}

void trapezoidal_concorrente(void *args)
{
    param_t *targs = (param_t *) args;
    
    // Variáveis locais de cada thread
    int id;
    long double (*g)(double x, double *param);
    double *param;
    long double a, b, h, sum, xi;
    Integral *integral;     // Estrutura de dados para armazenar os resultados locais de cada integração
    long long int n;
        
    integral = (Integral *) malloc(sizeof(Integral));
    if (integral == NULL)
        exit(ERRO);
    
    integral->n = n = (targs->b - targs->a)/targs->h;
    integral->Ln = integral->Rn = integral->Mn = 0;
    sum = 0;
    
    id = targs->id;
    g = targs->f;
    param = targs->param;
    a = targs->a;
    b = targs->b;
    h = targs->h;
    
    // Soma local trapeizodal
    for (int i = id + 1; i < n; i += nthreads) {
        xi = a + i * h;
        sum = sum + g(xi, param);
    }
       
    //pthread_mutex_lock(&lock);      // Todas as threads sobreescrevem essa parte
    integral_concorrente->n = n;
    integral_concorrente->sum = (h/2) * (g(a, param) + g(b, param)); 
    //pthread_mutex_unlock(&lock);

    barreira(id);
    
    pthread_mutex_lock(&lock);        // Essa parte não, as threads se excluem mutuamente
    integral_concorrente->sum += (h * sum);
    pthread_mutex_unlock(&lock);
      
    pthread_exit(NULL);

}

void simpsons_concorrente(void *args)
{
    param_t *targs = (param_t *) args;
    
    double id;
    long double (*g)(double x, double *param);
    double *param;
    long double a, b, h, sum, xi;
    Integral *integral;
    long long int n;
    
    
    integral = (Integral *) malloc(sizeof(Integral));
    if (integral == NULL)
        exit(ERRO);
    
    integral->n = n = (targs->b - targs->a)/targs->h;
    integral->Ln = integral->Rn = integral->Mn = 0;
    sum = 0;
    
    id = targs->id;
    g = targs->f;
    param = targs->param;
    a = targs->a;
    b = targs->b;
    h = targs->h;
    
    for (int i = id + 1; i < n; i += nthreads ) {
        xi = a + i * h;
        if ( i % 2 == 0 )
            sum = sum + 2 * g(xi, param);
        else 
            sum = sum + 4 * g(xi, param);
    }
  
    integral_concorrente->n = n;
    integral_concorrente->sum = (h/3) * ( g(a, param) + g(b, param) ); 
    
    barreira(id);
    
    pthread_mutex_lock(&lock);        
    integral_concorrente->sum += (h/3) * sum;
    pthread_mutex_unlock(&lock);
    
    pthread_exit(NULL);    
}
