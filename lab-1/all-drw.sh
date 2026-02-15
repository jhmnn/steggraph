#!/bin/bash

SETS=3
KS=8

mkdir -p out/charts

for ((i=1; i<${SETS}+1; i++)); do
  for ((j=1; j<${KS}+1; j++)); do
    python3 src/draw.py ../data/set-${i}/1.bmp out/img/enc-src-${i}-1-${j}.bmp out/charts/${i}-1-${j}.png ${j}
  done
done
