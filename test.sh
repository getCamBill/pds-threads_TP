#!/bin/bash
# Exemple de script shell qui mesure le temps d'éxécution de
# "./compteur-gc" avec le genome et le nb de thread passé en param

FINAL="final.dat"
TEMP=temp
rm -rf $FINAL
for ((n=2; n<=9; n++)); 
    do
    genSize=$((10**$n))
      for ((t=0; t<=5; t++));
        do 
          nbTh=$((2**$t))
          ./aleazard $genSize > $TEMP # si ds mémoire directement -> core dumped
          echo -n "$genSize $nbTh " >> $FINAL
          echo "$(./compteur-gc temp $nbTh)" >> $FINAL   
        done
    done
sed -i '1s/^/# genomeSize nbThread time\n/' $FINAL
# on affiche le graphique
gnuplot run.gnu
  # ni vu ni connu on supp les fichiers ...
rm -rf $TEMP
rm -rf $FINAL