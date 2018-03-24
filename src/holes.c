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
#define TRUE 1
#define FALSE 0
#define NUMBER_RANGE( var ) (var >= 48 && var <= 57)
#define debug if(TRUE)printf
#define memSize 128

typedef struct{
    char* arrayOfData[256];
    int numOfArray;
    char fileName[256];
}Holes;

typedef struct{
    char name[256];
    int size;
    int isInserted;
    int insertPosition;
}Process;

Holes* newHoles(){
    Holes* new = malloc(sizeof(Holes));
    new->arrayOfData[0] = '\0';
    new->numOfArray = 0;
}//end constructor

Process* newProcess(){
    Process* new = malloc(sizeof(Process));
    new->name[0] = '\0'
    new->size = 0;
    new->isInserted = 0;
    nwe->insertPosition = 0;
}//end constructor

void loadData(Hole* hole){
    //open file and check if there is error
    FILE* filePointer = fopen(hole->fileName, "r");
    if(filePointer == NULL){
        printf("Error, file is NULL\n");
        printf("Please input a valid file\n");
        return 0;
    }//end if

    //dec vars
    char line[256] = {"\0"};
    int sizeOfArray = 1;
    char** arrayOfString = calloc(sizeOfArray, sizeof(char));
    int x = 0;

    //read each line in the file
    printf("Loading data...\n");
    while(fgets(line, sizeof(line), filePointer) != NULL){
        strcpy(arrayOfString[x], line);
        debug(arrayOfString[x]);
        x++;
        arrayOfString = realloc(arrayOfString, sizeof(arrayOfString)*x+1);
    }//end while
    printf("Data loaded\n");
    debug("numOfArray = %d\n", x);
    Holes->numOfArray = x;
    Holes->arrayOfData = arrayOfString;
    fclose(filePointer);
}//end func

int main(int argc, char** argv){
    debug("argc = %d\n", argc);
    if(argc != 1){
        printf("Invalid argument");
        return 0;
    }//end if
    char* fileName[256];
    strcpy(fileName, argv[1]);
    debug("fileName = %s\n", fileName);

    return 0;
}//end main