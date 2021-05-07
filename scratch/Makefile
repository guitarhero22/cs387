INCLUDE_DIR=-I./include
dependencies=src/BufferManager/FileIO.cpp
MAIN=src/master.cpp
FLAGS=--std=c++17 -O3

master: master.o FileIO.o
	g++ -c -o $@ $(FLAGS) $(INCLUDE_DIR) $^ 2> errlinking.log

master.o: $(MAIN)
	g++ -c -o $@ $(FLAGS) $(INCLUDE_DIR) $< 2> errmaster.log

FileIO.o: $(dependencies)
	g++ -c -o $@ $(FLAGS) $(INCLUDE_DIR) $< 2> errFileIO.log

clean:
	rm *.o 
	rm *.log
	rm master 
