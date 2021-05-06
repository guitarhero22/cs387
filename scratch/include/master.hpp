#include "RBTree/rbtree.hpp"
#include "Bloom/bloom.hpp"
#include "Core/Merge.hpp"
#include <thread>
#include <mutex>
#include <filesystem>

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
	string filesys[10];
	string backups[2];
	string bloomdump;
	string tempfile;
	
	pair<K, V> dbread(K k);
	void dbwrite(K k, V v);

	void adjust();

	void serve(string batchfile);

	void run();

	Master();
};