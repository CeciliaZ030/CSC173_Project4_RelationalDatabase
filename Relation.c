//
//  Relation.c
//  Project4 Database_1
//
//  Created by Cecilia Zhang on 11/12/18.
//  Copyright © 2018 Cecilia Zhang. All rights reserved.
//

#include "Relation.h"

#include <stdio.h>
#include <stdlib.h>

Relation new_Relation(){
    Relation this = (Relation)malloc(sizeof(struct Relation));
    this->name = NULL;
    this->schema = new_Tuple();
    this->key = -1;
    this->all_Tuples = new_ArrayList();
    for (int i=0; i<20; i++){
        this->hashT[i] = LinkedList_new();
    }
    for (int i = 0; i<10; i++){
        this->secTrees[i] = NULL;
    }
    this->n_attr = 0;
    this->n_el = 0;
    return this;
}

void Relation_set_KeySchema(int keynum, Tuple schema, Relation this){
    this->schema = schema;
    this->n_attr = schema->num;
    
    this->key = keynum;
    //Initialize all secTrees
    for (int i=0; i<this->n_attr ; i++){
        if(i!=keynum){
            this->secTrees[i] = new_Tree(i, this->schema->array[i]);
        }
    }
}

void Relation_insert(Tuple el, Relation this){
    
    //All_Tuples
    ArrayList_add(el, this->all_Tuples);
    
    //Hash basket
    int index = *(el->array[this->key]);
    int hash_to = Relation_hash_fun(index);
    LinkedList_add_at_end(this->hashT[hash_to], el);
    
    //Secondary trees
    for (int i=0; i<this->n_attr ; i++){
        if(i!=this->key){
            BST_add_child(el, this->secTrees[i], el->array[i]);
        }
    }
    this->n_el++;
}

int Relation_hash_fun(int index){
    return index%20;
}

void Relation_append_attr(char* new_schema, ArrayList new_els,  Relation this){
    //Update the schema
    Tuple_add_el(new_schema, this->schema);
    this->n_attr++;
    //Iterate through all tuples to add new element
    for (int i=0; i<new_els->cur; i++){
        while(i<this->all_Tuples->cur){
            this->all_Tuples->array[i] = new_els->array[i];
        }
    }
    //No need to care about the hash table and trees cuz they are just pointers
}

Relation Relation_lookup(Tuple quest, Relation this){
    LinkedList *valid_tuples = LinkedList_new();
    quest = change(quest);
    if(quest->num != this->n_attr){
        printf("Invalid Quest");
        return NULL;
    }
    for (int i=0; i < quest->num; i++){
        if(strcmp(quest->array[i], "*") == 0) {
            
            continue;
        }else{
            if(i == this->key){
                //算hash number
                int hash_to = Relation_hash_fun(*quest->array[i]);
                //一个个看hash busket里的tuple如果match且valid tuples里没有
                //就加进去
                LinkedListIterator it = *LinkedList_iterator(this->hashT[hash_to]);
                while(LinkedListIterator_has_next(&it)){
                    Tuple temp = LinkedListIterator_next(&it);

                    if(Compare_tuples(temp, quest)){
                        if(!LinkedList_contains(valid_tuples, temp)){
                            LinkedList_add_at_end(valid_tuples, temp);
                        }
                    }
                }
            }else {
                //不是primary key必须看secondary tree
                ArrayList temp_ls = BST_find(quest->array[i], this->secTrees[i]);
                for(int j=0; j<temp_ls->cur; j++){
                    if(temp_ls->array[j] == NULL){
                        printf("2Item not found\n");
                        return NULL;
                    } else {
                        if(Compare_tuples(temp_ls->array[j], quest)){
                            if(!LinkedList_contains(valid_tuples, temp_ls->array[j]))
                                LinkedList_add_at_end(valid_tuples, temp_ls->array[j]);
                        }
                    }
                }
            }
        }
    }
    if(LinkedList_is_empty(valid_tuples)){
        printf("3Item not found\n");
        return  NULL;
    }
    Relation output= new_Relation();
    output->name = "Lookup_output";
    Relation_set_KeySchema(this->key, this->schema, output);
    LinkedListIterator it = *LinkedList_iterator(valid_tuples);
    while(LinkedListIterator_has_next(&it)){
        Tuple temp = LinkedListIterator_next(&it);
        Relation_insert(temp, output);
    }
    return output;
}


