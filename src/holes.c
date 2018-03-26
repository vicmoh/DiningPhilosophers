#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include "LinkedListAPI.h"
#include <stdbool.h>
#define MAX 128
typedef struct Process
{
    char pid;
    int memUsg;
    int memSwap;
}Process;

typedef struct Node 
{
    Process thread;
    struct Node *prev;
} Node;

typedef struct Queue {
    Node *head;
    Node *tail;
    int size;
    int limit;
} Queue;

Node * createNode(char pid,int memusage);
void* pop(Queue *pQueue);
int getSize(Queue * queue);
void push(Queue *queue, void *data);

Queue* createQueue(unsigned int limit);

void printMemStatus(Queue *memoryQ, double* cumulativeMem, char memory[MAX], double* totalPIDs, int *numProcesses, int* numHoles);

char *firstfit(Queue *q);

char * bestfit(Queue *q);

void worstfit(Queue *q);

void nextfit(Queue *q);



int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Incorrect number of arguments\n");
        return 1;
    }

   
    FILE* fptr = NULL;

    
    fptr = fopen(argv[1], "r");
    if(fptr == NULL)
    {
        printf("Unable to open file\n");
        return 1;
    }

    Queue *q1 = createQueue(50);
    Queue *q2 = createQueue(50);
    Queue *q3 = createQueue(50);
    Queue *q4 = createQueue(50);

    Node *new = malloc(sizeof(Node));

    char buffer[200];
    while(fgets(buffer,200,fptr)!= NULL)
    {

       if(sscanf(buffer, "%s %d", &new->thread.pid, &new->thread.memUsg) == 2)
       {
        Node * temp1 = createNode(new->thread.pid,new->thread.memUsg);

        Node * temp2 = createNode(new->thread.pid,new->thread.memUsg);

        Node * temp3 = createNode(new->thread.pid,new->thread.memUsg);

        Node *temp4 = createNode(new->thread.pid,new->thread.memUsg);

        push(q1, temp1);
        push(q2, temp2);
        push(q3, temp3);
        push(q4, temp4);
       }
    }
    Queue *firstFit = q1;
    Queue *bestFit = q2;
    Queue *worstFit = q3;
    Queue *nextFit = q4;
    printf("********************\n");
    printf("*     FIRST FIT    *\n");
    printf("********************\n");

    char  *str = firstfit(firstFit);
    printf("%s\n",str );
    printf("********************\n");
    printf("*     BEST FIT     *\n");
    printf("********************\n");

    str = bestfit(bestFit);
      printf("%s\n",str );
    printf("********************\n");
    printf("*     WORST FIT    *\n");
    printf("********************\n");
    worstfit(worstFit);
    printf("********************\n");
    printf("*     NEXT FIT     *\n");
    printf("********************\n");
    nextfit(nextFit);


}

char *firstfit(Queue *q)
{
    char memory[MAX];
    for(int i = 0; i<MAX;i++)
    {
        memory[i] = '0';
    }

    Queue *memoryQ = createQueue(50);
    char * string = calloc(1,sizeof(char)*1000);
    int inserted = 1;
    int numProcesses = 0, numHoles = 0;
    double cumulativeMem = 0.0, totalPIDs = 0.0, avgProcess = 0.0, avgHoles = 0.0;
    int availMemory = 0;
    bool flag = true;
    Node *temp = NULL; 
    Node *temp2 = NULL;
    while(q->size != 0) //(temp = pop(q)) != NULL) //While there are still things in the queue able to be popped
    {
        numProcesses = 0;
        numHoles = 0;
        availMemory = 0;
        flag = true;
        if(inserted == 1)
        {
            temp = pop(q);
        }
        int space = temp->thread.memUsg;
        for(int i = 0; i<MAX; i++)
        {
            if(memory[i] == '0')
            {
                availMemory++;
            }
            else if(memory[i] != '0')
            {
                availMemory = 0;
            }

            if(availMemory < space)
            {

            }
            else if(availMemory>=space) //Insert process
            {
                for(int j = i; j>i-space; j--) //Adding process in memory
                {
                    memory[j] = temp->thread.pid;
                }
                //printMemory(memory);
                flag = false;
                inserted = 1;
                push(memoryQ, temp);
                printMemStatus(memoryQ, &cumulativeMem, memory, &totalPIDs, &numProcesses, &numHoles);

                avgProcess = ((avgProcess*(totalPIDs-1)) + numProcesses)/totalPIDs;
                avgHoles = ((avgHoles *(totalPIDs-1)) + numHoles)/totalPIDs;
                break;
            }
        }
        
        if(flag == true) //No space
        {
            inserted = 0;
            temp2 = pop(memoryQ); //Pop process that's been on queue the longest
            for(int i = 0; i<MAX; i++)
            {
                if(memory[i] != temp2->thread.pid)
                {

                }
                else if(memory[i] == temp2->thread.pid)
                {
                    memory[i] = '0';
                }
            }
            temp2->thread.memSwap++;

            if(temp2->thread.memSwap >= 3)
            {

            }
            else if(temp2->thread.memSwap < 3) 
            {
                push(q, temp2);
            }
        }

    }

    sprintf(string,"**Total Loads: %.0lf, average #processes: %.1lf, average #holes: %.1lf, %%cumulativeMem: %.1lf**\n", totalPIDs, avgProcess, avgHoles, cumulativeMem);
    return string;
}

