#ifndef __MASTER_H__
#define __MASTER_H__

#include "RBTree/rbtree.hpp"
#include "Bloom/bloom.hpp"
#include "Core/Merge.hpp"
#include "Core/Read.hpp"
#include "utils.hpp"
#include <thread>
#include <mutex>
#include <fstream>
#include <filesystem>
#include <vector>

#define MAX_TREE_SIZE 5
#define NUMFILES 10

using namespace std;

struct K
{
	unsigned long bytes[8];

	bool operator<(const K& k2) const
	{
		for(int i = 0; i < 8; i++)
		{
			if(bytes[i] < k2.bytes[i])
				return true;
			else if(bytes[i] > k2.bytes[i])
				return false;
			else
				continue;
		}
		return false;
	}
};

struct V
{
	unsigned long bytes[8];
};

class Master{
	public:
	BinaryTree <K, V> *bintree, *reserve;
	BloomFile <K> *bloom;
	mutex memlock, fslock;
	int recent, current;
	string filesys[NUMFILES];
	string backups[2];
	string bloomdump;
	string tempfile;
	
	bool dbread(const K& k, V& v);
	void dbwrite(K k, V v);

	void adjust();

	void serve(string batchfile);
	int batchHandler(string fname);
	void run(vector <string> batchfiles);

	Master();

	~Master();
};

#endif