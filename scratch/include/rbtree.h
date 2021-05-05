#include<stdio.h>
#include<vector>
#define _temp_ template<typename T>

#ifndef INCLUDE_RBTREE
#define INCLUDE_RBTREE

_temp_
class Node{
    public:
        T data;
        Node<T> *p;
        vector<Node*> children;
};

_temp_
class BinaryTree{
    public:
        Node<T>* search(T);
        Node<T>* insert(T);
        int del(T);
        int del(Node<T>*);
};

_temp_
class  RBTree{
    public:
        Node<T>* search(T);
        Node<T>* insert(T);
        int del(T t);
        int del(Node<T>*)
};

#endif