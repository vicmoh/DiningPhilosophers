/**
 * @file LinkedListAPI.c
 * @author Vicky Mohammad
 * @date 2018
 * @brief A doubly linklist API
 * this is API is a modified verion from CIS*2750 that I made
 */

#ifndef _LIST_API_
#define _LIST_API_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

typedef struct listNode{
    void* data;
    struct listNode* previous;
    struct listNode* next;
} Node;

typedef struct listHead{
    Node* head;
    Node* tail;
    int length;
    void (*deleteData)(void* toBeDeleted);
    int (*compare)(const void* first,const void* second);
    char* (*printData)(void* toBePrinted);
} List;

typedef struct iter{
	Node* current;
} ListIterator;

//functions for init
List initializeList(char* (*printFunction)(void* toBePrinted),void (*deleteFunction)(void* toBeDeleted),int (*compareFunction)(const void* first,const void* second));
List* initializeListPointer(char* (*printFunction)(void* toBePrinted),void (*deleteFunction)(void* toBeDeleted),int (*compareFunction)(const void* first,const void* second));
Node* initializeNode(void* data);
//functions for inserting the data
void insertFront(List* list, void* toBeAdded);
void insertBack(List* list, void* toBeAdded);
void clearList(List* list);
void insertSorted(List* list, void* toBeAdded);
//functions for getting the data
void* deleteDataFromList(List* list, void* toBeDeleted);
void* getFromFront(List list);
void* getFromBack(List list);
char* toString(List list);
//functions for the elements
ListIterator createIterator(List list);
void* nextElement(ListIterator* iter);
int getLength(List list);
void* findElement(List list, bool (*customCompare)(const void* first,const void* second), const void* searchRecord);
//dummy functions for init the list
void* pop(List* list);
char* dummyPrint(void* toBePrinted);
void dummyDelete(void* toBeDeleted);
int dummyCompare(const void* first, const void* second);

#endif
