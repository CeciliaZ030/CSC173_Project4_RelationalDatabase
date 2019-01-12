//
//  BST Tree.c
//  Project4 Database_1
//
//  Created by Cecilia Zhang on 11/14/18.
//  Copyright © 2018 Cecilia Zhang. All rights reserved.
//

#include "BST_Tree.h"

#include <stdio.h>
#include <stdlib.h>

struct Tree {
    Node root;
    int key_attribute;
};

struct Node {
    void* data;
    char* key;
    Node lc;
    Node rc;
    Node p;
};

Node new_Node(void* data, char* key){
    Node this = (Node)malloc(sizeof(struct Node));
    this->data = data;
    this->key = key;
    this->lc = NULL;
    this->rc = NULL;
    this->p=NULL;
    return this;
}

Tree new_Tree(int key_attribute, char* name){
    Tree this = (Tree)malloc(sizeof(struct Tree));
    this->root = new_Node(NULL, name);
    this->key_attribute = key_attribute;
    return this;
}

void BST_set_root(Tree this, void* data, char* key){
    this->root = new_Node(data, key);
}

void BST_add_child(void* data, Tree this, char* key){
    Node new = new_Node(data, key);
    BST_insert_help(this->root, new);
}

void BST_insert_help(Node this, Node new){
    if(new->key > this->key){
        if(this->rc == NULL)
        {this->rc = new; new->p=this;}
        else
            BST_insert_help(this->rc, new);
    }if(new->key <= this->key){
        if(this->lc == NULL)
        {this->lc = new;new->p=this;}
        else
            BST_insert_help(this->lc, new);
    }else{
        this = new;
    }
}

ArrayList BST_find(char* key, Tree this){
    ArrayList found = new_ArrayList();
    BST_find_help(key, this->root, found);
    return found;
}

void BST_find_help(char* key, Node this, ArrayList found){
    if(key > this->key)
        BST_find_help(key, this->rc, found);
    if(key < this->key){
        BST_find_help(key, this->lc, found);
    }
    else if(this->key == key){
        ArrayList_add(this->data, found);
        if(this->lc == NULL){
            return;
        }
        else{
            BST_find_help(key, this->lc, found);
        }
    }
}

void BST_delete(void* data, Tree this, char* key){
    BST_delete_help(data, this->root, key);
}

char* BSTminValue(Node node){
    char* min = node->key;
    while (node->lc != NULL)
    {
        min = node->lc->key;
        node=node->lc;
    }
    return min;
}

Node BST_delete_help(void* data, Node this, char* key){
    if(this==NULL) return this;
    if (key < this->key)
        this->lc = BST_delete_help(data, this->lc, key);
    else if (key > this->key)
        this->rc = BST_delete_help(data, this->rc, key);
    else
    {
        if (this->lc == NULL)
            return this->rc;
        else if (this->rc==NULL)
            return this->lc;
        this->key = BSTminValue(this->rc);
        this->rc=BST_delete_help(data, this->rc, key);
    }
    return this;
}
