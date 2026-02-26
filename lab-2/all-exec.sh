#!/bin/bash

mkdir -p out/img/

SETS=3

echo

for ((i=1; i<${SETS}+1; i++)); do
  ./bin/main emb lsb ../data/set-${i}/1.bmp ../data/img/wm.bmp out/img/emb-lsb-${i}-1.bmp
  ./bin/main emb adaptive ../data/set-${i}/1.bmp ../data/img/wm.bmp out/img/emb-adaptive-${i}-1.bmp
  echo
  ./bin/main ext lsb out/img/emb-lsb-${i}-1.bmp out/img/ext-lsb-${i}-1.bmp 32761
  ./bin/main ext adaptive out/img/emb-adaptive-${i}-1.bmp out/img/ext-adaptive-${i}-1.bmp 32761
  echo
  ../lab-1/bin/main bin out/img/emb-lsb-${i}-1.bmp out/img/bin-emb-lsb-${i}-1.bmp 1
  ../lab-1/bin/main bin out/img/emb-adaptive-${i}-1.bmp out/img/bin-emb-adaptive-${i}-1.bmp 1
done
