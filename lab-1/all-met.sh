#!/bin/bash

SETS=3
KS=8
OUT="out/metrix.txt"

mkdir -p out

echo "[metrics]" > ${OUT}

for ((i=1; i<${SETS}+1; i++)); do
  echo -e "\n[data/set-${i}/1.bmp]" >> ${OUT}
  for ((j=1; j<${KS}+1; j++)); do
    echo "[out/img/enc-src-${i}-1-${j}.bmp]" >> ${OUT}
    python3 src/metrics.py data/set-${i}/1.bmp out/img/enc-src-${i}-1-${j}.bmp >> ${OUT}
    echo >> ${OUT}
  done
  echo "---" >> ${OUT}
done
