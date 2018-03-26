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
#define TRUE 1
#define FALSE 2
#define DEBUG_HELPER false
#define debug if(true)printf

/********************************************************
 * header
 ********************************************************/

typedef struct{
    //firt fit
    int inserted;
    int availableMem;
    bool flag;
}FF;

typedef struct{
    int inserted;
    int holeIndex; 
    int found;
    int flag;
    int availableMem;
    int smallestSpace; 
    int smallestIndex;
}BF;

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
    //main vars to be printed
    int numP;
    int numH;
    double cummulativeMem;
    int totalPID;
    double avgP;
    double avgH;
    //ff
    FF* ff;
    BF* bf;
}Hole;

FF* newFF(){
    FF* new = malloc(sizeof(FF));
    //first fit
    new->inserted = 1;
    new->availableMem = 0;
    new->flag = true;
    return new;
}//end constructor

BF* newBF(){
    BF* new = malloc(sizeof(BF));
    new->inserted = 1;
    new->holeIndex = 0; 
    new->found = 0;
    new->flag = true;//need fix
    new->availableMem = 0;
    new->smallestSpace = MEM_SIZE; 
    new->smallestIndex = 0;
    return new;
}//end constructor

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
    //
    new->numP = 0;
    new->numH = 0;
    new->cummulativeMem = 0.0 ;
    new->totalPID = 0.0;
    new->avgP = 0.0;
    new->avgH = 0.0;
    //
    new->ff = newFF();
    new->bf = newBF();
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
        //if(DEBUG_HELPER)printf("String Length: %d\n", (int)strlen(line));
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

void printStat(Hole* hole, char id, double memUsagePercentage){
    printf("%c PID Loaded, #processes = %d, #holes %d, %%memusage = %.4lf, cumulative %%mem = %.4lf\n", 
        id, hole->numP, hole->numH, memUsagePercentage, hole->cummulativeMem);
}//end func

void printFinal(Hole* hole){
    printf("Total Loads: %d, average #processes: %.4lf, average #holes: %.4lf, %%cummulativeMem: %.4lf\n", 
        hole->totalPID, hole->avgP, hole->avgH, hole->cummulativeMem);
}//end func

void initMem(char* mem, int size){
    for(int x=0; x<size; x++){
        mem[x] = '0';
    }//end for
}//end func

void resetHole(Hole* hole){
    hole->numP = 0;
    hole->numH = 0;
}//end func

void resetFF(FF* ff){
    ff->availableMem = 0;
    ff->flag = true;
}//end func

void resetBF(BF* bf){
    bf->availableMem = 0;
    bf->flag = FALSE;
    bf->found = 0;
    bf->smallestSpace = MEM_SIZE;
}//end func

void printMem(List* memQ, char mem[MEM_SIZE], Hole* hole, char id){
    //dec vars
    int numOfProcess = getLength(*memQ);
    hole->numP = numOfProcess;
    int used = 0;
    //loop till mem size
    for(int x=0; x<MEM_SIZE-1; x++){
        if(mem[x+1] != '0' && mem[x] == '0'){
            hole->numH = hole->numH + 1;
        }//end if
        
        if(mem[x] != '0'){
            used = used + 1;
        }//end if
    }//end for

    if(mem[MEM_SIZE-1] == '0'){
        hole->numH = hole->numH + 1;
    }else if(mem[MEM_SIZE-1] != '0'){
        used = used + 1;
    }//end if

    //calc the process
    double memUsage = (double)(used)/MEM_SIZE;
    double memUsagePercentage = memUsage * 100;
    double memID = hole->cummulativeMem * hole->totalPID;
    hole->cummulativeMem = (memID + memUsagePercentage) / (hole->totalPID + 1);
    hole->totalPID = hole->totalPID + 1;
    printStat(hole, id, memUsagePercentage);
}//end func

void calcAverage(Hole* hole){
    hole->avgP = ((hole->avgP*(hole->totalPID-1))+hole->numP)/hole->totalPID;
    hole->avgH = ((hole->avgH*(hole->totalPID-1))+hole->numH)/hole->totalPID;
}//end func

/********************************************************
 * op func
 ********************************************************/

