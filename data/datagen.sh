echo "Generating data for single threaded..."
for i in 100 500 1000 5000 10000 50000 100000
do
mkdir "$i"
echo "making for $i"
python3 datagen.py 1 $i
mv *.sql "$i"
mv *.scratch "$i"
done

echo "Generating data for 2 threads..."
for i in 100 500 1000 5000 10000 50000 100000
do
mkdir "$i"_2
echo "making for $i"
python3 datagen.py 2 $((i / 2))
mv *.sql "$i"_2
mv *.scratch "$i"_2
done