char * bestfit(Queue *q)
{
    char memory[MAX];
    for(int i = 0; i<MAX;i++)
    {
        memory[i] = '0';
    }
    char * string = calloc(1,sizeof(char)*1000);

    Queue *memoryQ = createQueue(50);
    int inserted = 1, holeIndex = 0, found = 0, numProcesses = 0, numHoles = 0;
    double cumulativeMem = 0, totalPIDs = 0.0, avgProcess = 0.0, avgHoles = 0.0;
    int flag, availMemory = 0, smallestSpace = MAX, smallestIndex = 0;
    Node *temp, *temp2;

    while(q->size != 0) //(temp = pop(q)) != NULL) //While there are still things in the queue able to be popped
    {
        numProcesses = 0;
        numHoles = 0;
        holeIndex = 0;
        availMemory = 0;
        flag = 0;
        found = 0;
        smallestSpace = MAX;
        if(inserted == 1)
        {
            temp = pop(q);
        }
        int space = temp->thread.memUsg;
        for(int i = 0; i<MAX; i++)
        {
            if(memory[i] == '0' && availMemory == 0)
            {
                //printf("Index: %d\n", i);
                holeIndex = i;
                availMemory++;
            }
            else if(memory[i] == '0')
            {
                availMemory++;
            }
            else 
            {
                found = 1;
                if(availMemory <= smallestSpace && availMemory >= space) //Find smallest hole and starting index that fits
                { 
                    //printf("availMemory: %d, index: %d\n", availMemory, holeIndex);
                    flag = 1;
                    smallestIndex = holeIndex;
                    smallestSpace = availMemory;
                }
                availMemory = 0;
            }

            if(i == MAX-1 && availMemory <= smallestSpace && availMemory >= space) //Reached end
            {
                
                    //printf("availMemory: %d, index: %d\n", availMemory, holeIndex);
                    //printf("flag on\n");
                    flag = 1;
                    smallestIndex = holeIndex;
                    smallestSpace = availMemory;
                    availMemory = 0;
                
            }
        }

        if(flag == 0) //No space
        {
            inserted = 0;
            temp2 = pop(memoryQ); //Pop process that's been on queue the longest
            //printf("Removed: %c\n", temp2->thread.pid);
            for(int i = 0; i<MAX; i++)
            {
                if(memory[i] == temp2->thread.pid) //Taking thread out of memory
                    memory[i] = '0';
            }
            temp2->thread.memSwap++;
            if(temp2->thread.memSwap < 3) //Swapped out only once or twice
            {
                push(q, temp2);
            }
        }
        else //A hole fits
        {
            for(int j = smallestIndex; j<smallestIndex+space; j++) //Adding process in memory
            {
                memory[j] = temp->thread.pid;
            }
            //printMemory(memory);
            flag = 1;
            inserted = 1;
            push(memoryQ, temp);
            printMemStatus(memoryQ, &cumulativeMem, memory, &totalPIDs, &numProcesses, &numHoles);
            avgProcess = ((avgProcess * (totalPIDs-1)) + numProcesses)/totalPIDs;
            avgHoles = ((avgHoles * (totalPIDs-1)) + numHoles)/totalPIDs;
            //printf("6\n");
        }
    }

    sprintf(string,"**Total Loads: %.0lf, average #processes: %.1lf, average #holes: %.1lf, %%cumulativeMem: %.1lf**\n", totalPIDs, avgProcess, avgHoles, cumulativeMem);
    return string;
}

