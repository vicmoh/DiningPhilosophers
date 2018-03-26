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
#define d debug("CHECK\n");
#define MEM_SIZE 128

typedef struct{
    char** arrayOfData;
    int numOfArray;
    char fileName[256];
}Hole;

typedef struct{
    char name[256];
    int size;
    int numOfSwaps;
    int isInserted;
    int insertPosition;
}Process;

typedef struct{
    double sumOfMemUsage;
    int numOfSums;
    double usagePercentage; 
    int numOfMemUsed;
    int* proccessMap;
    int numberOfHoles;
    int numberOfProcesses;
    int currentProcessMem;
    int previousProcessMem;
}Display;

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
    new->numOfSwaps = 0;
    new->isInserted = FALSE;
    new->insertPosition = 0;
    return new;
}//end constructor

Display* newDisplay(List* queue){
    Display* new = malloc(sizeof(Display));
    new->sumOfMemUsage = 0;
    new->numOfSums = 0;
    new->usagePercentage = 0; 
    new->numOfMemUsed = 0;
    new->proccessMap = calloc(getLength(*queue), sizeof(new->proccessMap));
    for(int x=0; x<getLength(*queue); x++){
        new->proccessMap[x] = FALSE;
    }//end for
    new->numberOfHoles = 0;
    new->numberOfProcesses = 0;
    new->currentProcessMem = FALSE;
    new->previousProcessMem = FALSE;
    return new;
}//end constructor

