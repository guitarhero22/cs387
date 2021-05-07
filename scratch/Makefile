INCLUDE_DIR=-I./include
dependencies=src/BufferManager/FileIO.cpp
MAIN=src/master.cpp
FLAGS=--std=c++17 -O3 -Wno-unused-result

master: master.o FileIO.o
	g++ -o $@ $(FLAGS) $(INCLUDE_DIR) $^

master.o: $(MAIN)
	g++ -c -o $@ $(FLAGS) $(INCLUDE_DIR) $<

FileIO.o: $(dependencies)
	g++ -c -o $@ $(FLAGS) $(INCLUDE_DIR) $<

clean:
	rm *.o 
	rm *.log
	rm master 