void worstfit(Queue *q)
{
    char memory[MAX];
    for(int i = 0; i<MAX;i++)
    {
        memory[i] = '0';
    }

    Queue *memoryQ = createQueue(50);
    int inserted = 1, holeIndex = 0, found = 0, numHoles = 0, numProcesses = 0;
    double cumulativeMem = 0, totalPIDs = 0.0, avgProcess = 0.0, avgHoles = 0.0;
    int flag, availMemory = 0, largestSpace, largestIndex = 0;
    Node *temp, *temp2;

    while(q->size != 0) //(temp = pop(q)) != NULL) //While there are still things in the queue able to be popped
    {
        numProcesses = 0;
        numHoles = 0;
        holeIndex = 0;
        availMemory = 0;
        flag = 0;
        found = 0;
        largestSpace = 0;
        if(inserted == 1)
        {
            temp = pop(q);
        }
        int space = temp->thread.memUsg;
        for(int i = 0; i<MAX; i++)
        {
            if(memory[i] == '0' && availMemory == 0)
            {
                //printf("Index: %d\n", i);
                holeIndex = i;
                availMemory++;
            }
            else if(memory[i] == '0')
            {
                availMemory++;
            }
            else 
            {
                found = 1;
                if(availMemory > largestSpace && availMemory >= space) //Find smallest hole and starting index that fits
                { 
                    //printf("availMemory: %d, index: %d\n", availMemory, holeIndex);
                    flag = 1;
                    largestIndex = holeIndex;
                    largestSpace = availMemory;
                }
                availMemory = 0;
            }

            if(i == 127) //Reached end
            {
                if(availMemory > largestSpace && availMemory >= space) //Find smallest hole and starting index that fits
                { 
                    //printf("availMemory: %d, index: %d\n", availMemory, holeIndex);
                    //printf("flag on\n");
                    flag = 1;
                    largestIndex = holeIndex;
                    largestSpace = availMemory;
                    availMemory = 0;
                }
            }
        }

        if(flag == 0) //No space
        {
            inserted = 0;
            temp2 = pop(memoryQ); //Pop process that's been on queue the longest
            //printf("Removed: %c\n", temp2->thread.pid);
            for(int i = 0; i<MAX; i++)
            {
                if(memory[i] == temp2->thread.pid) //Taking thread out of memory
                    memory[i] = '0';
            }
            temp2->thread.memSwap++;
            if(temp2->thread.memSwap < 3) //Swapped out only once or twice
            {
                push(q, temp2);
            }
        }
        else //A hole fits
        {
            for(int j = largestIndex; j<largestIndex+space; j++) //Adding process in memory
            {
                memory[j] = temp->thread.pid;
            }
            //printMemory(memory);
            flag = 1;
            inserted = 1;
            push(memoryQ, temp);
            printMemStatus(memoryQ, &cumulativeMem, memory, &totalPIDs, &numProcesses, &numHoles);
            avgProcess = ((avgProcess * (totalPIDs-1)) +numProcesses)/totalPIDs;
            avgHoles = ((avgHoles * (totalPIDs-1)) +numHoles)/totalPIDs;
        }
    }
    printf("**Total Loads: %.0lf, average #processes: %.1lf, average #holes: %.1lf, %%cumulativeMem: %.1lf**\n", totalPIDs, avgProcess, avgHoles, cumulativeMem);
}

