#!/bin/bash
# Script feito para automatizar os casos de testes do programa de integração
# concorrente.

# step = 0.001 = 10^3 particionamentos
# step = 0.000001 = 10^6 particionamentos
# step = 0.0000001 = 10^7 particionamentos
# step = 0.00000001 = 10^9 particionamentos

STEP_PART=(0.001 0.000001 0.0000001 0.00000001) 
NTHREADS=(1 2 4 8)
MODO_INTEGRACAO=3 

# Gera uma tabela com informações de tempo de execução do programa para o MODO_INTEGRACAO escolhido
echo -e "# TABELA #\n"
for iStep in $(seq 0 1 3); do        # Varre o vetor de steps (particionamentos) com 4 itens: 80 execuções
    for iThread in $(seq 0 1 3); do  # Varre o vetor NTHREADS, com 4 itens: 20 execuções
        for exec in $(seq 1 1 5); do # 5 execuções por thread
            ./integralc $MODO_INTEGRACAO ${NTHREADS[iThread]} 3 -3 3 ${STEP_PART[$iStep]} cos # arquivo cos criado com linha 1 1 0
        done;
    done;
done
