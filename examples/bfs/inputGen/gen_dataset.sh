#!/bin/bash

./graph_gen 1024 1k
./graph_gen 2048 2k
./graph_gen 4096 4k
./graph_gen 8192 8k
./graph_gen 16384 16k
./graph_gen 32768 32k
./graph_gen 65536 64k
./graph_gen 131072 128k
./graph_gen 261444 256k
./graph_gen 524288 512k
./graph_gen 1048576 1M
./graph_gen 2097152 2M
./graph_gen 4194304 4M
./graph_gen 8388608 8M
./graph_gen 16777216 16M


#!/bin/bash

EXEC=dsad

./$EXEC 2 graph1k.txt
./$EXEC 2 graph2k.txt
./$EXEC 2 graph4k.txt
./$EXEC 2 graph8k.txt
./$EXEC 2 graph16k.txt
./$EXEC 2 graph32k.txt
./$EXEC 2 graph64k.txt
./$EXEC 2 graph128k.txt
./$EXEC 2 graph256k.txt
./$EXEC 2 graph512k.txt

echo "1M\n"
./$EXEC 2 graph1M.txt

echo "2M\n"
./$EXEC 2 graph2M.txt

echo "4M\n"
./$EXEC 2 graph4M.txt

echo "8M\n"
./$EXEC 2 graph8M.txt

echo "16M\n"
./$EXEC 2 graph16M.txt




