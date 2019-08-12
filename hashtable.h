#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef int bool;
#define true 1
#define false 0

#define SIZE 10

typedef struct HashNode {
	char *word;   
	int freq;
	long code;
	char* huffmanCode;
	struct HashNode *next; //for chaining
	struct HashNode *left;
	struct HashNode *right;
}HashNode;

typedef struct HashTable{
	HashNode *list[SIZE];
}HashTable;

struct HashTable *createTable();
struct HashNode *createHashNode(char *word);
int getFreqOfToken(char *word, HashTable *table);
long getHashCode(long code);
bool searchTable(struct HashTable *table, char *target);
void insertToTable(HashNode *node, HashTable **table);
void printTable(HashTable *table);
void addToEndHashNode(HashNode **head, HashNode *target);
void combineHashtables(HashTable** table1, HashTable** table2); 
HashNode *addToFrontHashNode(HashNode *head, HashNode *target);
HashNode *pop(HashNode **head);

#endif
