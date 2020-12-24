#!/bin/bash

for ((t=0; t<=100; t++));
    do 
        ./rdv_q4 > tmp 
        i=0
            filename='tmp'
            while read -r line
            do
            id=$(cut -c-1 <<< "$line") # on récupère la première lettre 'a' ou 'b'

            # si on est dans les 10 premiers et que c'est 'a' alors on incrémente
            if [ "$id" = 'a' ] && [ $i -lt 10 ]; then 
                # echo $id
                i=$((i+1))
            fi
            # si on est dans les 10 premiers et que c'est 'b' alors on incrémente
            if [ "$id" = 'b' ] && [ $i -lt 20 ]; then 
                # echo $id
                i=$((i+1))
            fi
            
            done < "$filename"

            # le total doit être == à 20
            if [ $i -ne 20 ]; then 
                echo "l'ordre de 10 'a' en premier suivi de 10 'b' n'est pas respecté"
                exit
            fi
    done
rm -rf tmp