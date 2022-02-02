#include <math.h>
#include "fextras.h"

long double pi(double x, double *param)
{
    return sqrt(4 - pow(x, 2));
}

/** Calcula a exponencial negativa de e^{-x^2} **/
long double expnx2(double x, double *param)
{
    return exp(-1*pow(x, 2));    
}

/** Calcula a combinação não linear 5x^3e^x + 2x^2tan(x)**/
long double comnlinearExpTan(double x, double *param)
{
    return 5*pow(x,3)*exp(x)+2*pow(x,2)*tan(x);
}
