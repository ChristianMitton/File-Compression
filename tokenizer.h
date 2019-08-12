#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "hashtable.h"

#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h>  
#include<errno.h>
#include <unistd.h>

typedef int bool;
#define true 1
#define false 0

//Struct: ------------------------
typedef struct Node{
	int wordLength;
	int symbolIndex;
	char *word;
	char letter;
	struct Node *next;	
} Node;

//Method Signatures: ------------------------
void printArray(char **, int);
Node *createWordNode(int, char *);
Node *createLetterNode(char);
Node *createSymbolIndexNode(int);
void addToEndWordNode(Node**, Node*);
void addToEndLetterNode(Node**, Node*);
void addToEndIndexOfSymbolNode(Node**, Node*);
char *convertLetterLLToArrayOfLetters(Node *);
bool isALetter(char);
HashTable *tokenizeFile(char*);

#endif
