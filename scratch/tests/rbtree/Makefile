INCLUDEDIR=-I../../include -I./
FLAGS=--std=c++17
test: test.cpp
	g++ -D _DEBUG -o test $(INCLUDEDIR) $(FLAGS) test.cpp
	./test
	hexdump -c test.dump
clean:
	rm test