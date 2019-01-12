//
//  Tuple.c
//  Project4 Database_1
//
//  Created by Cecilia Zhang on 11/12/18.
//  Copyright © 2018 Cecilia Zhang. All rights reserved.
//

#include "Tuple.h"

#include <stdio.h>
#include <stdlib.h>


Tuple new_Tuple(){
    Tuple this = (Tuple)malloc(sizeof(struct Tuple));
    this->num = 0;
    return this;
}

void Tuple_free(Tuple this){
    if (this == NULL)
        return;
    else {
        for(int i=0; i<10; i++){
            this->array[i] = NULL;
        }
    }
    free(this);
}

//Element is added at the end
void Tuple_add_el(char* el, Tuple this){
    this->array[this->num] = el;
    this->num++;
}

void Tuple_remove_el(int index, Tuple this){
    this->array[index] = NULL;
    for (int i = index; i<this->num-1; i++){
        this->array[i]=this->array[i+1];
    }
    this->array[this->num-1] = NULL;
    this->num--;
}

//void Tuple_add_all(char* els[], Tuple this, int n){
//    for(int i=0; i<n; i++){
//        this->array[i] = els[i];
//        this->num++;
//    }
//}

void print_Tupple (Tuple this) {
    if(this==NULL)return;
    for(int i=0; i<this->num; i++){
        printf("%s\t", this->array[i]);
    }
    printf("\n");
}

