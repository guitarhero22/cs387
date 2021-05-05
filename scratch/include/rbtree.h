#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<stack>
#include<cmath>
#include<time.h>
#include "files.h"

#define _temp_ template<typename T>
#define _temp1_ template<typename K, typename V>

#ifndef INCLUDE_RBTREE
#define INCLUDE_RBTREE

using namespace std;

_temp1_
class Node{
    /* 
        K - Key Type
        V - Value Type
        K should have a < and == operator defined on them
    */
    public:
        K key;
        V value;
        int del = 0;
        Node<K, V> *l, *r;
        // ***************** API ********************
        Node(K k, V v): 
            key(k), value(v), l(NULL), r(NULL){} //class constructor
};

_temp1_
class BinaryTree{
    /* 
        K - Key Type
        V - Value Type
        K should have a < and == operator defined on them

        For K use something like struct **K{char data[64];};** OR class **K{public: data[64];};**
    */
    public:
        Node<K,V>* root; //Pointer to the root
        int size = 0; //Number of Nodes
        int traversal_started = 0, sz_k = 0, sz_v = 0;
        stack<Node<K,V>*> stk; // for inorder traversal

        // Constructors ******************
        void check(){
            float f = BLK_SIZE * 1.0 / (sz_k + sz_v);
            if(floor(f) != f){
                fprintf(stderr, "Key and Value size not consistent with block size");
                exit(1);
            }
            if((sz_v + sz_k) < 20){
                fprintf(stderr, "Combined size of key and value less than timestamp size, functionality not implemented");
                exit(1);
            }
        }
        BinaryTree(): 
            sz_k(sizeof(K)), sz_v(sizeof(V))
            {check();}

        BinaryTree(Node<K,V> n):
            sz_k(sizeof(K)), sz_v(sizeof(V))
            { //class constructor
                this->root = new Node<K,V>(); 
                *this->root = n;
            }

        BinaryTree(K k, V v):
            sz_k(sizeof(K)), sz_v(sizeof(V))
            { //class constructor
                this->root = new Node<K,V>(k, v);
            }

        //internal helper function / not to be used
        Node<K,V>* _search(K); 

        /* ************************** API ********************* */
        Node<K,V>* search(K); //search for a Key 
        int insert(K,V); //insert a Key, Value pair passed as an argumrnt
        int insert(Node<K,V>); //insert a Node containing the key and value

        // The deletes dont actually delete the nodes, rather set the del var of the nodes
        int del(K); //delete a Node with Key k
        int del(Node<K,V>*); //delete the Node with the same key as the given Node

        
        int dump(FILE*);//dumps the tree in the given file and empties the tree
        void empty(){}//empties the whole tree
        Node<K,V>* _inorder();//inorder travversal of the tree for traversal
};

// _temp1_
// class  RBTree{
//     public:
//         Node<K,V>* search(K);
//         Node<K,V>* insert(K);
//         int del(K);
//         int del(Node<K,V>*)
// };

#endif

#ifndef INCLUDE_BTREESRC
#define INCLUDE_BTREESRC

_temp1_
Node<K,V>* BinaryTree<K,V>::_search(K k){
    Node<K,V> *cur = this->root;
    while(cur != NULL){
        if(k < cur->key){
            if(cur -> l != NULL){
                cur = cur -> l;
            }else{
                return cur;
            }
        }
        else if(cur->key < k){
            if(cur -> r != NULL){
                cur = cur -> r;
            }else{
                return cur;
            }
        }else{
            return cur;
        }
    }
    return cur;
}

_temp1_
Node<K,V>* BinaryTree<K,V>::search(K k){
    Node<K,V> *n = this->_search(k);
    if(n == NULL) return NULL;
    if(n -> key == k) return n;
    return NULL;
}

_temp1_
int BinaryTree<K,V>::insert(K k, V v){

    Node<K,V>* p = this->_search(k);
    if(p == NULL){
        this->root = new Node<K,V>(k,v);
        this->size ++;
        return 0;
    }

    if(p->key < k){
        p -> r = new Node<K,V>(k,v);
        this->size ++;
    }
    else if(k < p->key){
        p -> l = new Node<K,V>(k,v);
        this->size ++;
    }else{
        p -> value = v;
        p -> del = 0;
    }

    return 0;
}

_temp1_
int BinaryTree<K,V>::insert(Node<K,V> n){
    return this->insert(n.key, n.value);
}

_temp1_
int BinaryTree<K,V>::del(K k){
    Node<K,V> *n = this->search(k);
    if(n == NULL) return -1;
    n -> del = 1;
    return 0;
}

_temp1_
int BinaryTree<K,V>::del(Node<K,V> *n){
    return this->del(n->key);
}

_temp1_
Node<K,V>* BinaryTree<K,V>::_inorder(){
    //Under DEV TODO
    if(!traversal_started){
        traversal_started = 1;
        while(!stk.empty()) stk.pop();
        Node<K,V>* n = root;
        while(n != NULL) {
            stk.push(n);
            n = n->l;
        }
    }
    if(stk.empty() && traversal_started){
        traversal_started = 0;
        return NULL;
    }

    Node<K,V>* n = stk.top();
    stk.pop();
    Node<K,V>* n1 = n -> r;
    while(n1 != NULL){
        stk.push(n1);
        n1 = n1 -> l;
    }
    return n; 
}

_temp1_
int BinaryTree<K,V>::dump(FILE *f){
    /* Dumps the tree content in a file */
    traversal_started = 1;
    while(!stk.empty()) stk.pop();
    if(root != NULL){
        Node<K,V>* n = root;
        while(n != NULL){
            stk.push(n);
            n = n -> l;
        }
    }

    int num_entries = BLK_SIZE / (sz_k + sz_v);

    // get time
    char *time_str;
    time_str = malloc(sizeof(char) * (sz_k + sz_v));
    for(int i=0;i<64;i++) time_str[i] = ' ';
    time_t now = time(0);
    tm tstruct = *localtime(&now);
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    printf("%s\n", buf);

    Node<K,V> * n;
    while(!stk.empty()){
        n = stk.top();
        stk.pop();
        /* 
            Do whatever you want with n here 
        */
        n = n->r;
        while(n != NULL){
            stk.push(n);
            n = n->l;
        }
    }
    traversal_started = 0;
    return 0;
}

#endif