char* setString(char* string){
    char* newString = calloc(strlen(string)+9, sizeof(char));
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
    char line[256];
    char** arrayOfString = malloc(sizeof(arrayOfString)*1);
    int x = 0;

    //read each line in the file
    printf("Loading data...\n");
    while(fgets(line, sizeof(line), filePointer) != NULL){
        line[strcspn(line, "\r\n")] = '\0';
        arrayOfString[x] = setString(line);
        debug("arrayOfString %d = %s\n", x, arrayOfString[x]);
        x++;
        arrayOfString = realloc(arrayOfString, sizeof(arrayOfString)*(x+1));
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
    char** arrayOfToken = malloc(sizeof(arrayOfToken)*2);
    int x = 0;
    char* token = strtok(stringToBeSplit, delimiter);
    //create a split func that will used to parse the data
    while(token != NULL){
        arrayOfToken[x] = setString(token);
        debug("arrayOfToken %d = %s\n", x, arrayOfToken[x]);
        x++;
        arrayOfToken[x] = NULL;
        arrayOfToken = realloc(arrayOfToken, sizeof(arrayOfToken)*(x+2));
        token = strtok(NULL, delimiter);
    }//end while
    return arrayOfToken;
}//end func

void freeToken(char** token){
    for(int x=0; token[x] != NULL; x++){
        free(token[x]);
    }//end for
    free(token);
}//end func

void deleteProcess(void* toBeDeleted){
    Process* object = (Process*)toBeDeleted;
    free(object);
}//end func

void printMem(Process* process, char** memoryMap, List* queue){
    //dec vars
    Display* data = newDisplay(queue);

    //for through the mem
    for(int x=0; x<MEM_SIZE; x++){
        //set current mem
        char* currentMem = setString(memoryMap[x]);
        
        //find which process is done
        for(int y=0; y<getLength(*queue); y++){
            if(strcmp(currentMem, memoryMap[y]) == 0){
                data->proccessMap[y] = TRUE;
            }//end if
        }//end for

    if(strcmp(memoryMap[x], "0") != 0){
            data->numOfMemUsed = data->numOfMemUsed + 1;
        }//end if

        free(currentMem);
    }//end for

    //calc the usage percentage
    data->usagePercentage = (double)data->numOfMemUsed/(double)MEM_SIZE;
    data->usagePercentage = data->usagePercentage * 100;
    data->sumOfMemUsage = data->sumOfMemUsage + data->usagePercentage;
    data->numOfSums = data->numOfSums + 1;

    //count number of process
    for(int x=0; x<getLength(*queue); x++){
        if(data->proccessMap[x] == TRUE){
            data->numberOfProcesses = data->numberOfProcesses + 1;
        }//end if
    }//end for

    //find the number if holes
    data->currentProcessMem = TRUE;
    for(int x=1; x<MEM_SIZE; x++){
        data->previousProcessMem = data->currentProcessMem;
        if(strcmp(memoryMap[x], "0") == 0){
            data->currentProcessMem = FALSE;
        }else{
            data->currentProcessMem = TRUE;
        }//end if
        if(data->currentProcessMem != data->previousProcessMem && data->currentProcessMem == FALSE){
            data->numberOfHoles = data->numberOfHoles + 1; 
        }//end if
    }//end for

    //at the start of memory block, check for the hole
    if(data->numberOfHoles != 0){
        data->numberOfHoles = data->numberOfHoles + 1;
    }//end if

    //print
    printf("pid %s loaded, #processes = %d, #holes = %d, %%memusage = %lf, cumulative %%memusage = %lf\n",
        process->name, data->numberOfProcesses, data->numberOfHoles-1, (double)(data->usagePercentage), (double)data->sumOfMemUsage/(double)data->numOfSums);
    free(data->proccessMap);
    free(data);
}//end func

Process* copyProcess(Process* toBeCopied){
    Process* new = malloc(sizeof(Process));
    strcpy(new->name, toBeCopied->name);
    new->size = toBeCopied->size;
    new->numOfSwaps = toBeCopied->numOfSwaps;
    new->isInserted = toBeCopied->isInserted;
    new->insertPosition = toBeCopied->insertPosition;
    return new;
}//end func

void removeProcess(char** memoryMap, List* addedQueue, List* queue){
    Process* process = getFromFront(*addedQueue);
    deleteDataFromList(addedQueue, process);
    if(!process){
        return;
    }//end if

    process->numOfSwaps = process->numOfSwaps + 1;
    if(process->numOfSwaps != 3){
        Process* copiedProcess = copyProcess(process);
        insertBack(queue, copiedProcess);
    }//end if

    for(int x=0;x<MEM_SIZE;x++){
        if(strcmp(memoryMap[x], process->name) == 0){
            strcpy(memoryMap[x], "0");
        }//end if
    }//end for
}//end func

void firstFit(List* queue){
    //create mem
    debug("debug create process\n");
    char** memoryMap = calloc(MEM_SIZE, sizeof(memoryMap));
    debug("getLength = %d\n", getLength(*queue));
    for(int x=0; x<MEM_SIZE; x++){
        memoryMap[x] = calloc(256 ,sizeof(char));
        strcpy(memoryMap[x], "0");
    }//end for
    debug("debug create process completed\n");

    //get process
    List* addedQueue = initializeListPointer(dummyPrint, deleteProcess, dummyCompare);
    Process* process = getFromFront(*queue);
    deleteDataFromList(queue, process);
    debug("debug get process\n");

    //go through the process
    while(process){
        int memSpace = 0;
        int x = 0;
        //go through the memoryMap
        for(x=0; x<MEM_SIZE; x++){
            //debug("memoryMap = %s\n", memoryMap[x]);
            if(strcmp(memoryMap[x], "0") != 0){
                memSpace = 0;
            }else{
                memSpace = memSpace + 1;
            }//end if
            if(process->size == memSpace){

                //insert process to memoryMap
                int insertAtIndex = x-memSpace+1;
                int endLoop = insertAtIndex + process->size;
                for(int y=insertAtIndex;y<endLoop;y++){
                    strcpy(memoryMap[y], process->name);
                }//end for

                //print
                printMem(process, memoryMap, queue);
                
                //insert to to recenly added queue
                Process* copiedProcess = copyProcess(process);
                insertBack(addedQueue, copiedProcess);
                break;
            }//end if
        }//end for

        if(x == MEM_SIZE){
            removeProcess(memoryMap, addedQueue, queue);
        }else{
            process = getFromFront(*queue);
            deleteDataFromList(queue, process);
        }//end if
    }//end while
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
    List* queue1 = initializeListPointer(dummyPrint, deleteProcess, dummyCompare);
    List* queue2 = initializeListPointer(dummyPrint, deleteProcess, dummyCompare);
    List* queue3 = initializeListPointer(dummyPrint, deleteProcess, dummyCompare);
    List* queue4 = initializeListPointer(dummyPrint, deleteProcess, dummyCompare);

    for(int x=0; x<hole->numOfArray; x++){
        //split the data and dec vars
        char tempName[256] = {"\0"};
        int tempSize = 0;
        char** token = split(hole->arrayOfData[x], " ");
        debug("debug: name = (%s), size = (%s)\n", token[0], token[1]);
        strcpy(tempName, token[0]);
        tempSize = atoi(token[1]);
        
        //make sure its within the size
        if(tempSize > 0 && MEM_SIZE >= tempSize){
            //process 1
            Process* process1 = newProcess();
            strcpy(process1->name, tempName);
            process1->size = tempSize; 
            //process 2
            Process* process2 = newProcess();
            strcpy(process2->name, tempName);
            process2->size = tempSize; 
            //process 3
            Process* process3 = newProcess();
            strcpy(process3->name, tempName);
            process3->size = tempSize; 
            //process 4
            Process* process4 = newProcess();
            strcpy(process4->name, tempName);
            process4->size = tempSize; 
            
            //insert the process to each queue
            insertBack(queue1, process1);
            insertBack(queue2, process2);
            insertBack(queue3, process3);
            insertBack(queue4, process4);
        }else{
            printf("Process ID %s, could not be added due to the size of memory\n", tempName);
        }//end if

        freeToken(token);
    }//end for
    debug("\n");

    printf("---------------<<<((( FIRST FIT )))>>>---------------\n");
    firstFit(queue1);
    printf("-----------------------------------------------------\n");

    clearList(queue1);
    clearList(queue2);
    clearList(queue3);
    clearList(queue4);
    return 0;
}//end main