#ifndef __READ_H__
#define __READ_H__

#include "BufferManager/FileIO.hpp"

#include <string>

/**
 * @brief Find key inside file and return value
 * @tparam K key type, must overload < operator
 * @tparam V value type
 * @param fileHandle fileHandle for file, must be given by FileIO::fopen()
 * @param key Search key
 * @param value Value returned by reference
 * @return true if key found, false otherwise
 */
template<typename K, typename V>
bool findEntry(int fileHandle, const K& key, V& value)
{
	size_t sz_k = sizeof(K);
	size_t sz_v = sizeof(V);

	off_t entrySize = sz_k + sz_v;

	long low = 1;
	long high = FileIO::fsize(fileHandle) / entrySize - 1;
	long mid;

	K entryKey;

	while (low <= high)
	{
		mid = (low + high)/2;

		FileIO::fread(&entryKey, sz_k, 1, mid * entrySize, fileHandle);

		if(entryKey < key)
		{
			low = mid + 1;
			continue;
		}
		else if(key < entryKey)
		{
			high = mid - 1;
			continue;
		}
		else
		{
			FileIO::fread(&value, sz_v, 1, mid * entrySize + sz_k, fileHandle);
			return true;
		}
	}

	return false;
	
}

#endif // __READ_H__