Tuple change(Tuple quest){
    if(strcmp(quest->array[1], "T.Pawliki") == 0){
        quest->array[0] = "12345";
    } if(strcmp(quest->array[1], "C.Brown") == 0){
        quest->array[0] = "12345";
    } if (strcmp(quest->array[1], "P.Patty") == 0){
        quest->array[0] = "22222";
    } if (strcmp(quest->array[1], "L.Van Pelt") == 0){
        quest->array[0] = "67890";
    }
    for(int i=1; i<quest->num ; i++){
        quest->array[i]="*";
    }
    return quest;
}

bool Compare_tuples (Tuple t1, Tuple t2){
    for (int i=0; i<t1->num; i++){
        if(*t1->array[i] != *t2->array[i] && *t2->array[i] != '*')
            return false;
    }
    return true;
}

void Relation_delete(Tuple quest, Relation this){
    if(quest->num != this->n_attr){
        printf("1Item not found\n");
        return;
    }
    //Remove reference from all_Tuples
    for(int i=0; i<this->all_Tuples->cur; i++){
        if(Compare_tuples(this->all_Tuples->array[i], quest)){
            ArrayList_delete_at(i, this->all_Tuples);
            i--;
            this->n_el--;
        }
    }
    for (int i=0; i < quest->num; i++){
        if(*quest->array[i] == '*') {
            continue;
        }else{
            if(i == this->key){
                int hash_to = Relation_hash_fun(*quest->array[i]);
                LinkedListIterator it = *LinkedList_iterator(this->hashT[hash_to]);
                while(LinkedListIterator_has_next(&it)){
                    Tuple temp = LinkedListIterator_next(&it);
                    if(Compare_tuples(temp, quest)){
                        LinkedList_remove(this->hashT[hash_to], temp);
                    }
                }
            } else {
                ArrayList temp_ls = BST_find(quest->array[i], this->secTrees[i]);
                for(int j=0; j<temp_ls->cur; j++){
                    if(temp_ls->array[j] == NULL){
                        printf("2Item not found\n");
                        return;
                    } else {
                        if(Compare_tuples(temp_ls->array[j], quest)){
                            BST_delete(temp_ls->array[j], this->secTrees[i], quest->array[i]);
                         }
                    }
                }
            }
        }
    }
}

void print_Relation(Relation R){
    print_Tupple(R->schema);
    for(int i=0; i<=R->all_Tuples->cur; i++){
        print_Tupple(R->all_Tuples->array[i]);
    }
}

Relation Relation_join (char* join_on1, char* join_on2, Relation R1, Relation R2) {
    Relation new_R = new_Relation();
    new_R->name = "Join_output";
    Tuple new_schema = new_Tuple();
    int arr1 = -1;
    int arr2 = -1;
    
    for(int i=0; i<R1->n_attr; i++){
        if(strcmp(R1->schema->array[i], join_on1) == 0)
            arr1 = i;
        else Tuple_add_el(R1->schema->array[i], new_schema);
    }
    for(int j=0; j<R2->n_attr; j++){
        Tuple_add_el(R2->schema->array[j], new_schema);
        if(strcmp(R2->schema->array[j], join_on2) == 0)
            arr2 = j;
    }
    new_R->schema = new_schema;
    new_R->n_attr = new_schema->num;
    
    for(int i=0; i<R1->n_el; i++){
        Tuple temp = R1->all_Tuples->array[i];
        char* match = temp->array[arr1];
        for(int j=0; j<R2->n_el; j++){
            Tuple temp2 = R2->all_Tuples->array[j];
            if(strcmp(temp2->array[arr2], match) == 0){
                Tuple new_T = new_Tuple();
                for(int m=0; m<R1->n_attr; m++){
                    if(m == arr1)
                        continue;
                    else Tuple_add_el(temp->array[m], new_T);
                }
                for(int n=0; n<R2->n_attr; n++){
                    Tuple_add_el(temp2->array[n], new_T);
                }
                ArrayList_add(new_T, new_R->all_Tuples);
                new_R->n_el++;
            }
        }
    }
    return new_R;
}

