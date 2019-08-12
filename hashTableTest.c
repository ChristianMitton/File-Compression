#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"
#include "hashtable.h"

typedef int bool;
#define true 1
#define false 0

void printTable(HashTable *table);

int main(int argc, char **argv){

	HashTable *t1 = createTable();	
	HashNode *n1 = createHashNode("aa");
	HashNode *n2 = createHashNode("bb");
	HashNode *n3 = createHashNode("cc");
	HashNode *n4 = createHashNode("aa");
	HashNode *n5 = createHashNode("dd");
	HashNode *n6 = createHashNode("ee");
	HashNode *n7 = createHashNode("ff");
	HashNode *n8 = createHashNode("gg");
	HashNode *n9 = createHashNode("hh");
	HashNode *n10 = createHashNode("ii");
	HashNode *n11 = createHashNode("jj");
	HashNode *n12 = createHashNode("This");
	HashNode *n13 = createHashNode("This");
	HashNode *n14 = createHashNode(" ");

	insertToTable(n1, &t1);
	insertToTable(n2, &t1);
	insertToTable(n3, &t1);
	insertToTable(n4, &t1);
	insertToTable(n5, &t1);
	insertToTable(n6, &t1);
	insertToTable(n7, &t1);
	insertToTable(n8, &t1);
	insertToTable(n9, &t1);
	insertToTable(n10, &t1);
	insertToTable(n11, &t1);
	insertToTable(n12, &t1);
	insertToTable(n13, &t1);
	insertToTable(n14, &t1);
	insertToTable(n14, &t1);

	//printTable(t1);

	//printf("%d\n",getFreqOfToken("cc",t1));
	//printf("%d\n",getFreqOfToken(" ",t1));
	HashTable *testTable = createTable();
	HashNode *testNode = createHashNode("aa");
	HashNode *testNode2 = createHashNode("aa");
	HashNode *testNode3 = createHashNode("marco");
	HashNode *testNode4 = createHashNode("jacob");
	HashNode *testNode5 = createHashNode("kelly");
	HashNode *testNode6 = createHashNode("anothony@");
	HashNode *testNode0 = createHashNode("anothony@");
	HashNode *testNode7 = createHashNode("bloop@scarletmail.rutgers.edu");
	HashNode *testNode8 = createHashNode("2");

	//printf("testNode code = %d\n",testNode->code);	
	//printf("testNode2 code = %d\n",testNode2->code);	
	//printf("testNode3 code = %d\n",testNode3->code);	
	
	insertToTable(testNode,&testTable);
	//printTable(testTable);
	
	insertToTable(testNode2,&testTable);
	//printTable(testTable);
	

	insertToTable(testNode3,&testTable);
	//printTable(testTable);

	insertToTable(testNode4,&testTable);
	//printTable(testTable);

	insertToTable(testNode5,&testTable);
	//printTable(testTable);

	insertToTable(testNode6,&testTable);
	//printTable(testTable);
	
	insertToTable(testNode0,&testTable);
	//printTable(testTable);

	insertToTable(testNode7,&testTable);
	//printTable(testTable);

	insertToTable(testNode8,&testTable);
	//printTable(testTable);

	combineHashtables(&t1, &testTable);
	printTable(testTable);
	char *p = "marco";
	int a = getHashCode((long)p);
	
	char *x = "anothony@";
	int y = getHashCode((long)x);

	printf("%s.....\n",testTable->list[a]->word);	
	printf("%s.....\n",testTable->list[y]->word);
	//printf("%s.....\n",testTable->list[y]->next->word);
	printf("%s.....\n",testTable->list[y]->next->word);
	return 0;
}
