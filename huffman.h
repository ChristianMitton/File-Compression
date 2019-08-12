
#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <errno.h>
#include <unistd.h>

#include "tokenizer.h"
#include "hashtable.h"

typedef int bool;
#define true 1
#define false 0

struct path_to_root {
	struct path_to_root *next;
	HashNode *n;
};

void freeList(Node**);
void printTree(HashNode*);
void *createCodeBook(char *, char*);
void *convertHashtableToLL(HashTable *);
void convertHashNodeLLToArray(HashNode *, HashNode[], int);
HashNode* insertCodeTree(HashNode**, char*, char*);
HashNode* buildTreeWithCodebook(char*);
HashNode *convertHashNodeArrayToLL(HashNode[], int);
void insertionSort(HashNode[], int);
void sortedLLInsert(HashNode**, HashNode *);
void sortedLLInsertDescending(HashNode **, HashNode*);
void printInorder(HashNode*);
HashNode *getPath(HashNode *, struct path_to_root*, HashNode*, HashNode*, HashNode**);
void pList(HashNode*);
void pLetterList(Node*);
char *getCode(HashNode*, HashNode*, HashNode*);
void appendWordToFile(char*, char*, char*);
void compress(char*, char*, char*);
void freeTree(HashNode* root);
HashNode *generateHuffmanTree(char*);
char *searchForWordInCodeBook(char*,char*);
char *decompress(char*, char*);
HashNode *getSortedListOfTokensAndTheirCodes(char*);
void generateCodebook(char*, HashNode*);

#endif
