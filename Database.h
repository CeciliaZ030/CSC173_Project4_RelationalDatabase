//
//  Database.h
//  CSC173PJT4
//
//  Created by Cecilia Zhang on 11/23/18.
//  Copyright © 2018 Junkang Gu. All rights reserved.
//

#ifndef Database_h
#define Database_h

#include <stdio.h>

#include "Relation.h"

#endif /* Database_h */

struct Database{
    ArrayList relationList;
};

typedef struct Database* Database;

Database new_Database(void);

void saveFile(Database database, char* filename);

Database readFile(char* filename);

int stringToInt(char* string);

Tuple stringToTuple(char* tuple);

void removeNewLine2(char* i);

char* removeNewLine(char* i);

char* readLine(FILE * file);

