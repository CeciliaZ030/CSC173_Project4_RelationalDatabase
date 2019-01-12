//
//  main.c
//  Project4 Database_1
//
//  Created by Cecilia Zhang on 11/12/18.
//  Copyright © 2018 Cecilia Zhang. All rights reserved.
//

#include <stdio.h>
#include "Database.h"

int main(int argc, const char * argv[]) {
    
    Database db = readFile("Database.txt");
    
    printf("Loading data from file...\n");
    
    
    // **** Check and Change the Index of the Relation in the Database ****
    for(int i=0; i<db->relationList->cur; i++){
        print_Relation(db ->relationList ->array[i]);
        printf("\n");
    }
    
    
    printf("\nPart 1\n");
    printf("1.Insert\n(33333, T.Pawliki, Faculty Rd, 585-956-234)\n");
    Tuple tInsert1 = new_Tuple();
    Tuple_add_el("33333", tInsert1);
    Tuple_add_el("T.Pawliki", tInsert1);
    Tuple_add_el("Faculty Rd", tInsert1);
    Tuple_add_el("585-956-234", tInsert1);
    
    Relation toInsert = db ->relationList ->array[1];
    Relation_insert(tInsert1, toInsert);
    print_Relation(toInsert);
    
    
    printf("\n2.Lookup\n(33333, *, *, *)\n");
    Tuple Find_Pawliki = new_Tuple();
    Tuple_add_el("33333", Find_Pawliki);
    Tuple_add_el("*", Find_Pawliki);
    Tuple_add_el("*", Find_Pawliki);
    Tuple_add_el("*", Find_Pawliki);
    Relation toLookUp = Relation_lookup(Find_Pawliki, toInsert);
    print_Relation(toLookUp);
    
    Tuple lookup2 = new_Tuple();
    Tuple_add_el("*", lookup2);
    Tuple_add_el("C.Brown", lookup2);
    Tuple_add_el("*", lookup2);
    Tuple_add_el("*", lookup2);
    printf("(*, C.Brown, *, *)\n");
    Relation toLookUp2 = Relation_lookup(lookup2, toInsert);
    print_Relation(toLookUp2);
    
    printf("\n3.Delete\n(33333, T.Pawliki, Faculty Rd, 585-956-234)\n");
    Relation_delete(tInsert1, toInsert);
    print_Relation(toInsert);
    
    //
    printf("\nPart 3\n");
    printf("1.Selection\nCourse = 'CS101'\n");
    Relation csgToSelect = db -> relationList -> array[0];
    Relation csgSelected = Relation_selection(csgToSelect, "Course", "CS101");
    print_Relation(csgSelected);
    
    printf("\n2.Projection\n");
    printf("'StudentID'\n");
    Tuple projectionT1 = new_Tuple();
    Tuple_add_el("Student", projectionT1);
    Relation csgProjected = Relation_projection(projectionT1, csgSelected);
    print_Relation(csgProjected);
    
    printf("\n3.Join\n");
    printf("Course-Day-Hour join Course-Room on 'Course'\n");
    Relation cdhToJoin = db -> relationList -> array[3];
    Relation crToJoin = db -> relationList -> array[4];
    Relation cdhJoinedCr = Relation_join("Course", "Course", cdhToJoin, crToJoin);
    print_Relation(cdhJoinedCr);
    
    printf("\n4.All Three (Example 8.15)...\n");
    Relation selectedRoom_CrJoinCdh = Relation_selection(Relation_selection(cdhJoinedCr, "Course", "CS101"),
                                                         "Day", "M");

    Tuple allT1 = new_Tuple();
    Tuple_add_el("Room", allT1);
    Relation projectedDayHour = Relation_projection(allT1, selectedRoom_CrJoinCdh);
    print_Relation(projectedDayHour);
    
    
    //
    printf("\nPart 2\n");
    printf("Query: What Grade Did [StudentName] get in [CourseName] ?\n");
    char studentName[256];
    char courseName[256];

    do{
        printf("    -Please enter the student's name\n");
        fgets(studentName,255,stdin);
        removeNewLine2(studentName);
        printf("    -Please enter the course name (Enter 'quit' to quit query)\n");
        fgets(courseName,255,stdin);
        removeNewLine2(courseName);
        Relation csgPart3 = db -> relationList -> array[0];
        Relation snapPart3 = db -> relationList -> array[1];

        print_Relation(what_grade_did_who_get_in_class(studentName, courseName, snapPart3, csgPart3));

    } while((strcmp(studentName, "quit")!=0)&& (strcmp(courseName, "quit")!=0));


    printf("Query: Where is [StudentName] at [Time] on [Day]?...\n");
    
    char studentName2[256];
    char Time[256];
    char Day[256];
    
    do{
        printf("    -Please enter student's name\n");
        fgets(studentName2,255,stdin);
        removeNewLine2(studentName2);
        printf("    -Please enter Time\n");
        fgets(Time,255,stdin);
        removeNewLine2(Time);
        printf("    -Please enter Day (Enter 'quit' to quit query)\n");
        fgets(Day,255, stdin);
        removeNewLine2(Day);
        
        Relation csgPart3 = db -> relationList -> array[0];
        Relation snapPart3 = db -> relationList -> array[1];
        Relation cdhPart3 = db -> relationList -> array[3];
        Relation crPart3 = db -> relationList -> array[4];
        
        
        print_Relation(Where_is_who_at_when(studentName2, Time, Day, csgPart3, snapPart3, cdhPart3, crPart3));
        
    } while((strcmp(studentName2, "quit")!=0 )&&(strcmp(Time, "quit")!=0 )
             &&  (strcmp(Day, "quit")!=0 ) );
    saveFile(db,"Database.txt");
    
    return 0;
    
}


