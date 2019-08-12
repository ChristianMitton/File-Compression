#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"
#include "hashtable.h"


typedef int bool;
#define true 1
#define false 0


int main(int argc, char **argv){
	HashTable *tokens = tokenizeFile("blank.txt");
	printTable(tokens);

	/*	
	FILE *file = fopen("blank.txt", "r");

	
	if (file == NULL){
    		printf("Empty File\n");
    		return 0;
	}	

	char letter;
	while(fscanf(file,"%c",&letter) == 1){
	//while(fscanf(file,"%c",&letter) != EOF){
		if(letter == ' '){
			printf("<SPACE>\n");
			continue;
		}
		if(letter == '\n'){
			printf("<NEW LINE>\n");
			continue;
		}
		printf("%c\n",letter);
	}
	fclose(file);
	*/
	/*
	char *x = "This"; 	
	char *y = "This"; 	

	printf("%d\n",(long) x);
	printf("%d\n",(long) y);
	*/	

	return 0;
}
