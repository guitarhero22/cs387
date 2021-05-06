#include "RBTree/rbtree.hpp"
#include "Core/Merge.hpp"

bool _test_merge()
{
	auto bt1 = BinaryTree<long long, long long>();
	auto bt2 = BinaryTree<long long, long long>();

	for(int i = 0; i < 10; i++)
	{
		bt1.insert(i*2, i*10);
	}

	// for(int i = 0; i < 10; i++)
	// {
	// 	bt2.insert(i*2+1, i*10+1);
	// }

	printf("Dumping bt1...\n");
	bt1.dump(fopen("bt1.dump", "wb"));
	bt1._free();
	// printf("Dumping bt2...\n");
	// bt2.dump(fopen("bt2.dump", "wb"));
	// bt2._free();

	// merge_files<long long, long long>(fopen("bt2.dump", "rb"), fopen("bt1.dump", "rb"), fopen("merged.dump", "wb"));
}

int main()
{
	_test_merge();
	return 0;
}