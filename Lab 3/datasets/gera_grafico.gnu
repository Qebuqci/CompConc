#------------------------------------------------
# Script para gerar o gráfico das acelerações
#------------------------------------------------

# Configurações para set o arquivo de saída do gráfico
set term postscript eps color blacktext "Helvetica" 24
set output "gr_lab3.eps"

# Configurações de título e labels
set title  "Aceleracao (ganho) da Concorrencia"
set ylabel "Tempo (em segundos)"
set xlabel "Dimensao dos vetores" offset 0,-1.5

# Configurações de legenda
set key bottom right outside
set key title "Threads"

# Configurações de domínio e imagem do gráfico
unset xtics
set style fill solid
set boxwidth 0.05

# Configurações de rótulos
set label "10^5" at 1,-0.2
set label "10^7" at 2,-0.2
set label "10^8" at 3,-0.2

# Plot dos dados
p [0:4] [0:4]   'dataset-m10_5-1.dat' using 3:6:xtic(5) w boxes lc 7 title "1", \
	        'dataset-m10_5-2.dat' using ($3+0.1):6:xtic(5) w boxes lc 4 title "2", \
		'dataset-m10_5-4.dat' using ($3+2*0.1):6:xtic(5) w boxes lc 2 title "4", \
		'dataset-m10_7-1.dat' using ($3+1):6:xtic(5) w boxes lc 7 notitle, \
		'dataset-m10_7-2.dat' using ($3+1.1):6:xtic(5) w boxes lc 4 notitle, \
		'dataset-m10_7-4.dat' using ($3+1+2*0.1):6:xtic(5) w boxes lc 2 notitle, \
		'dataset-m10_8-1.dat' using ($3+2):6:xtic(5) w boxes lc 7 notitle, \
		'dataset-m10_8-2.dat' using ($3+2.1):6:xtic(5) w boxes lc 4 notitle, \
		'dataset-m10_8-4.dat' using ($3+2.2):6:xtic(5) w boxes lc 2 notitle