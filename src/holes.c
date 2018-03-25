/********************************************
 * Vicky Mohammad
 * 0895381
 * March 22, 2018
 ********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "LinkedListAPI.h"
#define TRUE 1
#define FALSE 0
#define NUMBER_RANGE( var ) (var >= 48 && var <= 57)
#define debug if(TRUE)printf
#define memSize 128

typedef struct{
    char** arrayOfData;
    int numOfArray;
    char fileName[256];
}Hole;

typedef struct{
    char name[256];
    int size;
    int isInserted;
    int insertPosition;
}Process;

Hole* newHole(){
    Hole* new = malloc(sizeof(Hole));
    new->arrayOfData = NULL;
    new->numOfArray = 0;
    return new;
}//end constructor

Process* newProcess(){
    Process* new = malloc(sizeof(Process));
    new->name[0] = '\0';
    new->size = 0;
    new->isInserted = 0;
    new->insertPosition = 0;
    return new;
}//end constructor

char* setString(char* string){
    char* newString = calloc(strlen(string)+99, sizeof(char));
    strcpy(newString, string);
    return newString;
}//end func

void loadData(Hole* hole){
    //open file and check if there is error
    FILE* filePointer = fopen(hole->fileName, "r");
    if(filePointer == NULL){
        printf("Invalid file, exiting program\n");
        exit(0);
    }//end if

    //dec vars
    char line[256] = {"\0"};
    int sizeOfArray = 1;
    char** arrayOfString = calloc(sizeOfArray, sizeof(char));
    int x = 0;

    //read each line in the file
    printf("Loading data...\n");
    while(fgets(line, sizeof(line), filePointer) != NULL){
        line[strcspn(line, "\n\r")] = '\0';
        arrayOfString[x] = setString(line);
        debug("arrayOfString %d = %s\n", x, arrayOfString[x]);
        x++;
        arrayOfString = realloc(arrayOfString, sizeof(arrayOfString)*x+1);
    }//end while
    printf("\nData loaded\n");
    debug("numOfArray = %d\n", x);

    //assign the to the hole
    hole->numOfArray = x;
    hole->arrayOfData = arrayOfString;
    fclose(filePointer);
}//end func

char** split(char* stringToBeSplit, const char* delimiter){
    //dec vars
    char** arrayOfToken = calloc(2, sizeof(arrayOfToken));
    int x = 0;
    char* token = strtok(stringToBeSplit, delimiter);
    //create a split func that will used to parse the data
    while(token != NULL){
        arrayOfToken[x] = setString(token);
        // debug("arrayOfToken %d = %s\n", x, arrayOfToken[x]);
        x++;
        arrayOfToken[x] = NULL;
        arrayOfToken = realloc(arrayOfToken, sizeof(arrayOfToken)*x+1);
        token = strtok(NULL, delimiter);
    }//end while
    return arrayOfToken;
}//end func

int main(int argc, char** argv){
    //error check the num of argument
    debug("argc = %d\n", argc);
    if(argc != 2){
        printf("Invalid argument\n");
        return 0;
    }//end if

    //dec vars and set
    Hole* hole = newHole();
    debug("fileName = %s\n", argv[1]);
    strcpy(hole->fileName, argv[1]);
    loadData(hole);

    //debug
    for(int x=0; x<hole->numOfArray; x++){
        debug("debug data = (%s)\n", hole->arrayOfData[x]);
    }//end for
    debug("\n");

    //create 4 queues
    List* q1 = initializeListPointer(dummyPrint, dummyDelete, dummyCompare);
    List* q2 = initializeListPointer(dummyPrint, dummyDelete, dummyCompare);
    List* q3 = initializeListPointer(dummyPrint, dummyDelete, dummyCompare);
    List* q4 = initializeListPointer(dummyPrint, dummyDelete, dummyCompare);

    for(int x=0; x<hole->numOfArray; x++){
        char** token = split(hole->arrayOfData[x], " ");
        debug("debug: name = (%s), size = (%s)\n", token[0], token[1]);
    }//end for
    debug("\n");

    

    return 0;
}//end main