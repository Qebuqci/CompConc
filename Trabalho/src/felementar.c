#include "felementar.h"
#include <math.h>
#include <stdio.h>
#include "fextras.h"

#define ERRO -1

// Implementação das funções elementares baseado no vetor de parâmetros que recebem

long double polinomio(double x, double *param)
{
    double P = 0;
    int grau;
   
    grau = (int) param[0];
    for (int i = 1; i <= grau + 1; i++)
        P += param[i] * pow(x, i - 1);
          
    return P;
}

long double seno(double x, double *param) 
{
    double C, w, phi;
    
    C = param[0];
    w = param[1];
    phi = param[2];
        
    return C * sin(w * x + phi);
    
}

long double cosseno(double x, double *param) 
{
    double C, w, phi;
    
    C = param[0];
    w = param[1];
    phi = param[2];
    
    return C * cos(w * x + phi);
}

long double tangente(double x, double *param) 
{
    double C, w, phi;
    
    C = param[0];
    w = param[1];
    phi = param[2];
    
    return C * tan(w * x + phi);
}

long double secante(double x, double *param) 
{
    double C, w, phi;
    
    C = param[0];
    w = param[1];
    phi = param[2];
        
    return C/cos(w * x + phi);
}

long double cossecante(double x, double *param) 
{
    double C, w, phi;
    
    C = param[0];
    w = param[1];
    phi = param[2];
    
    return C/sin(w * x + phi);
}

long double cotangente(double x, double *param) 
{
    double C, w, phi;
    
    C = param[0];
    w = param[1];
    phi = param[2];
    
    return C/tan(w * x + phi);
    
}

long double aseno(double x, double *param) 
{
    double C, w, phi;
    
    C = param[0];
    w = param[1];
    phi = param[2];
    
    return C * asin(w * x + phi);
}

long double acosseno(double x, double *param) 
{
    double C, w, phi;
    
    C = param[0];
    w = param[1];
    phi = param[2];
    
    return C * acos(w * x + phi);
}

long double atangente(double x, double *param) 
{
    double C, w, phi;
    
    C = param[0];
    w = param[1];
    phi = param[2];

    return C * atan(w * x + phi);
    
}

long double asecante(double x, double *param) 
{
    double C, w, phi;
    
    C = param[0];
    w = param[1];
    phi = param[2];
    
    return C/cos(w * x + phi);  // TA ERRADO O CALCULO
}

long double acossecante(double x, double *param) 
{
    double C, w, phi;
    
    C = param[0];
    w = param[1];
    phi = param[2];
    
    return 1;   // FALTOU O CALCULO
}

long double acotangente(double x, double *param) 
{
    double C, w, phi;
    
    C = param[0];
    w = param[1];
    phi = param[2];
 
    return 1;   // FALTOU O CALCULO
}

long double exponencial(double x, double *param) 
{
    double C, omega, k;
    
    C = param[0];
    omega = param[1];
    k = param[2];
    
    return C*exp(x * omega + k);       
}

long double ln(double x, double  *param) 
{
    double C, omega, k;
    
    C = param[0];
    omega = param[1];
    k = param[2];
    
    return C * log(x * omega + k);
}

// Seleciona a função elementar baseado no número mágico de entrada e imprime a função escolhida com seus parâmetros
long double (*funcao(int numeroMagico, double *param))(double x,  double *param)
{      
    int i, k;
    if (numeroMagico == POLINOMIO) {
        printf("f(x) = ");
        for (i = param[0] + 1, k = param[0]; i > 0 && k > 0; i--, k--)
            if (param[i] != 0)
                printf("(%dx^%d) + ", (int) param[i], k);
        
        printf("%d", (int) param[++k]);
        printf("\n");
        return polinomio;
    }
    else if (numeroMagico == SENO) {
        printf("f(x) = (%.2lf) * sin(%.2lf * x + %.2lf)\n\
        onde C = %.2lf, w = %.2lf, phi = %.2lf\n", param[0], param[1], param[3], param[0], param[1], param[2] );
        return seno;
    }
    else if (numeroMagico == COSSENO) {
        printf("f(x) = (%.2lf) * cos(%.2lf * x + %.2lf)\n\
        onde C = %.2lf, w = %.2lf, phi = %.2lf\n", param[0], param[1], param[3], param[0], param[1], param[2] );
        return cosseno;
    }
    else if (numeroMagico == TANGENTE) {
        printf("f(x) = (%.2lf) * tan(%.2lf * x + %.2lf)\n\
        onde C = %.2lf, w = %.2lf, phi = %.2lf\n", param[0], param[1], param[3], param[0], param[1], param[2] );
        return tangente;
    }
    else if (numeroMagico == SECANTE) {
        printf("f(x) = (%.2lf) * sec(%.2lf * x + %.2lf)\n\
        onde C = %.2lf, w = %.2lf, phi = %.2lf\n", param[0], param[1], param[3], param[0], param[1], param[2] );
        return secante;
    }
    else if (numeroMagico == COSSECANTE) {
        printf("f(x) = (%.2lf) * cossec(%.2lf * x + %.2lf)\n\
        onde C = %.2lf, w = %.2lf, phi = %.2lf\n", param[0], param[1], param[3], param[0], param[1], param[2] );
        return cossecante;
    }
    else if (numeroMagico == COTANGENTE) {
        printf("f(x) = (%.2lf) * cotan(%.2lf * x + %.2lf)\n\
        onde C = %.2lf, w = %.2lf, phi = %.2lf\n", param[0], param[1], param[3], param[0], param[1], param[2] );
        return cotangente;
    }
    else if (numeroMagico == ARCOSENO)
        return aseno;
    else if (numeroMagico == ARCOCOSSENO)
        return acosseno;
    else if (numeroMagico == ARCOTANGENTE)
        return atangente;
    else if (numeroMagico == ARCOSECANTE)
        return asecante;
    else if (numeroMagico == ARCOCOSSECANTE)
        return acossecante;
    else if (numeroMagico == ARCOCOTANGENTE)
        return acotangente;
    else if (numeroMagico == EXPONENCIAL) {
        printf("f(x) = (%.2lf)e^(%.2lf * x + %.2lf)\n", param[0], param[1], param[2] );
        return exponencial;
    }
    else if (numeroMagico == LN) {
        printf("f(x) = (%.2lf)ln(%.2lf * x + %.2lf)\n", param[0], param[1], param[2] );
        return ln;
    }
    else if (numeroMagico == PI) {
        printf("f(x) = sqrt(4 - x^2)\n");
        return pi;
    }
    else if (numeroMagico == EXPNX2) {
        printf("f(x) = e^(-x^2)\n");
        return expnx2;
    }
    else if (numeroMagico == CNLTANEXP ) {
        printf("f(x) = 5x^3e^x + 2x^2tan(x)\n");
        return comnlinearExpTan;
    }
    
    return NULL;
}
