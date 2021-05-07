#ifndef __BLOOM_H__
#define  __BLOOM_H__

#include<stdlib.h>
#include<stdio.h>
#include<cstring>
#include<string>
#include<iostream>

using namespace std;

template<typename K> // provide Key Type for class template
class Bloom{
    public:
        unsigned char arr[(1 << 16) / 8];
        Bloom(bool initial){
            if(sizeof(K) < 48){
                fprintf(stderr, "Key of Size less than 48 not allowed in Bloom filter");
            }
            if(!initial)
                memset(arr, 0, sizeof(unsigned char) * ((1 << 16) / 8));
            else
                memset(arr, (1 << 8) - 1, sizeof(unsigned char) * ((1 << 16) / 8));
        }

        void set(K k){
            /*
                Pass a key to set the corresponding bits 1 in the bloom array
            */
            unsigned short *s = (unsigned short *)&k;
            s -= 16;
            for(int i = 0; i<3; ++i){
                // #ifdef _DEBUG
                //     fprintf(stderr, "%hu\n", *s);
                // #endif
                s += 16;
                arr[(*s) / 8] = arr[(*s) / 8] | ( 1<<((*s) % 8) );
            }
            return;
        };

        bool get(K k){
            /*
                Pass a key to find out whether the key is present
            */
            unsigned short *s = (unsigned short *)&k;
            s -= 16;
            for(int i = 0; i<3; ++i){
                s += 16;
                if(!( arr[(*s) / 8] & (1<<((*s)%8)) )) return false;
            }
            return true;
        }
};

template<typename K>
class BloomFile{
    /*
        This version of the bloom filter will have an associated file on disk
        POroviding some crash consistency
    */
    public:
        unsigned char arr[(1 << 16) / 8];
        string fname;
        FILE * bloomwrite = NULL;
        BloomFile(string _fname, bool initial, bool load): fname(_fname){
            /*
                if load is true the Bloom Arr will be loaded from the given file
                else a new bloom file will be create with fname and bool array will be initialized to initial
            */
           if(load){
               FILE * bloomread = fopen(fname.c_str(), "rb");
               if(bloomread == NULL){
                   fprintf(stderr, "Bloom File Not Found\n");
                   exit(1);
               }
                if(((1<<16)/8) != fread(arr, sizeof(unsigned char), (1<<16)/8, bloomread)){
                    fprintf(stderr, "Bloom File Corrupted!\n");
                    exit(1);
                }
                fclose(bloomread);
           }else{
                if(sizeof(K) < 48){
                    fprintf(stderr, "Key of Size less than 48 not allowed in Bloom filter\n");
                    exit(1);
                }
                if(!initial)
                    memset(arr, 0, ((1 << 16) / 8));
                else
                    memset(arr, (1 << 8) - 1, ((1 << 16) / 8));
           }
            bloomwrite = fopen(fname.c_str(), "wb");
                if(bloomwrite == NULL){
                    fprintf(stderr,  "Bloom: Could not create File  Found!\n");
                    exit(1);
                }
                fwrite(arr, sizeof(unsigned char), (1<<16)/8, bloomwrite);
                fflush(bloomwrite);
        }

        void set(K k){
            /*
                Pass a key to set the corresponding bits 1 in the bloom array
            */
            unsigned short *s = (unsigned short *)&k;
            
            s -= 1;
            for(int i = 0; i<3; ++i){
                s += 1;
                unsigned char temp = arr[(*s) / 8] | ( 1<<((*s) % 8) );
                if(temp != arr[(*s) / 8]){
                    arr[(*s) / 8] = temp;
                    setFile((*s) / 8);
                }
            }
            return;
        };

        bool get(K k){
            /*
                Pass a key to find out whether the key is present
            */
            unsigned short *s = (unsigned short *) &k;
            s -= 1;
            for(int i = 0; i<3; ++i){
                s += 1;
                if(!(arr[(*s) / 8] & ( 1<<((*s)%8)))) return false;
            }
            return true;
        }

        void setFile(int i){
            fseek(bloomwrite, i*sizeof(unsigned char), SEEK_SET);
            if(fwrite(arr + i, sizeof(unsigned char), 1, bloomwrite) != 1){
                fprintf(stderr, "BloomFile::setFile - write error\n");
                exit(1);
            };
            fflush(bloomwrite);
        }

        ~BloomFile(){
            if(bloomwrite != NULL)
                fclose(bloomwrite);
        }
};

#endif