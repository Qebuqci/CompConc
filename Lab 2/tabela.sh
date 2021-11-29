#/bin/bash

# Script em bash para :
# 1. Automatizar os casos de execução de multiplicação de matrizes
# 2. Gerar conjunto de dados para plotar os resultados de tempo

# Forma de uso: ./tabela.sh > tabela.txt

# Observações:
# -----------------------------------------------------------------------------------
# O script demora para entregar a saída, pois são 90 multiplicações de matrizes 
# (execuções) no total
# 5 execuções com 1 thread para matrizes de ordem 500, 1000, 2000 = 15 execuções
# 5 execuções com 2 threads para mesma sequencia de ordens de matrizes = 15 execuções
# 5 execuções com 4 threads para essa mesma sequência  = 15 execuções
# Dando um total de 45 execuções concorrentes

# Para cada execução concorrente, uma sequencial também é calculada. 
# Assim, como são 45 concorrentes, há mais 45 sequenciais. Totalizando 90 execuções

# Logo, por serem muitas execuções, o script demora para dar o resultado final
# Aqui demorou em torno de 20~40 minutos.
# ------------------------------------------------------------------------------------

# Se o script for executado exatamente como proposto, redirecionando a saída para
# um arquivo ASCII (tabela.txt), então ao final da execução, haverão quatro ASCII files.
# A tabela.txt e mais três datasets, que são usados para plotar os resultados
# pelo script do gnuplot

DIM_MATRIZ=(500 1000 2000)
NTHREADS=(1 2 4)
FILES_DATASET=("dataset-m500.dat" "dataset-m1000.dat" "dataset-m2000.dat")

# Gera tabela
echo -e "# TABELA #\n"
for iMatriz in $(seq 0 1 2); do
    echo -e "# MATRIZ COM DIMENSÃO ${DIM_MATRIZ[$iMatriz]}"
    echo -e "# Tempo Sequencial (em s)\t#Thread Principal\t#Tempo Concorrente (em s)\t#nº Threads\t#Ganho"
    for iThread in $(seq 0 1 2); do
        for exec in $(seq 1 1 5); do
            ./lab2 ${DIM_MATRIZ[$iMatriz]} ${NTHREADS[iThread]}
        done;
    done;
done

# Gera dataset a partir da tabela.txt
# Se o resultado do ./tabela.sh for redirecionado para outro ARQUIVO.
# ./tabela.sh > ARQUIVO
# ALTERAR ESSA PARTE DO SCRIPT
ARQUIVO="tabela.txt"
sed -n "3,19p"  $ARQUIVO > ${FILES_DATASET[0]}
sed -n "20,36p" $ARQUIVO > ${FILES_DATASET[1]}
sed -n "37,53p" $ARQUIVO > ${FILES_DATASET[2]}