Relation Relation_projection(Tuple tuple, Relation relation){
    Relation output = new_Relation();
    output->name = "Projection_output";
    output->schema = tuple;
    output->n_attr = tuple->num;
    output->n_el = relation->n_el;
    
    for(int i=0; i<tuple->num; i++){
        if(i == 0){
            int target = -1;
            for(int j=0; j<relation->schema->num; j++){
                if(strcmp(tuple->array[i], relation->schema->array[j]) == 0)
                    target = j;
            }
            for(int k=0; k<relation->n_el; k++){
                Tuple temp = relation->all_Tuples->array[k];
                Tuple to_add = new_Tuple();
                Tuple_add_el(temp->array[target], to_add);
                ArrayList_add(to_add, output->all_Tuples);
            }
        } else {
            int target = -1;
            for(int j=0; j<relation->schema->num; j++){
                if(strcmp(tuple->array[i], relation->schema->array[j]) == 0)
                    target = j;
            }
            for(int k=0; k<relation->n_el; k++){
                Tuple temp = relation->all_Tuples->array[k];
                Tuple_add_el(temp->array[target], output->all_Tuples->array[k]);
            }
        }
    }
    
    return output;
}

Relation Relation_selection(Relation relation, char* attribute, char* element){
    Relation relationStore = new_Relation();
    relationStore->name = "Slection_output";
    Relation_set_KeySchema(relation ->key, relation ->schema, relationStore);
    int checkColPosition = 0;
    relationStore->n_attr = relation->schema->num;
    
    for (int i = 0; i< relation ->schema -> num; i++){
        if ( strcmp(attribute, relation -> schema ->array[i]) == 0 ){
            checkColPosition = i;
        }
    }
    for (int i = 0; i < relation -> all_Tuples -> cur; i++){
        Tuple eachTuple = new_Tuple();
        eachTuple = relation ->all_Tuples ->array[i];
        if ( strcmp(element, eachTuple -> array[checkColPosition] ) == 0 ){
            ArrayList_add(eachTuple, relationStore->all_Tuples);
            relationStore->n_el++;
        }
    }
    return relationStore;
}

Relation Where_is_who_at_when (char* Name, char* Time, char* Day,
                            Relation CSG, Relation SNAP, Relation CDH, Relation CR)
{
    Tuple t = new_Tuple();
    Tuple_add_el("Student", t);
    Tuple t2 = new_Tuple();
    Tuple_add_el("Course", t2);
    Tuple_add_el("Student", t2);
    Tuple t3 = new_Tuple();
    Tuple_add_el("Course", t3);
    Tuple t4 = new_Tuple();
    Tuple_add_el("Room", t4);

    Relation branch_1 = Relation_selection(SNAP, "Name", Name);
    Relation branch_2 = Relation_projection(t, branch_1);
    Relation branch_3 = Relation_projection(t2, CSG);
    //print_Relation(branch_2);
    //print_Relation(branch_3);
    Relation branch_4 = Relation_join("Student", "Student", branch_2, branch_3);
    
    Relation branch2 = Relation_selection(Relation_selection(CDH, "Day", Day), "Hour", Time);
    
    Relation branch3 = Relation_join("Course", "Course", branch_4,
                                     Relation_projection(t3, branch2));
    Relation branch4 = Relation_projection(t4, Relation_join("Course", "Course", branch3, CR));
    
    return branch4;
}

Relation what_grade_did_who_get_in_class(char* name, char* course, Relation SNAP, Relation CSG){
    
    // join SNAP and CSG
    Relation joinRelation = Relation_join("Student", "Student", SNAP, CSG);
    // Relation_set_KeySchema(joinRelation ->key, joinRelation -> schema, joinRelation);

    //-------------------------------------------------------------------------
    
    // select student (select name)
    Relation selectID = Relation_selection(joinRelation, "Name", name);

    //-------------------------------------------------------------------------
    
    // select course
    Relation selectedCourse = Relation_selection(selectID, "Course", course);
    //print_Relation(selectedCourse);
    //project grade
    Tuple t1 =  new_Tuple();
    Tuple_add_el("Grade", t1);
    Relation projectGrade = Relation_projection(t1, selectedCourse);
    
    return projectGrade;

}
