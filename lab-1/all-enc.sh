#!/bin/bash

SETS=3
KS=8

for ((i=1; i<${SETS}+1; i++)); do
  for ((j=1; j<${KS}+1; j++)) do
    ./bin/main enc ../data/set-${i}/1.bmp ../data/msg.txt out/img/enc-src-${i}-1-${j}.bmp ${j}
    ./bin/main enc out/img/bin-${i}-1-${j}.bmp ../data/msg.txt out/img/enc-bin-${i}-1-${j}.bmp ${j}
  done
done
