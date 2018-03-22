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
#define FALSE 2
#define NUMBER_RANGE( var ) (var >= 48 && var <= 57)
#define debug if(TRUE)printf

/********************************************
 * header
 ********************************************/

typedef struct{
    int philNumber;
    int lowFork;
    int upFork;
    int numOfEats;
    int forkSelection;
}Philosopher;

typedef struct{
    Philosopher** phil;
    pthread_mutex_t* forks;
}Instance;

Philosopher* newPhilosopher();
Instance* newInstance(int numOfPhils);
void initPThreadMutex(Instance* vars, int numOfPhils);
int checkIfNumbers(char* string);

/********************************************
 * main
 ********************************************/

int main(int argc, char **argv){
    //dec vars
    char* numOfPhilsInString = argv[1];
    char* numOfEatsInString = argv[2];
    int numOfPhils = 0;
    int numOfEats = 0;
    debug("argc = %d\n", argc);

    //error check
    if(argc == 3){
        printf("There must be 3 arguments...\n");
        printf("Please re-run the program..\n");
        printf("Exiting program...\n");
        return 0;
    }//end if
    if(checkIfNumbers(numOfPhilsInString) == FALSE || checkIfNumbers(numOfEatsInString) == FALSE){
        printf("Arguments must be a number...\n");
        printf("Please re-run the program..\n");
        printf("Exiting program...\n");
        return 0;
    }//end if

    //convert the arg to int
    numOfPhils = atoi(numOfPhilsInString);
    numOfEats = atoi(numOfEatsInString);
    Instance* vars = newInstance(numOfPhils);
    initPThreadMutex(vars, numOfPhils);
    
}//end main

/********************************************
 * constructors
 ********************************************/

Philosopher* newPhilosopher(){
    Philosopher* new = malloc(sizeof(Philosopher));
    new->philNumber = 0;
    new->lowFork = 0;
    new->upFork = 0;
    new->numOfEats = 0;
    new->forkSelection = 0;
    return new;
}//end constructor

Instance* newInstance(int numOfPhils){
    //init instance to act as global
    Instance* new = malloc(sizeof(Instance));
    // init phil
    // For each philosopher, 
    //assign it two forks such that phil i get forks i-1 
    //and i for all i>0. If i=0, take fork0 and forkMax
    new->phil = calloc(numOfPhils, sizeof(Philosopher*));
    for(int x=0; x<numOfPhils; x++){
        new->phil[x] = newPhilosopher();
        new->phil[x]->philNumber = x+1;
        new->phil[x]->upFork = x;
        new->phil[x]->numOfEats = 0;
        if(x == 0){
            new->phil[x]->lowFork = numOfPhils-1;
        }else if(x != 0){
            new->phil[x]->lowFork = x-1;
        }//end if
    }//end if
    //init forks
    new->forks = calloc(numOfPhils, sizeof(pthread_mutex_t));
    return new;
}//end constructor

/********************************************
 * functions
 ********************************************/

void initPThreadMutex(Instance* vars, int numOfPhils){
    //init the pthread mutex
    for(int x=0; x<numOfPhils; x++){
        pthread_mutex_init(&vars->forks[x], NULL);
    }//end for
}//end func

int checkIfNumbers(char* string){
    //make sure it doesnt start at 0
    if(strlen(string) > 1 && string[0] == '0'){
        return FALSE;
    }//end if

    //loop through each char and check if its number
    for(int x=0; x<strlen(string); x++){
        if(!NUMBER_RANGE(string[x])){
            return FALSE;
        }//end if
    }//end for
    return TRUE;
}//end func

