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

void loadData(){

}//end func

int main(int argc, char** argv){
    debug("argc = %d\n", argc);
    if(argc != 1){
        printf("Invalid argument");
        return 0;
    }//end if
    return 0;
}//end main