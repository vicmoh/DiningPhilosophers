/********************************************
 * Vicky Mohammad
 * 0895381
 * March 22, 2018
 * https://www.geeksforgeeks.org/dining-philosophers-solution-using-monitors/
 * https://osandnetworkingcslab.wordpress.com/implementation-of-dining-philosophers-using-threads/
 ********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#define TRUE 1
#define FALSE 0
#define INVERT 1
#define NORMAL 0
#define NUMBER_RANGE( var ) (var >= 48 && var <= 57)
#define debug if(FALSE)printf

/********************************************
 * header
 ********************************************/

//dec forks on the table
pthread_mutex_t* forks;
//dec object philosopher
typedef struct{
    int philNumber;
    int downFork;
    int upFork;
    int numOfEats;
    int forkSelection;
}Philosopher;
//dec dine that will have phil eating
typedef struct{
    Philosopher** phil;
    pthread_t* tempThread;
}Dine;
//objects
Philosopher* newPhilosopher();
Dine* newDine(int numOfPhils);
//functions/methods
int checkIfNumbers(char* string);
void runSimulation(Dine* vars, int numOfPhils, int numOfEats);
void* philosopherDiner(void* arg);
int waiting();

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

    //error checking if num of arg is not 3 and not number
    if(argc != 3){
        printf("There must be 2 arguments...\n");
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
    Dine* vars = newDine(numOfPhils);
    
    //run the program
    runSimulation(vars, numOfPhils, numOfEats);
    printf("Simulation complete.\n");
    printf("All %d philosophers are done eating!\n", numOfPhils);
    return 0;
}//end main

/********************************************
 * constructors
 ********************************************/

Philosopher* newPhilosopher(){
    //create new philosopher
    Philosopher* new = malloc(sizeof(Philosopher));
    new->philNumber = 0;
    new->downFork = 0;
    new->upFork = 0;
    new->numOfEats = 0;
    new->forkSelection = 0;
    return new;
}//end constructor

Dine* newDine(int numOfPhils){
    //init Dine to act as global
    Dine* new = malloc(sizeof(Dine));

    // initilizie new philosophers
    // For each philosopher, 
    //assign it two forks for picking the nearest
    new->phil = calloc(numOfPhils, sizeof(Philosopher*));
    for(int x=0; x<numOfPhils; x++){
        new->phil[x] = newPhilosopher();
        new->phil[x]->philNumber = x+1;
        new->phil[x]->upFork = x;
        new->phil[x]->numOfEats = 0;
        if(x == 0){
            new->phil[x]->downFork = numOfPhils-1;
        }else if(x != 0){
            new->phil[x]->downFork = x-1;
        }//end if
    }//end if

    //initilizie the number of forks availble based on the number of philosophers
    forks = calloc(numOfPhils, sizeof(pthread_mutex_t));
    for(int x=0; x<numOfPhils; x++){
        pthread_mutex_init(&forks[x], NULL);
    }//end if
    new->tempThread = calloc(numOfPhils, sizeof(pthread_t));
    return new;
}//end constructor

/********************************************
 * functions/methods
 ********************************************/

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

void runSimulation(Dine* vars, int numOfPhils, int numOfEats){
    //run the simlation
    debug("running sim()\n");
    //assign the relevent vars to the object being passed into the thread
    for(int x=0; x<numOfPhils; x++){
        vars->phil[x]->numOfEats = numOfEats;
        if(numOfPhils-1 == x){
            vars->phil[x]->forkSelection = INVERT;
        }//end if
        pthread_create(&vars->tempThread[x], NULL, &philosopherDiner, &vars->phil[x]);
    }//end for

    //thread cache
    for(int x=0; x<numOfPhils; x++){
        pthread_join(vars->tempThread[x],NULL);
    }//end for
    free(vars->tempThread);
}//end func

void* philosopherDiner(void* arg){
    debug("calling philosopherDiner()\n");
    //set the current phil that is waiting to eat
    Philosopher* phil = *((Philosopher**)arg);

    //For loop as many times as they have to eat
    for(int x=0; x<phil->numOfEats; x++){
        //let the phil grab the forks
        if(phil->forkSelection == INVERT){
            waiting();
            printf("Philosopher %d is thinking...\n", phil->philNumber);
            pthread_mutex_lock(&forks[phil->upFork]);
            pthread_mutex_lock(&forks[phil->downFork]);
        }else if(phil->forkSelection == NORMAL){
            waiting();
            printf("Philosopher %d is thinking...\n", phil->philNumber);
            pthread_mutex_lock(&forks[phil->downFork]);
            pthread_mutex_lock(&forks[phil->upFork]);
        }//end if
        waiting();

        //time for the person to eat
        printf("Philosopher %d is eating...\n", phil->philNumber);
        pthread_mutex_unlock(&forks[phil->downFork]);
        pthread_mutex_unlock(&forks[phil->upFork]);
    }//end for
    pthread_exit(NULL);
}//end func

int waiting(){
    //create a waiting time by using sleep
    //the waiting time will be random from 1 to 2 sleep
    debug("calling waiting()\n");
    int randomNum = rand() % 2;
    int waitTime = sleep(randomNum + 1);
    debug("wait time = %d\n", waitTime);
    return waitTime;
}//end func 