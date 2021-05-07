INCLUDEDIR=-I../../include -I./
FLAGS=--std=c++17
genfs: test.cpp
	g++ -D GENFS -o genfs $(INCLUDEDIR) $(FLAGS) test.cpp
	./genfs
	mv bloomdump* ../../fs
test: test.cpp
	g++ -D _DEBUG -D TEST -o test $(INCLUDEDIR) $(FLAGS) test.cpp
	./test
clean:
	rm test genfs *.dump bllomdump*