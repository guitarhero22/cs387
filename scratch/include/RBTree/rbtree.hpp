#ifndef __RBTREE_H__
#define __RBTREE_H__

#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<stack>
#include<cmath>
#include<time.h>
#include<string>
#include "files.hpp"
#include "utils.hpp"

using namespace std;

unsigned char _write_ = 1,
              _del_ = 2,
              _sync_ = 3;

template<typename K, typename V>
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
        Node(){};
        Node(K k, V v): 
            key(k), value(v), l(NULL), r(NULL){} //class constructor
};

template<typename K, typename V>
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
        FILE *logfile = NULL;
        stack<Node<K,V>*> stk; // for inorder traversal
        pthread_mutex_t lock;

        // Constructors ******************
        void check(){
            float f = BLK_SIZE * 1.0 / (sz_k + sz_v);
            if(floor(f) != f){
                fprintf(stderr, "Key and Value size not consistent with block size");
                exit(1);
            }

            #ifdef _DEBUG
                fprintf(stderr, "Size of Key %d, Size of Value %d\n", sz_k, sz_v);
            #endif
        }

        BinaryTree(): 
            sz_k(sizeof(K)), sz_v(sizeof(V)), root(NULL)
            {check();}

        BinaryTree(string _logfile, bool load): 
            sz_k(sizeof(K)), sz_v(sizeof(V)), root(NULL)
            {   /* Provide a file name to be used as log
                  and use 
                */
                if(load){

                }else{
                    logfile = fopen(_logfile.c_str(), "wb");
                }
            }

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
        void log_action(K k, V v, unsigned char act);
        Node<K,V>* _search(K); 

        /* ************************** API **********************/
        Node<K,V>* search(K); //search for a Key 
        int insert(K,V); //insert a Key, Value pair passed as an argumrnt
        int insert(Node<K,V>); //insert a Node containing the key and value

        // The deletes dont actually delete the nodes, rather set the del var of the nodes
        int del(K); //delete a Node with Key k
        int del(Node<K,V>*); //delete the Node with the same key as the given Node

        
        int dump(FILE*);//dumps the tree in the given file and empties the tree
        void _free();//empties the whole tree
        Node<K,V>* _inorder();//inorder travversal of the tree for traversal
};

// template<typename K, typename V>
// class  RBTree{
//     public:
//         Node<K,V>* search(K);
//         Node<K,V>* insert(K);
//         int del(K);
//         int del(Node<K,V>*)
// };
template<typename K, typename V>
void BinaryTree<K,V>::log_action(K k, V v, unsigned char act){
            //internal function will write logs to logfile
            if(logfile == NULL) return;
            fwrite(&act, sizeof(unsigned char), 1, logfile);
            fwrite(&k, 1, sz_k, logfile);
            fwrite(&v, 1, sz_v, logfile);
            fflush(logfile);
            return;
}

