#ifndef __RBTREE_H__
#define __RBTREE_H__

#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<stack>
#include<cmath>
#include<time.h>
#include<string>
#include<string.h>
#include "files.hpp"
#include "utils.hpp"
#include <pthread.h>

using namespace std;



/** 
 * @brief Node for the In Memory Trees
 * @tparam K - Key Typeshould have a < and == operator defined on them
 * @tparam V - Value Type
 */
template<typename K, typename V>
class Node
{
    public:
        K key;
        V value;
        int del = 0;
        Node<K, V> *l, *r;
        // ***************** API ********************
        Node(){};
        Node(K k, V v): key(k), value(v), l(NULL), r(NULL){}
};

/** 
  * @tparam K - Key Type
  * @tparam V - Value Type
  * @param K should have a < and == operator defined on them \
        For K use something like struct **K{char data[64];};** OR class **K{public: data[64];};**
*/
template<typename K, typename V>
class BinaryTree{
    public:
        int size = 0; //Number of Nodes
        int traversal_started = 0, sz_k = 0, sz_v = 0;
        FILE *logfile = NULL;
        string logfilename = "";
        stack<Node<K,V>*> stk; // for inorder traversal
        Node<K,V>* root; //Pointer to the root
        unsigned char _write_ = 1,
              _del_ = 2,
              _sync_ = 3;
        // pthread_mutex_t lock;

        // Constructors ******************
        void check(); // internal function
        void fromFile(string); //loads the Binary Tree from File
        void setLogFile(string); //sets a new logfile
        BinaryTree(): 
            sz_k(sizeof(K)), sz_v(sizeof(V)), root(NULL)
            {check();}

        BinaryTree(string _logfile): 
            sz_k(sizeof(K)), sz_v(sizeof(V)), root(NULL)
        {  
            setLogFile(_logfile);
        }

        BinaryTree(Node<K,V> n):
            sz_k(sizeof(K)), sz_v(sizeof(V))
        {
            this->root = new Node<K,V>(); 
            *this->root = n;
        }

