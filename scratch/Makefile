INCLUDE_DIR=-I./include
dependencies=src/BufferManager/FileIO.cpp
MAIN=src/master.cpp
FLAGS=--std=c++17 -Wall -Wno-unused-result -Wno-unused-variable -O3

final: $(MAIN) $(dependencies)
	g++ -o $(@F:.cpp=) $(FLAGS) $(INCLUDE_DIR) -O3 $^