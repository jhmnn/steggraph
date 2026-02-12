#!/bin/bash

SETS=3
IMGS=5
KS=8

mkdir -p out/img

for ((i=1; i<${SETS}+1; i++)); do
  for ((j=1; j<${IMGS}+1; j++)) do
    for ((k=1; k<${KS}+1; k++)) do
      ./bin/main bin data/set-${i}/${j}.bmp out/img/bin-${i}-${j}-${k}.bmp ${k}
    done
  done
done
