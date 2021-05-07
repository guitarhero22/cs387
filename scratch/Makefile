INCLUDE_DIR=./include
BUILD_DIR=./build
BUILD_DEBUG_DIR=./build-debug
SRC_DIR=./src
OBJS=$(BUILD_DIR)/main.o $(BUILD_DIR)/master.o $(BUILD_DIR)/FileIO.o
DEBUG_OBJS=$(BUILD_DEBUG_DIR)/main.o $(BUILD_DEBUG_DIR)/master.o $(BUILD_DEBUG_DIR)/FileIO.o
FLAGS= -std=c++17 -O3 -Wno-unused-result -lpthread
DEBUG_FLAGS = -g -D _DEBUG -std=c++17 -Wno-unused-result -lpthread

main: $(BUILD_DIR) main.o master.o FileIO.o fs
	g++ -o $(BUILD_DIR)/$@  $(OBJS) $(FLAGS)
	cp $(BUILD_DIR)/$@ fs/$@

debug: $(BUILD_DEBUG_DIR) $(DEBUG_OBJS) fs
	g++ -o $(BUILD_DEBUG_DIR)/main $(DEBUG_OBJS) $(DEBUG_FLAGS)
	cp $(BUILD_DEBUG_DIR)/main fs/main

fs:	$(SRC_DIR)/fs.cpp
	rm -rf fs
	mkdir fs
	# 
	# *** Please wait for 10 seconds...
	# 
	g++ -o ./fs/$@ $(FLAGS) -I$(INCLUDE_DIR) $<
	cd fs && ./fs
	# 
	# *** Filesystem Made. Check in fs.
	#

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
	rm -f fs/fs

clean-fs:
	rm -rf fs
