//
//  ArrayList.c
//  Project4 Database_1
//
//  Created by Cecilia Zhang on 11/12/18.
//  Copyright © 2018 Cecilia Zhang. All rights reserved.
//

#include "ArrayList.h"

#include <stdio.h>
#include <stdlib.h>


ArrayList new_ArrayList(){
    ArrayList this = (ArrayList)malloc(sizeof(struct ArrayList));
    for (int i = 0; i < 100; i++){
        this->array[i] = NULL;
    }
    this->cur = 0;
    return this;
}

void ArrayList_add(void* data, ArrayList this){
    this->array[this->cur] = data;
    this->cur++;
}

void ArrayList_delete_at(int index, ArrayList this){
    this->array[index] = NULL;
    for(int i=index; i<100-1; i++){
        this->array[i] = this->array[i+1];
    }
    this->cur--;
}

void* ArrayList_find(void* data, ArrayList this){
    for(int i=0; i<100; i++){
        if(this->array[i] == data){
            return this->array[i];
        }
    }
    return NULL;
}

//void ArrayList_print(ArrayList this){
//    for(int i=0; i<100; i++){
//        if(this->array[i] != NULL){
//            printf("%s  ", this->array[i]);
//        }
//    }
//    printf("\n");
//}
