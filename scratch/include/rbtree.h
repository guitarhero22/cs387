#include<stdio.h>
#include<vector>
#include<stack>

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
    */
    public:
        Node<K,V>* root; //Pointer to the root
        int size = 0; //Number of Nodes
        stack<Node<K,V>*> stk; // for inorder traversal
        int traversal_started = 0;

        // Constructors ******************
        BinaryTree(){}
        BinaryTree(Node<K,V> n){ //class constructor
            this->root = new Node<K,V>(); 
            *this->root = n;
        }
        BinaryTree(K k, V v){ //class constructor
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

        
        int dump(FILE* f){return 0;}//dumps the tree in the given file and empties the tree
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

#include "rbtree.cpp"

#endif
