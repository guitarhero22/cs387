#include "rbtree.h"
#include <stdio.h>
#include <time.h>
using namespace std;

//    Use K and V as follows
struct K{
    char c[16];
};

struct V{
    char c[16];
};

char buf[64];

int main(){
    printf("Testing rbtree...\n");
    //******************************************* 

    auto bt1 = BinaryTree<K, V>();

    //*******************************************
    auto bt = BinaryTree<int, int>();
    bt.insert(1, 0);
    bt.insert(4, 0);
    bt.insert(2, 0);
    bt.insert(5, 0);
    bt.insert(6, 0);
    bt.insert(3, 0);
    auto k = bt._inorder();
    while(k != NULL){
        printf("%d\n", k -> key);
        k = bt._inorder();
    }

    //test whether the tree is freed
    bt._free();
    k = bt._inorder();
    while(k != NULL){
        printf("%d\n", k -> key);
        k = bt._inorder();
    }

    // test file dump, date etc.
    printf("Dumping bt1...\n");
    bt1.dump(fopen("test.dump", "w"));
    printf("Freeing bt1 ...\n");
    bt1._free();

    // Get current date/time, format is YYYY-MM-DD.HH:mm:ss
    // time_t now = time(0);
    // tm tstruct = *localtime(&now);
    // tstruct = *localtime(&now);
    // for(int i=0; i<64; ++i) buf[i] = '0';
    // strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    // buf[19] = ' ';
    // buf[63] = '\0';
    // printf("%s\n", buf);
}