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
#define MEM_SIZE 128
#define DEBUG_HELPER true
#define debug if(true)printf

/********************************************************
 * header
 ********************************************************/

typedef struct{
    //firt fit
    int inserted;
    int numP;
    int numH;
    double cummulativeMem;
    int totalPID;
    int avgP;
    int avgH;
    int availableMem;
    bool flag;
}FF;

typedef struct{
    char ID;
    int memoryUsage;
    int memorySwap;
}Process;

typedef struct{
    //data
    char** array;
    int arraySize;
    char* fileName;
    //ff
    FF* ff;
}Hole;

FF* newFF(){
    FF* new = malloc(sizeof(FF));
    //first fit
    new->inserted = 1;
    new->numP = 0;
    new->numH = 0;
    new->cummulativeMem = 0.0 ;
    new->totalPID = 0.0;
    new->avgP = 0.0;
    new->avgH = 0.0;
    new->availableMem = 0;
    new->flag = true;
    return new;
}//end func

Process* newProcess(){
    Process* new = malloc(sizeof(Process));
    new->ID = 0;
    new->memoryUsage = 0;
    new->memorySwap = 0;
    return new; 
}//end constructor

Hole* newHole(){
    Hole* new = malloc(sizeof(Hole));
    //data
    new->array = NULL;
    new->arraySize = 0;
    new->fileName = NULL;
    new->ff = newFF();
    return new;
}//end constructor

/********************************************************
 * helper functions, some of them are from my old course
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
        line[strcspn(line, "\r\n")] = '\0';
        if(DEBUG_HELPER)printf("line %d: (%s)\n", numberOfLines, line);
        stringArray[numberOfLines] = setString(line); 
        //debug("String Length: %d\n", (int)strlen(line));
        numberOfLines++;
        //create string array from each line
        stringArray = realloc(stringArray, sizeof(stringArray)*(numberOfLines+1));
        //go to the next
    }//end loop
    if(DEBUG_HELPER)printf("String loaded...\n");
    
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
 * function for linklist
 ********************************************************/

void deleteProcess(void* process){
    Process* object = (Process*)process;
    free(object);
}//end func

int compareProcesses(const void* a, const void* b){
    if(a == b){
        return 0;
    }else{
        return -1;
    }//end if
}//end func

/********************************************************
 * function
 ********************************************************/

void initMem(char* mem, int size){
    for(int x=0; x<size; x++){
        mem[x] = '0';
    }//end for
}//end func

void resetFF(FF* ff){
    ff->numP = 0;
    ff->numH = 0;
    ff->availableMem = 0;
    ff->flag = true;
}//end func

void printMem(List* memQ, double* cummulativeMem, Hole* hole){
    int used = 0;
    
}//end func

/********************************************************
 * op func
 ********************************************************/

void firstFit(Hole* hole, List* queue){
    //create mem
    char mem[MEM_SIZE];
    initMem(mem, MEM_SIZE);
    Process* tempP = NULL;
    Process* tempP2 = NULL;
    List* memQ = initializeListPointer(dummyPrint, deleteProcess, compareProcesses);

    //loop until length is 0
    while(getLength(*queue) != 0){
        resetFF(hole->ff);
        if(hole->ff->inserted == 1){
            tempP = pop(queue);
        }//end if

        //dec spacec 
        int space = tempP->memoryUsage;

        for(int x=0; x<MEM_SIZE; x++){
            //count avilable mem
            if(mem[x] != '0'){
                hole->ff->availableMem = 0;
            }else{
                hole->ff->availableMem = hole->ff->availableMem + 1;
            }//end if

            //insert processes
            if(hole->ff->availableMem >= space){
                int end = x-space;
                for(int y=x; y>end; y--){
                    mem[y] = tempP->ID;
                }//end for
                hole->ff->flag = 1;
                hole->ff->flag = false;
                insertBack(memQ, tempP);
            }//end if
        }//end for

    }//end while
    
    clearList(memQ);
}//end if

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
    hole->array = readFileByLine(hole->fileName, &hole->arraySize, 256);
    
    //dec queues
    List* queue1 = initializeListPointer(dummyPrint, deleteProcess, compareProcesses);
    List* queue2 = initializeListPointer(dummyPrint, deleteProcess, compareProcesses);
    List* queue3 = initializeListPointer(dummyPrint, deleteProcess, compareProcesses);
    List* queue4 = initializeListPointer(dummyPrint, deleteProcess, compareProcesses);

    //assign the data
    for(int x=0; x<hole->arraySize; x++){
        //create a splitter for the data
        int tokenSize = 0;
        char** token = split(hole->array[x], " ", &tokenSize);
        debug("debug: id = %c, memUsage = %s\n", token[0][0], token[1]);
        //assign p1
        Process* process1 = newProcess();
        process1->ID = token[0][0];
        process1->memoryUsage = atoi(token[1]);
        //assign p2
        Process* process2 = newProcess();
        process2->ID = token[0][0];
        process2->memoryUsage = atoi(token[1]);
        //assign p3
        Process* process3 = newProcess();
        process3->ID = token[0][0];
        process3->memoryUsage = atoi(token[1]);
        //assign p4
        Process* process4 = newProcess();
        process4->ID = token[0][0];
        process4->memoryUsage = atoi(token[1]);

        //insert to the back
        insertBack(queue1, process1);
        insertBack(queue2, process2);
        insertBack(queue3, process3);
        insertBack(queue4, process4);

        //free the token
        freeStringArray(token, tokenSize);
    }//end for

    printf("----------------------------<<<((( FIRST FIT )))>>>----------------------------\n");
    firstFit(hole, queue1);
    printf("-------------------------------------------------------------------------------\n");


    //free the hole
    free(hole->fileName);
    freeStringArray(hole->array, hole->arraySize);
    free(hole);
    //free queue
    clearList(queue1);
    clearList(queue2);
    clearList(queue3);
    clearList(queue4);
    return 0;    
}//end main
