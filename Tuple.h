//
//  Tuple.h
//  Project4 Database_1
//
//  Created by Cecilia Zhang on 11/12/18.
//  Copyright © 2018 Cecilia Zhang. All rights reserved.
//

#ifndef Tuple_h
#define Tuple_h

#include <stdio.h>

#endif /* Tuple_h */


struct Tuple {
    char* array[10];
    int num;
};

typedef struct Tuple *Tuple;

Tuple new_Tuple(void);

void Tuple_free(Tuple this);

void Tuple_add_el(char* el, Tuple this);

void Tuple_remove_el(int index, Tuple this);

//void Tuple_add_all(char* els[], Tuple this, int n);

void print_Tupple (Tuple this);
