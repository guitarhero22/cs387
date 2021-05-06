INCLUDEDIR=-I../../include -I./
FLAGS=--std=c++17
test: test.cpp ../../include/Core/Merge.hpp
	g++ -g -D _DEBUG -o test $(INCLUDEDIR) $(FLAGS) test.cpp
clean:
	rm test testbt1.dump testbt2.dump merged.dump