template<typename K, typename V>
Node<K,V>* BinaryTree<K,V>::_search(K k){
    Node<K,V> *cur = this->root;
    while(cur != NULL){
        if(k < (cur->key)){
            if(cur -> l != NULL){
                cur = cur -> l;
            }else{
                return cur;
            }
        }
        else if((cur->key) < k){
            if((cur->r) != NULL){
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

template<typename K, typename V>
Node<K,V>* BinaryTree<K,V>::search(K k){
    pthread_mutex_lock(&lock);
    Node<K,V> *n = this->_search(k);
    if(n -> key != k) n = NULL;
    pthread_mutex_unlock(&lock);
    return n;
}

template<typename K, typename V>
int BinaryTree<K,V>::insert(K k, V v){

    pthread_mutex_lock(&lock);
    Node<K,V>* p = this->_search(k);
    if(p == NULL){
        this->root = new Node<K,V>(k,v);
        this->size ++;
        pthread_mutex_unlock(&lock);
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
    log_action(k, v, _write_);
    pthread_mutex_unlock(&lock);
    return 0;
}

template<typename K, typename V>
int BinaryTree<K,V>::insert(Node<K,V> n){
    return this->insert(n.key, n.value);
}

template<typename K, typename V>
int BinaryTree<K,V>::del(K k){
    Node<K,V> *n = this->search(k);
    pthread_mutex_lock(&lock);
    if(n == NULL) {
        pthread_mutex_unlock(&lock);
        return -1;
    }
    n -> del = 1;
    memset(&(n -> value), 0, sz_v); //setting the value of the Value 0000...00000 for marking as delete
    Node<K,V> dummy;
    log_action(k, dummy.V, _del_);
    pthread_mutex_unlock(&lock);
    return 0;
}

template<typename K, typename V>
int BinaryTree<K,V>::del(Node<K,V> *n){
    return this->del(n->key);
}

template<typename K, typename V>
Node<K,V>* BinaryTree<K,V>::_inorder(){
    //Under DEV TODO
    pthread_mutex_lock(&lock);
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
        pthread_mutex_unlock(&lock);
        return NULL;
    }

    Node<K,V>* n = stk.top();
    stk.pop();
    Node<K,V>* n1 = n -> r;
    while(n1 != NULL){
        stk.push(n1);
        n1 = n1 -> l;
    }
    pthread_mutex_unlock(&lock);
    return n; 
}

template<typename K, typename V>
int BinaryTree<K,V>::dump(FILE *f){
    /* Dumps the tree content in a file */
    pthread_mutex_lock(&lock);
    traversal_started = 1;
    while(!stk.empty()) stk.pop();
    Node<K,V>* n = root;
    while(n != NULL){
        stk.push(n);
        n = n -> l;
    }

    int num_entries = BLK_SIZE / (sz_k + sz_v);

    // get time
    char *time_str;
    time_str = (char*) malloc(sz_k + sz_v);
    for(int i=0;i<sz_k + sz_v;i++) time_str[i] = '0';
    time_t now = time(0);
    tm tstruct = *localtime(&now);
    tstruct = *localtime(&now);
    strftime(time_str, 20, TIME_FMT, &tstruct);
    time_str[19] = '0';
    #ifdef _DEBUG
        fprintf(stderr, "Check the time: %s\n", time_str);
    #endif
    fwrite(time_str, 1, sz_k + sz_v, f);

    while(!stk.empty()){
        n = stk.top();
        stk.pop();
        /* 
            Do whatever you want with n here 
        */
        #ifdef _DEBUG
            fprintf(stderr, "Program Here\n");
        #endif

        fwrite(&(n -> key), 1, sz_k, f);
        fwrite(&(n -> value), 1, sz_v, f);

        n = n->r;
        while(n != NULL){
            stk.push(n);
            n = n->l;
        }
    }
    traversal_started = 0;
    fflush(f);
    fclose(f);
    Node<K,V> dummy;
    log_action(dummy.key, dummy.value, _sync_);
    free(time_str);
    pthread_mutex_unlock(&lock);
    return 0;
}

template<typename K, typename V>
void BinaryTree<K,V>::_free(){
    pthread_mutex_lock(&lock);
    traversal_started = 1;
    while(!stk.empty()) stk.pop();
    Node<K,V>* n = root;
    if(n != NULL) stk.push(n);
    #ifdef _DEBUG
        if(!stk.empty()){
                fprintf(stderr, "Size of tree: %d, stk not empty\n", size);
        }
    #endif
    while(!stk.empty()){
        #ifdef _DEBUG
            fprintf(stderr, "_free: Program Here\n");
        #endif
        n = stk.top();
        stk.pop();
        if(n -> l != NULL) stk.push(n -> l);
        if(n -> r != NULL) stk.push(n -> r);
        delete n;
    }
    root = NULL;
    size = 0;
    traversal_started = 0;
    pthread_mutex_unlock(&lock);
    return;
}

#endif
