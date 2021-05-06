#include "RBTree/rbtree.hpp"
#include "Core/Merge.hpp"

struct K
{
	long unsigned higher;
	long unsigned lower;

	K() = default;

	K(int i) : higher(0), lower(i) {}

	bool operator<(const K& k2) const
	{
		if(this->higher < k2.higher)
			return true;
		
		if(this->higher > k2.higher)
			return false;
		
		return this->lower < k2.lower;
	}
};

struct V
{
	long unsigned higher;
	long unsigned lower;

	V() = default;

	V(int i) : higher(0), lower(i) {}

	V(long unsigned h, long unsigned l) : higher(h), lower(l) {}

	bool operator<(const V& v2) const
	{
		if(this->higher < v2.higher)
			return true;
		
		if(this->higher > v2.higher)
			return false;
		
		return this->lower < v2.lower;
	}
};

bool _test_merge()
{
	V tomb = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};

	auto bt1 = BinaryTree<K, V>("logactions.log", 0);
	auto bt2 = BinaryTree<K, V>();

	for(int i = 0; i < 10; i++)
	{
		bt1.insert(i*2, i*10);
	}

	for(int i = 0; i < 10; i++)
	{
		bt2.insert(i*2+1, i*10+1);
	}

	bt2.insert(8, tomb);

	printf("Dumping bt1...\n");
	bt1.dump(fopen("testbt1.dump", "wb"));
	bt1._free();
	printf("Dumping bt2...\n");
	bt2.dump(fopen("testbt2.dump", "wb"));
	bt2._free();

	merge_files<K, V>(fopen("testbt2.dump", "rb"), fopen("testbt1.dump", "rb"), fopen("merged.dump", "wb"));
}

int main()
{
	_test_merge();
	return 0;
}