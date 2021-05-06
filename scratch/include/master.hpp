#include "RBTree/rbtree.hpp"
#include "Bloom/bloom.hpp"
#include "Core/Merge.hpp"
#include <thread>
#include <mutex>

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
	FILE* filesys[10];
	FILE* backups[2];
	FILE* bloomdump;
	FILE* tempfile;
	
	pair<K, V> dbread(K k);
	void dbwrite(K k, V v);

	void adjust();

	void serve(string batchfile);

	void run();

	Master();
};