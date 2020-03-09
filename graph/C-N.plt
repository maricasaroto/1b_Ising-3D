set term tikz size 6.0cm, 5.0cm fontscale 0.5 dl 0.5 fulldoc
set output "C-N-tex.tex"
set tics nomirror
set tics scale 0.5
set decimalsign ","

set grid
set title "Modelo de Ising"
set xlabel "Temperatura ($T$)"
set ylabel "Calor específico ($C/N$)"
set key at graph 0.30, 0.97

plot "../dat/output_2.dat" u 1:4 w lp pt 7 lw 2 ps 0.3 lc 6 t'$L=2$', "../dat/output_4.dat" u 1:4 w lp pt 7 lw 2 ps 0.3 t'L=4',"../dat/output_8.dat" u 1:4 w lp pt 7 lw 2 ps 0.3 t'$L=8$',"../dat/output_16.dat" u 1:4 w lp pt 7 lw 2 ps 0.3 t'$L=16$'


unset output
