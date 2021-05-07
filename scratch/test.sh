#! /bin/bash

echo "testing write ..."
for i in 100 500 1000 5000 10000 50000 100000
do
make clean >/dev/null
rm -rf fs >/dev/null
make >/dev/null
cd fs
echo "Checking for $i"
for j in `ls ../../data/$i | grep write | grep scratch`
do
echo "time $j..."
time ./main ../../data/$i/$j 
done
cd ..
done

echo "testing write ..."
for i in 100 500 1000 5000 10000 50000 100000
do
make clean >/dev/null
rm -rf fs >/dev/null
make >/dev/null
cd fs
echo "Checking for $i multi threaded..."
rm -rf ../../data/"$i"_2/*.out >/dev/null
echo `ls ../../data/"$i"_2 | grep write | grep scratch`
time ./main ../../data/"$i"_2/writebatch0.scratch ../../data/"$i"_2/writebatch1.scratch
cd ..
done