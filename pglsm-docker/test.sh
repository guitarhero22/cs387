#! /bin/bash

echo "testing write ..."
for i in 100 500 1000 5000 10000 50000 100000
do
make
echo "Checking for $i"
for j in `ls ../data/$i | grep write | grep sql`
do
echo "time $j..."
time  psql -q -f ../data/$i/$j 
done
done
