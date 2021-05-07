#include "BufferManager/FileIO.hpp"

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

int FileIO::openFile(const std::string& filePath)
{
	/**
	 * O_RDWR Open for reading and writing
	 * O_SYNC Will write to disk before returning
	 */
	return open(filePath.c_str(), O_RDWR | O_SYNC);
}

void FileIO::closeFile(int fileHandle)
{
	//flushFile(fileHandle);

	unsigned int i = 0;
	for(auto &blk: buffer)
	{
		if(bufferMetaData[i].fileHandle == fileHandle)
		{
			bufferMetaData[i].offset = 0;
			bufferMetaData[i].fileHandle = -1;

			lruIndexQ.erase(lruIndexMap[i]);
			lruIndexMap.erase(i);
		}
		i++;
	}

	close(fileHandle);
}

void FileIO::flushFile(int fileHandle)
{
	unsigned int i = 0;
	for(auto &blk: buffer)
	{
		off_t offset = bufferMetaData[i].offset;
		int fd =  bufferMetaData[i].fileHandle;
		i++;

		if(fd < 0 || fd != fileHandle)
			continue;

		lseek(fileHandle, offset, SEEK_SET);
		write(fileHandle, blk.block, BLK_SIZE);
	}
}

size_t FileIO::fread(void* ptr, size_t size, size_t count, off_t offset, int fileHandle)
{
	unsigned int i = 0;
	for(auto &blk:buffer)
	{
		off_t blkOff = bufferMetaData[i].offset;
		int fd =  bufferMetaData[i].fileHandle;
		i++;

		if(fd < 0 || fd != fileHandle)
			continue;

		if(offset < blkOff || offset >= blkOff + BLK_SIZE)
			continue;

		off_t offsetIntoBlock = offset - blkOff;
		memcpy(ptr, blk.block + offsetIntoBlock, count * size);

		lruIndexQ.erase(lruIndexMap[i-1]);
		lruIndexQ.push_front(i-1);
		lruIndexMap[i-1] = lruIndexQ.begin();

		return count * size;
	}

	i = allocateBlock();

	off_t blkOff = offset - offset % BLK_SIZE;

	lseek(fileHandle, blkOff, SEEK_SET);

	int num = read(fileHandle, buffer[i].block, BLK_SIZE);
	#ifdef _DEBUG
		if(num != BLK_SIZE)
			fprintf(stderr, "FileIO::fread() Could only read %d bytes\n", num);
	#endif

	bufferMetaData[i].offset = blkOff;
	bufferMetaData[i].fileHandle = fileHandle;

	off_t offsetIntoBlock = offset - blkOff;
	memcpy(ptr, buffer[i].block + offsetIntoBlock, count * size);

	return num;
}

int FileIO::allocateBlock() 
{
	unsigned int i = 0;
	for(; i < bufferMetaData.size(); i++)
	{
		if(bufferMetaData[i].fileHandle < 0)
			break;
	}

	if(i == bufferMetaData.size())
	{
		i = evictLRU();
	}

	lruIndexQ.push_front(i);
	lruIndexMap[i] = lruIndexQ.begin();

	return i;
}

int FileIO::evictLRU() 
{
	int toEvict = lruIndexQ.back();

	off_t& offset = bufferMetaData[toEvict].offset;
	int fd =  bufferMetaData[toEvict].fileHandle;
	auto blk = buffer[toEvict];

	lseek(fd, offset, SEEK_SET);
	write(fd, blk.block, BLK_SIZE);

	bufferMetaData[toEvict].offset = 0;
	bufferMetaData[toEvict].fileHandle = -1;

	lruIndexQ.pop_back();
	lruIndexMap.erase(toEvict);

	return toEvict;
}

off_t FileIO::fsize(int fileHandle) 
{
	struct stat fileStat;
	fstat(fileHandle, &fileStat);
	return fileStat.st_size;
}

std::array<Blk, MAX_BLK> FileIO::buffer;
std::array<BlkMetaData, MAX_BLK> FileIO::bufferMetaData = initialize_array();
std::list<int> FileIO::lruIndexQ;
std::unordered_map<int, std::list<int>::iterator> FileIO::lruIndexMap;