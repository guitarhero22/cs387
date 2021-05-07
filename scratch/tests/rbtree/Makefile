INCLUDEDIR=-I../../include -I./
FLAGS=--std=c++17
test: test.cpp
	g++ -D _DEBUG -o test -D TEST $(INCLUDEDIR) $(FLAGS) test.cpp
	./test
	hexdump -c test.dump
genfs: test.cpp
	g++ -o genfs -D GENFS $(INCLUDEDIR) $(FLAGS) test.cpp
	./genfs
	mv file* ../../fs
clean: 
	rm test genfs file* *.dump