#ifndef __BUFMAN_H__
#define __BUFMAN_H__

#include "files.hpp"
#include "utils.hpp"

#include <string>
#include <array>
#include <list>
#include <unordered_map>

#define MAX_BLK 1024

struct Blk
{
	/// @brief Block of memory
	byte block[BLK_SIZE];
};

struct BlkMetaData
{
	/// @brief Filehandle of this block
	int fileHandle;

	/// @brief Offset of block in file
	off_t offset;
};

class FileIO
{
public:
	FileIO();

	/**
	 * @brief Opens a file
	 * @param filePath Path of file to read
	 * @return The fileHandle of the file is returned
	 */
	static int openFile(const std::string& filePath);

	/**
	 * @brief Closes the file and flushes it to disk
	 * @param fileHandle fileHandle to close
	 */
	static void closeFile(int fileHandle);

	/**
	 * @brief Forces the write of the file to disk
	 * @param fileHandle fileHandle to flush to disk
	 */
	static void flushFile(int fileHandle);

	/**
	 * @brief Reads from fileHandle
	 * @param ptr Pointer to which fread will put the read memory (must have at least size*count bytes allocated)
	 * @param size Size, in bytes, of each element to be read. size_t is an unsigned integral type.
	 * @param count Number of elements, each one with a size of size bytes. size_t is an unsigned integral type.
	 * @param offset File offset to read from
	 * @param fileHandle fileHandle to read from
	 * @return The total number of elements successfully read is returned.
	 */
	static size_t fread(void* ptr, size_t size, size_t count, off_t offset, int fileHandle);

	/**
	 * @brief Returns file size in bytes
	 * @param fileHandle fileHandle of file
	 * @return filesize in bytes
	 */
	static off_t fsize(int fileHandle);

private:

	/// @brief Internal buffer of blocks
	static std::array<Blk, MAX_BLK> buffer;

	/// @brief Holds fileHandle and offset of blocks
	static std::array<BlkMetaData, MAX_BLK> bufferMetaData;

	static std::list<int> lruIndexQ;
	static std::unordered_map<int, std::list<int>::iterator> lruIndexMap;

private:

	/**
	 * @brief Allocates a block, evicts one if necessary
	 * @return Index of allocated block
	 */
	static int allocateBlock();

	/**
	 * @brief Evicts the least recently used block
	 * @return Index of evicted block in buffer
	 */
	static int evictLRU();
};

constexpr std::array<BlkMetaData, MAX_BLK> initialize_array()
{
	std::array<BlkMetaData, MAX_BLK> ret{};
	for (int i = 0; i < MAX_BLK; i++)
		ret[i].fileHandle = -1;
	return ret;
}





#endif // __BUFMAN_H__