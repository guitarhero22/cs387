#include<stdio.h>
#include<unistd.h>
#include "RBTree/rbtree.hpp"
#include "Bloom/bloom.hpp"

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

int main(){
    // Make bloomdump
    auto bloom = BloomFile<K>("bloomdump", 0, 0);

    //Make FS
    auto bt = BinaryTree<K,K>();
    for(int i = 0; i<10; ++i){
        sleep(1);
        bt.dump(fopen(("file" + to_string(i)).c_str(), "wb"));
    }

    //Make Backups
    bt.setLogFile("backup0");
    bt.setLogFile("backup1");
}