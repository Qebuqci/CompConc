#ifndef __FEXTRAS_H
#define __FEXTRAS_H

/**Calcula a função sqrt(4 - x^2), que dá pi quando integrada em [0,2] **/
long double pi(double x, double *param);

/** Calcula a exponencial negativa de e^{-x^2} **/
long double expnx2(double x, double *param);

/** Calcula a combinação não linear 5x^3e^x + 2x^2tan(x)**/
long double comnlinearExpTan(double x, double *param);


#endif
