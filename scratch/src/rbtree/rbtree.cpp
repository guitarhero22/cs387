#include "rbtree.h"

#ifndef INCLUDE_BTREESRC
#define INCLUDE_BTRESRC

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

#endif