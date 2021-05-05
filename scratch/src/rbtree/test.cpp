#include "rbtree.h"
#include <stdio.h>
using namespace std;

int main(){
    printf("Testing rbtree...\n");
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
}