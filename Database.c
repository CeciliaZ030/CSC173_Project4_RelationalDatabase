//
//  Database.c
//  CSC173PJT4
//
//  Created by Cecilia Zhang on 11/23/18.
//  Copyright © 2018 Junkang Gu. All rights reserved.
//

#include "Database.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Database new_Database(){
    Database this = (Database)malloc(sizeof(struct Database));
    this->relationList=new_ArrayList();
    return this;
}

int stringToInt(char* string){
    int output=0;
    long length=strlen(string);
    for(int i=0; i<length;i++){
        output=output*10+(string[i]-'0');
    }
    return output;
}

Tuple stringToTuple(char* tuple){
    Tuple output=new_Tuple();
    char * string = strtok(tuple, ";");
    while (string != NULL)
    {
        Tuple_add_el(string, output);
        string = strtok(NULL, ";");
    }
    return output;
}

void removeNewLine2(char* i){
    long length=strlen(i);
    if(i[length-1]=='\n'){
        i[length-1]='\0';
    }
}
char* removeNewLine(char* i){
    long length=strlen(i);
    if(i[length-1]=='\n'){
        i[length-1]='\0';
    }
    return i;
}

char* readLine(FILE * file){
    char line[256];
    char* output=removeNewLine(fgets(line, sizeof(line), file));
    char * copy = malloc(strlen(output) + 1);
    strcpy(copy, output);
    if(copy==NULL) return readLine(file);
    return copy;
}

Database readFile(char* filename){
    Database db=new_Database();
    FILE* file;
    file=fopen(filename, "r");
    if(file==NULL){
        printf("File not found\n");
    }else{
   
        //IGNORE TITLE
	readLine(file);
        //NUMBER OF RELATIONS
        char* tmp=readLine(file);
        int numRelation=stringToInt(tmp);
        
        for( int i=0; i<numRelation;i++){//create a relation
            Relation tmp=new_Relation();
            //IGNORE SPACE
            readLine(file);
            //NAME
            tmp->name=readLine(file);
            //IGNORE KEY TITLE
            readLine(file);
            //KEY INDEX
            tmp->key=stringToInt(readLine(file));
            
            //IGNORE TITLE
            readLine(file);
            //NUM OF TUTPLES
            int numofTuple=stringToInt(readLine(file));
            //READ SCHEMA
            char* schema=readLine(file);
            tmp->schema=stringToTuple(schema);
            tmp->n_attr = tmp->schema->num;
            Relation_set_KeySchema(tmp->key, tmp->schema, tmp);
            
            //INSERT TUPLES
            for( int j=0; j<numofTuple; j++){//create a tuple
                char* tuple=readLine(file);
                Relation_insert(stringToTuple(tuple), tmp);
            }
            ArrayList_add(tmp, db->relationList);
        }
    }
    fclose(file);
    return db;
    
}

void saveFile(Database database, char* filename){
    FILE * file=NULL;
    file=fopen(filename, "r");
    if(file==NULL){
        file=fopen(filename, "w+");
    }else{
        remove(filename);
        file=fopen(filename, "w+");
    }
    int numRelations=database->relationList->cur;
    fprintf(file,"Num of Relations\n%d\n",numRelations);//Number of relations
    for(int i=0; i<numRelations;i++){//write a relation
        fprintf(file,"\n");
        Relation r= database->relationList->array[i];
        fprintf(file, "%s\n",r->name);
        fprintf(file,"Primary Key\n");
        fprintf(file, "%d\n",r->key);
        fprintf(file,"Num of tuples\n");
        fprintf(file, "%d\n",r->all_Tuples->cur);
        for(int k=0; k<r->schema->num;k++){//write the schema
            fprintf(file, "%s",r->schema->array[k]);
            if(k!=r->schema->num-1) fprintf(file, ";"); else fprintf(file, "\n");
        }
        int numTuples=r->all_Tuples->cur;
        for(int j=0; j<numTuples;j++){//write all tuples
            Tuple t=r->all_Tuples->array[j];
            for(int l=0; l<r->schema->num; l++){//write a tuple
                fprintf(file, "%s",t->array[l]);
                if(l!=r->schema->num-1) fprintf(file, ";"); else fprintf(file, "\n");
            }
        }
    }
    fclose(file);
}

