#include "Bloom/bloom.hpp"
#include <stdio.h>
#include <string>
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
    unsigned char c[64];
};

struct V{
    char c[16];
};

char buf[64];

int main(){
    #ifdef TEST
    printf("Testing bloom...\n");
    //******************************************* 
    printf("Test 1...\n");
    Bloom<K> bloom(0);

    K k1, k2;
    for(int i=0; i<64; ++i) k1.c[i] = 0;
    for(int i=0; i<64; ++i) k2.c[i] = (1 << 8) - 1;

    bloom.set(k1);
    if(bloom.get(k1))
        printf("k1 may be present\n");
    else
        printf("k1 not present\n");

    if(bloom.get(k2))
        printf("k2 may be present\n");
    else
        printf("k2 not present\n");

    //*******************************************
    printf("Test2 ......\n");
    Bloom<K> bloom1(1);

    if(bloom1.get(k1))
        printf("k1 may be present\n");
    else
        printf("k1 not present\n");

    if(bloom1.get(k2))
        printf("k2 may be present\n");
    else
        printf("k2 not present\n");


    // *****************************************    
    printf("BloomFile Test1...\n");
    BloomFile<K> bloom2("bloom2", 0, 0);

    bloom2.set(k1);
    if(bloom2.get(k1))
        printf("k1 may be present\n");
    else
        printf("k1 not present\n");

    if(bloom2.get(k2))
        printf("k2 may be present\n");
    else
        printf("k2 not present\n");

    //******************************************
    printf("BloomFile Test2...\n");
    BloomFile<K> bloom3("bloom2", 0, 1);
    if(bloom3.get(k1))
        printf("k1 may be present\n");
    else
        printf("k1 not present\n");

    if(bloom3.get(k2))
        printf("k2 may be present\n");
    else
        printf("k2 not present\n");

    #endif

    #ifdef GENFS
    auto bloom = BloomFile<K1>("bloomdump", 0, 0);
    #endif 
}