void nextfit(Queue *q)
{
    char memory[MAX];
    for(int i = 0; i<MAX;i++)
    {
        memory[i] = '0';
    }

    Queue *memoryQ = createQueue(50);
    int inserted = 1, numProcesses = 0, numHoles = 0, lastIndex = 0;
    double cumulativeMem = 0.0, totalPIDs = 0.0, avgProcess = 0.0, avgHoles = 0.0;
    int flag, availMemory = 0;
    Node *temp, *temp2;
    while(q->size != 0) //(temp = pop(q)) != NULL) //While there are still things in the queue able to be popped
    {
        numProcesses = 0;
        numHoles = 0;
        availMemory = 0;
        flag = 0;
        lastIndex = 0;
        if(inserted == 1)
        {
            temp = pop(q);
        }
        int space = temp->thread.memUsg;
       
        for(int i = lastIndex;i<MAX;i++)
        {
            //printf("%d\n", i);
            if(memory[i] == '0')
            {
                availMemory++;
            }
            else if(memory[i] != '0')
            {
                availMemory = 0;
            }

            if(availMemory < space)
            {

            }
            else if(availMemory>=space) //Insert process
            {
                for(int j = i; j>i-space; j--) //Adding process in memory
                {
                    memory[j] = temp->thread.pid;
                }
                //printMemory(memory);
                flag = 1;
                inserted = 1;
                lastIndex = i;

                push(memoryQ, temp);
                printMemStatus(memoryQ, &cumulativeMem, memory, &totalPIDs, &numProcesses, &numHoles);

                avgProcess = ((avgProcess*(totalPIDs-1)) + numProcesses)/totalPIDs;
                avgHoles = ((avgHoles *(totalPIDs-1)) + numHoles)/totalPIDs;
                break;
            }
            
        }
        if(flag == 0) //No space
        {
            inserted = 0;
            temp2 = pop(memoryQ); //Pop process that's been on queue the longest
            for(int i = 0; i<128; i++)
            {
                if(memory[i] == temp2->thread.pid) //Taking thread out of memory
                    memory[i] = '0';
            }
            temp2->thread.memSwap++;

            if(temp2->thread.memSwap < 3) //Swapped out only once or twice
                push(q, temp2);
        }
    }
    printf("**Total Loads: %.0lf, average #processes: %.1lf, average #holes: %.1lf, %%cumulativeMem: %.1lf**\n", totalPIDs, avgProcess, avgHoles, cumulativeMem);
}

void printMemStatus(Queue *memoryQ, double* cumulativeMem, char memory[128], double* totalPIDs, int *numProcesses, int *numHoles)
{
    int used = 0;

    Node* head = memoryQ->head;
    while(head != NULL)
    {
        head = head->prev;
        *numProcesses+=1;
    }
    for(int i = 0; i<MAX-1; i++) //All but last
    {
        
        if(memory[i] == '0' && memory[i+1] != '0')
        {
            *numHoles+=1;
        }

        if(memory[i] == '0')
        {

        }
        else if(memory[i] != '0')
        {
            used++;
        }
    }

    if(memory[127] != '0')
        used++;
    else
        *numHoles += 1;

    double memUsg = (double)used/128 * 100;

    *cumulativeMem = ((*cumulativeMem * *totalPIDs) + memUsg)/ (*totalPIDs+1);

    *totalPIDs += 1;

    printf("PID Loaded, #processes = %d, #holes %d, %%memusage = %.2lf, cumulative %%mem = %.2lf\n", *numProcesses, *numHoles, memUsg, *cumulativeMem);

}



Queue* createQueue( unsigned int limit) 
{
    Queue *queue = (Queue*) malloc(sizeof (Queue));
   if( limit < 0 || limit == 0)
   {
      limit = 999; 
   } 
    if (queue == NULL )
    {
        return NULL;
    }
    else if(queue != NULL){
   
        queue->limit = limit;
        queue->size = 0;
        queue->head = NULL;
        queue->tail = NULL;
    }
    return queue;
}

void push(Queue *queue, void *data) 
{
    /* Bad parameter */
    if (queue == NULL) {
        return;
    }
    if(data == NULL)
    {
        return;
    }
    if(queue->size >= queue->limit) 
    {
        return;
    }
    Node * item = (Node*)data;
    item->prev = NULL;
    if (queue->size == 0) 
    {
        queue->head = item;
        queue->tail = item;

    } 
    else if(queue->size != 0)
    {
        /*adding item to the end of the queue*/
        queue->tail->prev = item;
        queue->tail = item;
    }
    queue->size++;
}

void* pop(Queue *queue) 
{
    /*the queue is empty or bad param*/
    Node *head;

    if (queue->size == 0)
        return NULL;

    head = queue->head;
    queue->head = (queue->head)->prev;
    queue->size--;

    return head;
}

Node * createNode(char pid,int memusage)
{

    Node * node = malloc(sizeof(Node)+2);
    node->thread.pid = pid;
    node->thread.memUsg = memusage;
    node->thread.memSwap = 0;
    return node;
}

int getSize(Queue * queue)
{
    return queue->size;
}