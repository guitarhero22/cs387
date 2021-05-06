#ifndef __BLOOM_H__
#define  __BLOOM_H__

#include<stdlib.h>
#include<stdio.h>

template<typename K> // provide Key Type for class template
class Bloom{
    public:
        unsigned char arr[(1 << 16) / 8];
        Bloom(){
            if(sizeof(K) < 48){
                fprintf(stderr, "Key of Size less than 48 not allowed in Bloom filter");
            }
        }
        void set(K k){
            unsigned short *s = (unsigned short *)&k;
            for(int i = 0; i<3; ++i){
                s += i*16;
                arr[(*s) / 8] = arr[(*s) / 8] | ( 1<<((*s) % 8) );
            }
            return;
        };
        bool get(K k){
            unsigned short *s = (unsigned short *)&k;
            for(int i = 0; i<3; ++i){
                s += i*16;
                if(!(arr[(*s) / 8] & ( 1<<((*s)%8)))) return false;
            }
            return true;
        }
};

#endif