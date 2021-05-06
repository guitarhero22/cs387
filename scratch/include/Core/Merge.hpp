#ifndef __MERGE_H__
#define __MERGE_H__

#include "utils.hpp"

#include <stdio.h>
#include <time.h>
#include <string>
#include <cstring>

static byte tombByte[] = {TOMBSTONE_BYTE};

/**
 * @brief Checks if the value is a tombstone
 * @tparam V entry value type
 * @param entryv pointer to entry value
 * @return true if entry value is equal to the tombstone value, false otherwise
 */
template<typename V>
static bool isTomb(V* entryv)
{

	size_t sz_v = sizeof(V);
	for(int i = 0; i < sz_v; i++)
	{
		if(memcmp((byte*)entryv + i, tombByte, 1) != 0)
		{
			return false;
		}
	}
	return true;
}

/**
 * @brief Merges two files into a third
 * @tparam K entry key type, > operator needs to be overloaded for K
 * @tparam V entry value type
 * @param oldest2 Second oldest file, must be opened in read mode
 * @param oldest1 oldest file, must be opened in read mode)
 * @param newfd File to be output to (retains timestamp of oldest2), must be opened in write mode
 */
template<typename K, typename V>
void merge_files(FILE* oldest2, FILE* oldest1, FILE* newfd)
{

	size_t sz_k = sizeof(K);
	size_t sz_v = sizeof(V);

	fseek(oldest1, 0, SEEK_SET);
	fseek(oldest2, 0, SEEK_SET);
	fseek(newfd, 0, SEEK_SET);

	char time_str1[sz_k + sz_v];
	fread(time_str1, 1, sz_k + sz_v, oldest1);

	char time_str2[sz_k + sz_v];
	fread(time_str2, 1, sz_k + sz_v, oldest2);

	tm ts1;
	strptime(time_str1, TIME_FMT, &ts1);

	tm ts2;
	strptime(time_str2, TIME_FMT, &ts2);

	if(difftime(mktime(&ts2), mktime(&ts1)) < 0)
	{
		#ifdef _DEBUG
			fprintf(stderr, "merge_files: Files not passed in correct order, files not merged");
		#endif

		return;
	}

	fwrite(time_str2, 1, sz_k + sz_v, newfd);

	K entry1k;
	V entry1v;
	K entry2k;
	V entry2v;

	fread(&entry1k, sz_k, 1, oldest1);
	fread(&entry1v, sz_v, 1, oldest1);
	fread(&entry2k, sz_k, 1, oldest2);
	fread(&entry2v, sz_v, 1, oldest2);

	while(1)
	{

		if(entry1k < entry2k)
		{
			if(!isTomb(&entry1v))
			{
				fwrite(&entry1k, sz_k, 1, newfd);
				fwrite(&entry1v, sz_v, 1, newfd);
			}

			fread(&entry1k, sz_k, 1, oldest1);
			fread(&entry1v, sz_v, 1, oldest1);
		}
		else if(entry2k < entry1k)
		{

			if(!isTomb(&entry2v))
			{
				fwrite(&entry2k, sz_k, 1, newfd);
				fwrite(&entry2v, sz_v, 1, newfd);
			}

			fread(&entry2k, sz_k, 1, oldest2);
			fread(&entry2v, sz_v, 1, oldest2);
		}
		else
		{
			if(!isTomb(&entry2v))
			{
				fwrite(&entry2k, sz_k, 1, newfd);
				fwrite(&entry2v, sz_v, 1, newfd);
			}

			fread(&entry1k, sz_k, 1, oldest1);
			fread(&entry1v, sz_v, 1, oldest1);
			fread(&entry2k, sz_k, 1, oldest2);
			fread(&entry2v, sz_v, 1, oldest2);
		}

		if(feof(oldest1))
			break;
		if(feof(oldest2))
			break;
	}

	while(1)
	{
		if(feof(oldest1))
			break;
		
		fread(&entry1k, sz_k, 1, oldest1);
		fread(&entry1v, sz_v, 1, oldest1);

		if(!isTomb(&entry1v))
		{
			fwrite(&entry1k, sz_k, 1, newfd);
			fwrite(&entry1v, sz_v, 1, newfd);
		}
	}

	while(1)
	{
		if(feof(oldest2))
			break;
		
		fread(&entry2k, sz_k, 1, oldest2);
		fread(&entry2v, sz_v, 1, oldest2);

		if(!isTomb(&entry2v))
		{
			fwrite(&entry2k, sz_k, 1, newfd);
			fwrite(&entry2v, sz_v, 1, newfd);
		}
	}

	return;
}

#endif // __MERGE_H__