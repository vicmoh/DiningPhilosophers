/********************************************
 * Vicky Mohammad
 * 0895381
 * March 22, 2018
 ********************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "LinkedListAPI.h"
#define MEM_SIZE 128;
#define DEBUG_HELPER true
#define debug if(true)printf

/********************************************************
 * header
 ********************************************************/

typedef struct{
    char processID;
    int memoryUsage;
    int memorySwap;
}Process;

typedef struct{
    char** array;
    int arraySize;
    char* fileName;
}Hole;

Process* newProcess(){
    Process* new = malloc(sizeof(Process));
    new->processID = 0;
    new->memoryUsage = 0;
    new->memorySwap = 0;
    return new; 
}//end constructor

Hole* newHole(){
    Hole* new = malloc(sizeof(Hole));
    new->array = NULL;
    new->arraySize = 0;
    new->fileName = NULL;
    return new;
}//end constructor

/********************************************************
 * helper functions, some of them are from my old  class
 ********************************************************/

char* setString(char* string){
    char* newString = calloc(1 ,sizeof(char)*(strlen(string)+9));
    strcpy(newString, string);
    return newString;
}//end func

char** split(char* stringToBeSplit, const char* delimiter, int* arraySize){
    //create string array
    int numOfSplits = 0;
    char** stringArray = calloc(1 ,sizeof(stringArray)*(numOfSplits+1));
    //tokenize through thw code
    char *token;
    char* stringBuffer = setString(stringToBeSplit);
    token = strtok(stringBuffer, delimiter);
    //iterate through the tokem
    while(token != NULL) {
        //copy the split token
        stringArray[numOfSplits] = setString(token);
        //keep going
        token = strtok(NULL, delimiter);
        //realloc more memory
        numOfSplits++;
        stringArray = realloc(stringArray, sizeof(stringArray)*(numOfSplits+1));
    }//end if 
    //return
    *arraySize = numOfSplits;
    free(stringBuffer);
    return stringArray;
}//end func

char** readFileByLine(char* fileName, int* arraySize, const int lineSize){
    //dec var
    FILE* filePointer = fopen(fileName, "r");
    char line[lineSize];
    char** stringArray = calloc(1 ,sizeof(stringArray));
    int numberOfLines = 0;

    //if file doesnt exist
    if(filePointer == NULL) {
        if(DEBUG_HELPER)printf("No such file.\n");
        free(stringArray);
        return NULL;
    }//end if

    if(DEBUG_HELPER)printf("Loading string...\n");
    //scan until end of file
    //while (fgets(line, sizeof(line), filePointer) != NULL) {
    while (fgets(line, sizeof(line), filePointer) != NULL) {
        //copy string
        if(DEBUG_HELPER)printf("line %d: %s", numberOfLines, line);
        line[strcspn(line, "\r\n")] = '\0';
        stringArray[numberOfLines] = setString(line); 
        //debug("String Length: %d\n", (int)strlen(line));
        numberOfLines++;
        //create string array from each line
        stringArray = realloc(stringArray, sizeof(stringArray)*(numberOfLines+1));
        //go to the next
    }//end loop
    if(DEBUG_HELPER)printf("\nString loaded...\n");
    
    //return
    *arraySize = numberOfLines; 
    fclose(filePointer);
    return stringArray;
}//end func

void freeStringArray(char** array, int arraySize){
    for(int x = 0; x < arraySize; x++) {
        free(array[x]);
    }//end if
    free(array);
}//end func

/********************************************************
 * main
 ********************************************************/

int main(int argc, char** argv){
    //error check
    if(argc != 2){
        printf("Invalid number of arguments\n");
    }//end if

    //dec object hole
    Hole* hole = newHole();
    hole->fileName = setString(argv[1]);
    readFileByLine(hole->fileName, &hole->arraySize, 256);
    
    
    

    return 0;    
}//end main
