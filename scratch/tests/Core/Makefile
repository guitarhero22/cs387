INCLUDEDIR=-I../../include -I./
FLAGS=--std=c++17
test: test.cpp ../../include/Core/Merge.hpp ../../include/Core/Read.hpp ../../src/BufferManager/FileIO.cpp ../../include/BufferManager/FileIO.hpp
	g++ -g -o test $(INCLUDEDIR) $(FLAGS) ../../src/BufferManager/FileIO.cpp test.cpp
clean:
	rm test testbt1.dump testbt2.dump merged.dump