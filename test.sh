#/bin/bash

./server &

sleep 1

for i in $(seq 0 10000)
do
    ./client &
done
