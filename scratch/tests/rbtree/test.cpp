#include "RBTree/rbtree.hpp"
#include <stdio.h>
#include <time.h>
using namespace std;
struct K1
{
	unsigned long bytes[8];

	bool operator<(const K1& k2) const
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
//    Use K and V as follows
struct K{
    char c[16];
};

struct V{
    char c[16];
};

char buf[64];

int main(){
    #ifdef TEST
    printf("Testing rbtree...\n");
    //******************************************* 

    auto bt1 = BinaryTree<K, V>();

    //*******************************************
    auto bt = BinaryTree<long long, long long>("logactions.log");
    bt.insert(1, 0);
    bt.insert(4, 0);
    bt.insert(2, 0);
    bt.insert(5, 0);
    bt.insert(6, 0);
    bt.insert(3, 0);
    auto k = bt._inorder();
    while(k != NULL){
        printf("%lld\n", k -> key);
        k = bt._inorder();
    }

    //test whether the tree is freed
    bt._free();
    k = bt._inorder();
    while(k != NULL){
        printf("%lld\n", k -> key);
        k = bt._inorder();
    }

    printf("Testing Recovery from Logfile...\n");
    bt.fromFile("logactions.log");
    k = bt._inorder();
    while(k != NULL){
        printf("%lld\n", k -> key);
        k = bt._inorder();
    }
    bt.dump(fopen("test1.dump", "wb"));

    // test file dump, date etc.
    printf("Dumping bt1...\n");
    bt1.dump(fopen("test.dump", "wb"));
    printf("Freeing bt1 ...\n");
    bt1._free();
    #endif

    #ifdef GENFS
    string fname="file";
    auto bt = BinaryTree<K1, K1>();
    for(int i = 0; i < 10; ++i){
        bt.dump(fopen((fname + to_string(i)).c_str(), "wb"));
    }
    #endif

}