        BinaryTree(K k, V v):
            sz_k(sizeof(K)), sz_v(sizeof(V))
        {
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
        ~BinaryTree()
        {
            while(!stk.empty()) stk.pop();
            setLogFile("");
        }
};

template<typename K, typename V>
void BinaryTree<K,V>::setLogFile(string _logfile)
{
    logfilename = _logfile;
    if(logfile != NULL) fclose(logfile);
    if(logfilename == "") return;
    logfile = fopen(_logfile.c_str(), "wb");
    if(logfile == NULL){
        fprintf(stderr, "Couldn't backup create file!\n");
    }
}

template<typename K, typename V>
void BinaryTree<K,V>::fromFile(string fname)
{
    if(fname == logfilename){
        fprintf(stderr, "log filenmae same as backup file\n");
        char *log_str = (char*) malloc(30);
        strcpy(log_str, "backup-");
        time_t now = time(0);
        tm tstruct = *localtime(&now);
        tstruct = *localtime(&now);
        strftime(log_str + 7, 20, TIME_FMT, &tstruct);
        fprintf(stderr, "Using the following name instead: %s\n", log_str);
        setLogFile(string(log_str));
            
        errlog("BinaryTRee::fromFile freeing log_str here\n");

        free(log_str);

        errlog("BinaryTRee::fromFile log_str freed successfully here\n");
    }
    FILE *backup = fopen(fname.c_str(), "rb");
    if(backup == NULL)
    {
        fprintf(stderr, "BinaryTree Backup File Not Found!\n");
        exit(1);
    }
    _free();
    int entry_size = sz_k + sz_v + 1;
    long int start = ftell(backup);
    fseek(backup, 0, SEEK_END);
    float num_entries = 1.0 * (ftell(backup) - start) / (entry_size);
    
    if(floor(num_entries) != num_entries){
        fprintf(stderr, "Backup File Corrupted!\n");
        exit(1);
    }
    unsigned char * buff = (unsigned char *) malloc(entry_size + 1);
    bool found_sync = false;
    errlog("BinaryTree::fromFile Number of entries: %f, start: %ld, end: %ld\n", num_entries, start, ftell(backup));
    if(start != ftell(backup)){
        while(ftell(backup) > start && !found_sync){
            errlog("BinaryTree::fromFile inside while loop: start: %ld, end %ld\n", start, ftell(backup));
            fseek(backup, -entry_size, SEEK_CUR);
            fread(buff, entry_size, 1, backup);
            fseek(backup, -entry_size, SEEK_CUR);
            if(*((unsigned char *) buff) == _sync_){
                found_sync = true;
                errlog("Backup Restore: Found _sync_\n");
            }
        }
        if(found_sync) fseek(backup, entry_size, SEEK_CUR);
        errlog("BinaryTree::fromFile while end: End of File: %d, current pos: %ld\n", feof(backup), ftell(backup));
        while(!feof(backup)){
            fread(buff, entry_size, 1, backup);
            if(*(unsigned char*) buff == _write_)
            {
                insert(*(K *)(buff + 1), *(V *)(buff + 1 + sz_k));
            }
            else if(*(unsigned char*) buff == _del_){
                del(*(K *)(buff + 1));
            }else if(*(unsigned char*) buff == _sync_)
            {
                errlog("Backups in Sync\n");
            }else{
                fprintf(stderr, "backup entry corrupted!\n");
            }
        }
    }
    if(backup != NULL) fclose(backup);
    return;
}

template<typename K, typename V>
void BinaryTree<K,V>::check()
{
    float f = BLK_SIZE * 1.0 / (sz_k + sz_v);
    if(floor(f) != f)
    {
        fprintf(stderr, "Key and Value size not consistent with block size");
        exit(1);
    }
    errlog("Size of Key %d, Size of Value %d\n", sz_k, sz_v);
}

template<typename K, typename V>
void BinaryTree<K,V>::log_action(K k, V v, unsigned char act)
{
    if(logfile == NULL) return;
    fwrite(&act, sizeof(unsigned char), 1, logfile);
    fwrite(&k, 1, sz_k, logfile);
    fwrite(&v, 1, sz_v, logfile);
    fflush(logfile);
    return;
}

template<typename K, typename V>
Node<K,V>* BinaryTree<K,V>::_search(K k)
{
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
    // pthread_mutex_lock(&lock);
    Node<K,V> *n = this->_search(k);
    if(n != NULL)
        if((n->key) < k || k < (n->key)) n = NULL;
    // pthread_mutex_unlock(&lock);
    return n;
}

template<typename K, typename V>
int BinaryTree<K,V>::insert(K k, V v){
    errlog("BinaryTree::insert: Write request incoming\n");
    // pthread_mutex_lock(&lock);
    Node<K,V>* p = this->_search(k);
    if(p == NULL){
        this->root = new Node<K,V>(k,v);
        this->size ++;
        log_action(k, v, _write_);
        // pthread_mutex_unlock(&lock);
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
    // pthread_mutex_unlock(&lock);
    return 0;
}

template<typename K, typename V>
int BinaryTree<K,V>::insert(Node<K,V> n){
    return this->insert(n.key, n.value);
}

template<typename K, typename V>
int BinaryTree<K,V>::del(K k){
    Node<K,V> *n = this->search(k);
    // pthread_mutex_lock(&lock);
    if(n == NULL) {
        // pthread_mutex_unlock(&lock);
        return -1;
    }
    n -> del = 1;
    memset(&(n -> value), TOMBSTONE_BYTE, sz_v); //setting the value of the Value 0000...00000 for marking as delete
    Node<K,V> dummy;
    log_action(k, dummy.value, _del_);
    // pthread_mutex_unlock(&lock);
    return 0;
}

template<typename K, typename V>
int BinaryTree<K,V>::del(Node<K,V> *n){
    return this->del(n->key);
}

template<typename K, typename V>
Node<K,V>* BinaryTree<K,V>::_inorder(){
    //Under DEV TODO
    // pthread_mutex_lock(&lock);
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
        // pthread_mutex_unlock(&lock);
        return NULL;
    }

    Node<K,V>* n = stk.top();
    stk.pop();
    Node<K,V>* n1 = n -> r;
    while(n1 != NULL){
        stk.push(n1);
        n1 = n1 -> l;
    }
    // pthread_mutex_unlock(&lock);
    return n; 
}

template<typename K, typename V>
int BinaryTree<K,V>::dump(FILE *f){
    /* Dumps the tree content in a file */
    // pthread_mutex_lock(&lock);
    traversal_started = 1;
    while(!stk.empty()) stk.pop();
    Node<K,V>* n = root;
    while(n != NULL){
        stk.push(n);
        n = n -> l;
    }

    //int num_entries = BLK_SIZE / (sz_k + sz_v);

    // get time
    char *time_str = new char[sz_k + sz_v];
    for(int i=0;i<sz_k + sz_v;i++) time_str[i] = '0';
    time_t now = time(0);
    tm tstruct = *localtime(&now);
    tstruct = *localtime(&now);
    strftime(time_str, 20, TIME_FMT, &tstruct);
    time_str[19] = '0';
    errlog("BinaryTree::dump: Check the time: %s\n", time_str);
    fwrite(time_str, 1, sz_k + sz_v, f);

    while(!stk.empty()){
        n = stk.top();
        stk.pop();
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
    Node<K,V> dummy;
    log_action(dummy.key, dummy.value, _sync_);
    errlog("BinaryTree::dump: freeing time_str here\n");

    delete[] time_str;
    errlog("BinaryTree::dump: time_str freed successfully here\n");
    // pthread_mutex_unlock(&lock);
    return 0;
}

template<typename K, typename V>
void BinaryTree<K,V>::_free(){
    // pthread_mutex_lock(&lock);
    traversal_started = 1;
    while(!stk.empty()) stk.pop();
    Node<K,V>* n = root;
    if(n != NULL) stk.push(n);
    errlog("BinaryTree::_free: Size of tree: %d, stk not empty\n", size);
    while(!stk.empty()){
        n = stk.top();
        stk.pop();
        if(n -> l != NULL) stk.push(n -> l);
        if(n -> r != NULL) stk.push(n -> r);
        errlog("BinaryTree::_free: Freeing Node here\n");
        delete n;
    }
    root = NULL;
    size = 0;
    traversal_started = 0;
    // pthread_mutex_unlock(&lock);
    return;
}

#endif