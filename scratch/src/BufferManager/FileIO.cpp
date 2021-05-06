#include "BufferManager/FileIO.hpp"

#include <sys/stat.h>
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
	flushFile(fileHandle);

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

		if(blkOff < offset || offset >= blkOff + BLK_SIZE)
			continue;

		off_t offsetIntoBlock = offset - blkOff;
		memcpy(ptr, blk.block + offsetIntoBlock, count * size);

		lruIndexQ.erase(lruIndexMap[i]);
		lruIndexQ.push_front(i);
		lruIndexMap[i] = lruIndexQ.begin();

		return count * size;
	}

	i = allocateBlock();

	off_t blkOff = offset - offset % BLK_SIZE;

	lseek(fileHandle, blkOff, SEEK_SET);

	if(read(fileHandle, buffer[i].block, BLK_SIZE) != BLK_SIZE)
	{
		return 0;
	}

	bufferMetaData[i].offset = blkOff;
	bufferMetaData[i].fileHandle = fileHandle;

	off_t offsetIntoBlock = offset - blkOff;
	memcpy(ptr, buffer[i].block + offsetIntoBlock, count * size);

	return count * size;
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