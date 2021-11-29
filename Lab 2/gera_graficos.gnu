# Script para gerar os gráficos no Gnuplot

set term postscript eps color blacktext "Helvetica" 24
set output "m500.eps"
set title "Matriz com dimensao 500"
set ylabel "Tempo (em seg)"
set xlabel "Numero de threads"
set style fill solid

set boxwidth 0.05

p [-1:5] [0:3] 'dataset-m500.dat' using ($2-0.05):1 w boxes lc 7 title "Sequencial", \
	        'dataset-m500.dat'using 4:3:xtic(4) w boxes lc 6 title "Concorrente"

set term postscript eps color blacktext "Helvetica" 24
set output "m1000.eps"
set title "Matriz com dimensao 1000"
set ylabel "Tempo (em seg)"
set xlabel "Numero de threads"
set style fill solid

set boxwidth 0.05

p [-1:5] [0:20] 'dataset-m1000.dat' using ($2-0.05):1 w boxes lc 7 title "Sequencial", \
	        'dataset-m1000.dat' using 4:3:xtic(4) w boxes lc 6 title "Concorrente"

set term postscript eps color blacktext "Helvetica" 24
set output "m2000.eps"
set title "Matriz com dimensao 2000"
set ylabel "Tempo (em seg)"
set xlabel "Numero de threads"
set style fill solid

set boxwidth 0.05

p [-1:5] [0:100] 'dataset-m2000.dat' using ($2-0.05):1 w boxes lc 7 title "Sequencial", \
	        'dataset-m2000.dat' using 4:3:xtic(4) w boxes lc 6 title "Concorrente"
		

