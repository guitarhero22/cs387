INCLUDEDIR=-I../../include -I./
FLAGS=--std=c++17
test: rbtree.cpp test.cpp
	g++ -o test $(INCLUDEDIR) $(FLAGS) test.cpp
	./test
clean:
	rm test