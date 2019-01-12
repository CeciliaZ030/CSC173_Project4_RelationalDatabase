//
//  Relation.h
//  Project4 Database_1
//
//  Created by Cecilia Zhang on 11/12/18.
//  Copyright © 2018 Cecilia Zhang. All rights reserved.
//

#ifndef Relation_h
#define Relation_h

#include "BST_Tree.h"
#include "Tuple.h"
#include "LinkedList.h"

#include <stdio.h>
#include <string.h>


struct Relation {
    char* name;
    Tuple schema;
    int key;
    ArrayList all_Tuples;
    Tree secTrees[10];
    LinkedList *hashT[20];
    int n_attr;
    int n_el;
};


typedef struct Relation* Relation;

Relation new_Relation(void);

void Relation_set_KeySchema(int keynum, Tuple schema, Relation this);

int Relation_hash_fun(int index);

void Relation_insert(Tuple el, Relation this);

void Relation_append_attr(char* new_schema, ArrayList new_els,  Relation this);

Relation Relation_lookup(Tuple quest, Relation this);

bool Compare_tuples (Tuple t1, Tuple t2);

Tuple change(Tuple quest);

void Relation_delete(Tuple quest, Relation this);

void print_Relation(Relation R);

Relation Relation_join (char* join_on1, char* join_on2, Relation R1, Relation R2);

Relation Relation_projection(Tuple tuple, Relation relation);

Relation Relation_selection(Relation relation, char* attribute, char* element);

Relation Where_is_who_at_when (char* Name, char* Tiem, char* Day, Relation CSG, Relation SNAP, Relation CDH, Relation CR);

Relation what_grade_did_who_get_in_class(char* name, char* course, Relation SNAP, Relation CSG);

#endif /* Relation_h */
