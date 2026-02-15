#!/bin/bash

KS=8

mkdir -p out/img

SET=1
for ((k=1; k<${KS}+1; k++)) do
  ./bin/main bin ../data/set-${SET}/1.bmp out/img/bin-${SET}-1-${k}.bmp ${k}
  ./bin/main bin out/img/enc-src-1-1-${k}.bmp out/img/bin-enc-src-${SET}-1-${k}.bmp ${k}
done
for ((k=1; k<${KS}+1; k++)) do
  ./bin/main bin ../data/set-${SET}/2.bmp out/img/bin-${SET}-2-${k}.bmp ${k}
done
for ((k=1; k<${KS}+1; k++)) do
  ./bin/main bin ../data/set-${SET}/4.bmp out/img/bin-${SET}-4-${k}.bmp ${k}
done
for ((k=1; k<${KS}+1; k++)) do
  ./bin/main bin ../data/set-${SET}/5.bmp out/img/bin-${SET}-5-${k}.bmp ${k}
done
for ((k=1; k<${KS}+1; k++)) do
  ./bin/main bin ../data/set-${SET}/6.bmp out/img/bin-${SET}-6-${k}.bmp ${k}
done

SET=2
for ((k=1; k<${KS}+1; k++)) do
  ./bin/main bin ../data/set-${SET}/1.bmp out/img/bin-${SET}-1-${k}.bmp ${k}
  ./bin/main bin out/img/enc-src-1-1-${k}.bmp out/img/bin-enc-src-${SET}-1-${k}.bmp ${k}
done
for ((k=1; k<${KS}+1; k++)) do
  ./bin/main bin ../data/set-${SET}/2.bmp out/img/bin-${SET}-2-${k}.bmp ${k}
done
for ((k=1; k<${KS}+1; k++)) do
  ./bin/main bin ../data/set-${SET}/3.bmp out/img/bin-${SET}-3-${k}.bmp ${k}
done
for ((k=1; k<${KS}+1; k++)) do
  ./bin/main bin ../data/set-${SET}/4.bmp out/img/bin-${SET}-4-${k}.bmp ${k}
done
for ((k=1; k<${KS}+1; k++)) do
  ./bin/main bin ../data/set-${SET}/5.bmp out/img/bin-${SET}-5-${k}.bmp ${k}
done

SET=3
for ((k=1; k<${KS}+1; k++)) do
  ./bin/main bin ../data/set-${SET}/1.bmp out/img/bin-${SET}-1-${k}.bmp ${k}
  ./bin/main bin out/img/enc-src-1-1-${k}.bmp out/img/bin-enc-src-${SET}-1-${k}.bmp ${k}
done
for ((k=1; k<${KS}+1; k++)) do
  ./bin/main bin ../data/set-${SET}/4.bmp out/img/bin-${SET}-4-${k}.bmp ${k}
done
for ((k=1; k<${KS}+1; k++)) do
  ./bin/main bin ../data/set-${SET}/8.bmp out/img/bin-${SET}-8-${k}.bmp ${k}
done
for ((k=1; k<${KS}+1; k++)) do
  ./bin/main bin ../data/set-${SET}/46.bmp out/img/bin-${SET}-46-${k}.bmp ${k}
done
for ((k=1; k<${KS}+1; k++)) do
  ./bin/main bin ../data/set-${SET}/95.bmp out/img/bin-${SET}-95-${k}.bmp ${k}
done


