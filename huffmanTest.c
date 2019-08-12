#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"
#include "hashtable.h"
#include "huffman.h"

int main(int argc, char **argv){
createCodeBook("gulliver.txt","gulliverCode.txt");

//HashNode *huffmanTree = generateHuffmanTree("blank.txt");
compress("gulliver.txt", "gulliverCode.txt", "gulliver.txt.hcz");
decompress("gulliver.txt.hcz", "gulliverCode.txt");
////decompress("blank.txt.hcz", "testCodeBook.txt");
//
//
////getListOfTokensAndTheirCodes("testCodeBook.txt");
///*
//HashTable *table = createTable();
//// a dog cat button ball and
//HashNode *n1 = createHashNode("a");
//n1->freq = 5;
//HashNode *n2 = createHashNode("dog");
//n2->freq = 9;
//HashNode *n3 = createHashNode("cat");
//n3->freq = 12;
//HashNode *n4 = createHashNode("button");
//n4->freq = 13;
//HashNode *n5 = createHashNode("ball");
//n5->freq = 16;
//HashNode *n6 = createHashNode("and");
//n6->freq = 45;
//insertToTable(n1,&table);
//insertToTable(n2,&table);
//insertToTable(n3,&table);
//insertToTable(n4,&table);
//insertToTable(n5,&table);
//insertToTable(n6,&table);
//
//printTable(table);
////createHuffmanTree(table);
//
////writeFileTest("foo.txt");
////readFileTest("foo.txt");
//
//createHuffmanTree(table);
//*/
//return 0;
}
