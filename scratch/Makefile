INCLUDE_DIR=./include
BUILD_DIR=./build
BUILD_DEBUG_DIR=./build-debug
SRC_DIR=./src
OBJS=$(BUILD_DIR)/main.o $(BUILD_DIR)/master.o $(BUILD_DIR)/FileIO.o
DEBUG_OBJS=$(BUILD_DEBUG_DIR)/main.o $(BUILD_DEBUG_DIR)/master.o $(BUILD_DEBUG_DIR)/FileIO.o
FLAGS= -std=c++17 -O3 -Wno-unused-result -lpthread
DEBUG_FLAGS = -D _DEBUG -std=c++17 -Wno-unused-result -lpthread

main: $(BUILD_DIR) main.o master.o FileIO.o
	g++ -o $(BUILD_DIR)/$@  $(OBJS) $(FLAGS)

debug: $(BUILD_DEBUG_DIR) $(DEBUG_OBJS)
	g++ -o $(BUILD_DEBUG_DIR)/main $(DEBUG_OBJS) $(FLAGS)

main.o: $(SRC_DIR)/main.cpp
	g++ -c -o $(BUILD_DIR)/$@ $(FLAGS) -I$(INCLUDE_DIR) $<

master.o: $(SRC_DIR)/master.cpp
	g++ -c -o $(BUILD_DIR)/$@ $(FLAGS) -I$(INCLUDE_DIR) $<

FileIO.o: $(SRC_DIR)/BufferManager/FileIO.cpp
	g++ -c -o $(BUILD_DIR)/$@ $(FLAGS) -I$(INCLUDE_DIR) $<

$(BUILD_DEBUG_DIR)/main.o: $(SRC_DIR)/main.cpp
	g++ -c -o $@ $(DEBUG_FLAGS) -I$(INCLUDE_DIR) $<

$(BUILD_DEBUG_DIR)/master.o: $(SRC_DIR)/master.cpp
	g++ -c -o $@ $(DEBUG_FLAGS) -I$(INCLUDE_DIR) $<

$(BUILD_DEBUG_DIR)/FileIO.o: $(SRC_DIR)/BufferManager/FileIO.cpp
	g++ -c -o $@ $(DEBUG_FLAGS) -I$(INCLUDE_DIR) $<

./build:
	mkdir build

./build-debug:
	mkdir build-debug

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(BUILD_DEBUG_DIR)
