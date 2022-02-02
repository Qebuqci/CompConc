#ifndef __FELEMENTAR__H
#define __FELEMENTAR__H

// Números mágicos de cada função elementar
#define POLINOMIO 1
#define SENO 2
#define COSSENO 3
#define TANGENTE 4
#define SECANTE 5
#define COSSECANTE 6
#define COTANGENTE 7
#define ARCOSENO 8
#define ARCOCOSSENO 9
#define ARCOTANGENTE 10
#define ARCOSECANTE 11
#define ARCOCOSSECANTE 12
#define ARCOCOTANGENTE 13
#define EXPONENCIAL 14
#define LN 15

// Números mágicos das funções extra - começando a partir do 20
#define PI 20
#define EXPNX2 21
#define CNLTANEXP 22

/** Procedimento Multiplexador (seletor) de uma função elementar
 * Entrada: Número mágico indicando a função elementar e seu vetor de parâmetros
 * vindo dos arquivos. 
 * Saída: Um ponteiro para função elementar
 **/
long double (*funcao(int numeroMagico, double *param))(double x,  double *param);

/** Polinômio de x de grau n
 * Entrada: um nº real x e um vetor de parâmetros
 * Saída: Polinômio de grau n (informado no vetor) calculado em x
 * Parâmetros do polinômio:
 *  1º parâmetro - indicativo do grau
 *  2º parâmetro - termo constante, se houver
 *  3º parâmetro - termo linear, se houver
 *  4º parâmetro - termo quadrático, se houver
 * ... Assim por diante
 * Exemplos: polinomio(x, [3,0,0,2,4]) = 4x^3 + 2x^3 (grau 3)
 *           polinomio(x, [1,1,3]) = 3x + 1 (grau 1)
**/
long double polinomio(double x, double *param);

/** Funções Trigonométricas. Todas as funções Trigonométricas recebem a 
 * mesma configurações de parâmetros
 * Entrada: nº real x e um vetor de parametros, que são coeficientes da 
 * forma:
 *  C - constante que multiplica a função trigonométrica
 *  w - fator omega que indica a velocidade angular
 *  phi - angulo de defasagem phi
 * Saída: Calculo da função trigonométrica com aplicação desses coeficientes
**/

/** Função Seno
  * Saída: C.sin(wx + phi)
**/
long double seno(double x, double *param);

/** Função Cosseno
  * Saída: C.cos(wx + phi)
**/
long double cosseno(double x, double *param);

/** Função Tangente
 * Saída: C.tan(wx + phi)
**/
long double tangente(double x, double *param);

/** Função Secante
  * Saída: C.sec(wx + phi)
**/
long double secante(double x, double *param);

/** Função Cossecante
  * Saída: C.cossec(wx + phi)
**/
long double cossecante(double x, double *param);

/** Função Cotangente
  * Saída: C.cotg(wx + phi)
**/
long double cotangente(double x, double *param);

/** Função Arco Sino
 * Saída: C.asin(wx + phi)
**/
long double aseno(double x, double *param);

/** Função Arco Cosseno
 * Saída: C.asin(wx + phi)
**/
long double acosseno(double x, double *param);

/** Função Arco Tangente
 * Saída: C.atan(wx + phi)
**/
long double atangente(double x, double *param);

/** Função Arco Secante
 * Saída: C.asec(wx + phi)
**/
long double asecante(double x, double *param);

/** Função Arco Cossecante
 * Saída: C.asec(wx + phi)
**/
long double acossecante(double x, double *param);

/** Função Arco Cotangente
 * Saída: C.asec(wx + phi)
**/
long double acotangente(double x, double *param);

/** Exponencial. A exponencial de base \epsilon têm três parâmetros
 * de configuração. C - coeficiente que multiplica, \omega que multiplica x
 * e k como fator constante.
 * Saída: C.e^(wx + k)
**/
long double exponencial(double x, double *param);

/** Logarítmo natural têm três parâmetros.
 * parâmetro C que multiplica log. w que multiplica x e fator constante
 * k
 * Saída: C.log(w x + k)
**/ 
long double ln(double x, double *param);


#endif