void firstFit(Hole* hole, List* queue){
    //create mem
    char mem[MEM_SIZE];
    initMem(mem, MEM_SIZE);
    Process* p1 = NULL;
    Process* p2 = NULL;
    List* memQ = initializeListPointer(dummyPrint, deleteProcess, compareProcesses);
    
    //loop until length is 0
    while(getLength(*queue) != 0){
        resetHole(hole);
        resetFF(hole->ff);
        if(hole->ff->inserted == TRUE){
            p1 = pop(queue);
            if(DEBUG_HELPER)printf("queue length = %d\n", getLength(*queue));
        }//end if

        //dec spacec 
        int space = p1->memoryUsage;

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
                    mem[y] = p1->ID;
                }//end for
                hole->ff->inserted = 1;
                hole->ff->flag = false;
                insertBack(memQ, p1);
                printMem(memQ, mem, hole, p1->ID);
                calcAverage(hole);
                break;
            }//end if
        }//end for

        //for no space
        if(hole->ff->flag == true){
            hole->ff->inserted = FALSE;
            p2 = pop(memQ);
            if(DEBUG_HELPER)printf("memQ length = %d\n", getLength(*memQ));
            //if(DEBUG_HELPER)printf
            if(p2 == NULL){
                if(DEBUG_HELPER)printf("p2 == NULL !!!!!!!!!!!!!!!!!!!!!!!!!\n");
                break;
            }//end
            //look through the mem
            for(int x=0; x<MEM_SIZE; x++){
                char id = p2->ID;
                if(mem[x] == id){
                    mem[x] = '0';
                }//end if
            }//end for
            
            //count swap mem
            p2->memorySwap = p2->memorySwap + 1;
            
            if(p2->memorySwap < 3){
                insertBack(queue, p2);
            }//end if
        }//end if
    }//end while

    printFinal(hole);
    clearList(memQ);
}//end func

void bestFit(Hole* hole, List* queue){
    //create mem
    char mem[MEM_SIZE];
    initMem(mem, MEM_SIZE);
    List* memQ = initializeListPointer(dummyPrint, deleteProcess, compareProcesses);
    Process* p1 = NULL;
    Process* p2 = NULL;
    //loop til there is nothing in queue
    while(getLength(*queue) != 0){
        resetHole(hole);
        resetBF(hole->bf);
        if(hole->bf->inserted == TRUE){
            p1 = pop(queue);
        }//end if
        
        //set the amount space
        int space = p1 ->memoryUsage;
        
        //loop mem
        for(int x =0; x<MEM_SIZE; x++){

            //check case if mem is avail
            int memIsAvail = FALSE;
            if(hole->bf->availableMem == 0){
                memIsAvail = TRUE;
            }//end if

            if(memIsAvail == TRUE && mem[x] == '0'){
                hole->bf->holeIndex = x;
                hole->bf->availableMem = x;
            }else if(memIsAvail == FALSE && mem[x] == '0'){
                hole->bf->availableMem = hole->bf->availableMem + 1;
            }else{
                hole->bf->found = TRUE;
                
                //check mem is out of bound
                int isLessThanSmallestPlace = FALSE;
                int isGreaterThanSpace = FALSE;
                if(hole->bf->availableMem <= hole->bf->smallestSpace){
                    isLessThanSmallestPlace = TRUE;
                    if(hole->bf->availableMem >= space){
                        isGreaterThanSpace = TRUE;
                    }//end if
                }//end if

                //search for the smallest hole
                if(isLessThanSmallestPlace == TRUE && isGreaterThanSpace == TRUE){
                    hole->bf->smallestIndex = hole->bf->holeIndex;
                    hole->bf->smallestSpace = hole->bf->availableMem;
                    hole->bf->flag = TRUE;
                }//end if

                //set available mem to 0
                hole->bf->availableMem = 0;
            }//end
        }//end for

        if(hole->bf->flag == FALSE){
            hole->bf->inserted = FALSE;
            p2 = pop(memQ);
            for(int x=0; x<MEM_SIZE; x++){
                if(mem[x] == p2->ID){
                    mem[x] = '0';
                }//end if
                p2->memorySwap = p2->memorySwap + 1;
                if(p2->memorySwap < 3){
                    insertBack(queue, p2);
                }//end if
            }//end for
        }else{
            int end = hole->bf->smallestIndex+space;
            for(int x=0; x<end; x++){
                mem[x] = p1->ID;
            }//end for
            hole->bf->inserted = TRUE;
            hole->bf->flag = TRUE;
            insertBack(memQ, p1);
            printMem(memQ, mem, hole, p1->ID);
            calcAverage(hole);
        }//end if
    }//end while

    printFinal(hole);
    clearList(memQ);
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
    hole->array = readFileByLine(hole->fileName, &hole->arraySize, 256);
    
    //dec queues
    List* queue1 = initializeListPointer(dummyPrint, deleteProcess, compareProcesses);
    List* queue2 = initializeListPointer(dummyPrint, deleteProcess, compareProcesses);
    List* queue3 = initializeListPointer(dummyPrint, deleteProcess, compareProcesses);
    List* queue4 = initializeListPointer(dummyPrint, deleteProcess, compareProcesses);

    //assign the data
    if(DEBUG_HELPER)printf("hole->arraySize = %d\n", hole->arraySize);
    for(int x=0; x<hole->arraySize; x++){
        //create a splitter for the data
        int tokenSize = 0;
        char** token = split(hole->array[x], " ", &tokenSize);
        if(DEBUG_HELPER)printf("debug: id = %c, memUsage = %s\n", token[0][0], token[1]);
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
