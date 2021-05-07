#include "RBTree/rbtree.hpp"
#include "Bloom/bloom.hpp"
#include "Core/Merge.hpp"
#include "Core/Read.hpp"
#include <thread>
#include <mutex>
#include <fstream>
#include <filesystem>
#include <vector>

#define NUMFILES 10

using namespace std;

struct K
{
	char c[64];
};

struct V
{
	char c[64];
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

	void run(vector <string> batchfiles);

	Master();
};