INCLUDE_DIR=-I./include
dependencies=src/BufferManager/FileIO.cpp
MASTER=src/master.cpp
MAIN=src/main.cpp
FLAGS=--std=c++17 -O3 -Wno-unused-result -lpthread

main: main.o master.o FileIO.o
	g++ -o $@  $^ $(FLAGS)

main.o: $(MAIN)
	g++ -c -o $@ $(FLAGS) $(INCLUDE_DIR) $<

master.o: $(MASTER)
	g++ -c -o $@ $(FLAGS) $(INCLUDE_DIR) $<

FileIO.o: $(dependencies)
	g++ -c -o $@ $(FLAGS) $(INCLUDE_DIR) $<

clean:
	rm *.o 
	rm main
