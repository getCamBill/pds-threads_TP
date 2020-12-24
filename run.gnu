set title "Temps de calcul du taux de GC en fonction de sa taille et du nombre de thread alloué"
set logscale x
set dgrid3d 30,30
set pm3d
splot 'final.dat' using 1:2:3 with lines
pause -1  "Hit return to continue"
set term pngcairo
set output "plot.png"
replot
set output