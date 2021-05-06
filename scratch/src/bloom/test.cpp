#include "bloom.h"
#include <stdio.h>
using namespace std;

//    Use K and V as follows
struct K{
    char c[64];
};

struct V{
    char c[16];
};

char buf[64];

int main(){
    printf("Testing bloom...\n");
    //******************************************* 
    Bloom<K> bloom;

    K k1, k2;
    for(int i=0; i<64; ++i) k1.c[i] = '0';
    for(int i=0; i<64; ++i) k1.c[i] = '1';

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
}