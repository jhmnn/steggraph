#!/bin/bash

mkdir -p out/

SETS=3
IMGS=1
OUT="out/metrix.txt"
PSNR_OUT="out/psnr.csv"

echo "dataset,psnr" > ${PSNR_OUT}

echo "[set-1]"
for ((i=1; i<${IMGS}+1; i++)); do
  python ./src/psnr.py ../data/set-1/${i}.bmp ./out/img/emb-adaptive-1-${i}.bmp boss ${PSNR_OUT}
done

echo "[set-2]"
for ((i=1; i<${IMGS}+1; i++)); do
  python ./src/psnr.py ../data/set-2/${i}.bmp ./out/img/emb-adaptive-2-${i}.bmp medicals ${PSNR_OUT}
done

echo "[set-3]"
for ((i=1; i<${IMGS}+1; i++)); do
  python ./src/psnr.py ../data/set-3/${i}.bmp ./out/img/emb-adaptive-3-${i}.bmp other ${PSNR_OUT}
done

python ./src/int.py

echo "[metrics]" > ${OUT}

for ((i=1; i<${SETS}+1; i++)); do
  echo -e "\n[../data/set-${i}/1.bmp]" >> ${OUT}
  echo >> ${OUT}
  echo "[out/img/emb-lsb-${i}-1.bmp]" >> ${OUT}
  python3 src/metrics.py ../data/set-${i}/1.bmp out/img/emb-lsb-${i}-1.bmp >> ${OUT}
  echo >> ${OUT}
  echo "[out/img/emb-adaptive-${i}-1.bmp]" >> ${OUT}
  python3 src/metrics.py ../data/set-${i}/1.bmp out/img/emb-adaptive-${i}-1.bmp >> ${OUT}
  echo >> ${OUT}
  echo "---" >> ${OUT}
done
