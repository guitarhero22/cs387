#include "rbtree.h"
#include <stdio.h>
#include <time.h>
using namespace std;

//    Use K and V as follows
struct K{
    char c[64];
};

struct V{
    char c[64];
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

    // Get current date/time, format is YYYY-MM-DD.HH:mm:ss
    time_t now = time(0);
    tm tstruct = *localtime(&now);
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    printf("%s\n", buf);
}