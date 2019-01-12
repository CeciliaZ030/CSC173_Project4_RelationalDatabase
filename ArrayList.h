//
//  ArrayList.h
//  Project4 Database_1
//
//  Created by Cecilia Zhang on 11/12/18.
//  Copyright © 2018 Cecilia Zhang. All rights reserved.
//

#ifndef ArrayList_h
#define ArrayList_h

#include <stdio.h>

#endif /* ArrayList_h */

struct ArrayList {
    void* array[100];
    int cur;
};

typedef struct ArrayList* ArrayList;

ArrayList new_ArrayList(void);

void ArrayList_add(void* data, ArrayList this);

void ArrayList_delete_at(int index, ArrayList this);

void* ArrayList_find(void* data, ArrayList this);

//void ArrayList_print(ArrayList this);
