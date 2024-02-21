#/bin/bash

for i in $(seq 0 10000)
do
    ./client &
done
