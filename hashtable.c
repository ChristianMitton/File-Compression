#include "hashtable.h"

long getHashCode(long code){
	return code % SIZE;
}

struct HashTable *createTable(){
	HashTable *table = malloc(sizeof(HashTable));
	int i = 0;
	for(i = 0; i < SIZE; i++) {
		table->list[i] = NULL;
	}
	return table;
}

struct HashNode *createHashNode(char *word){
	HashNode *newNode = malloc(sizeof(HashNode));
	newNode->word = word;
	newNode->freq = 1;
	newNode->huffmanCode = (char*) malloc(sizeof(word));
	newNode->code = getHashCode((long) word);
	return newNode;
}

/*
   when passing in for 'table' argument, include '&'
   */
void insertToTable(HashNode *node, HashTable **table){
	long code = node->code;

	//new=---------------
	int i = 0;
	for(i = 0; i < SIZE; i++){   
		HashNode *ptr = (* table)->list[i];
		while(ptr != NULL){ 
			if(strcmp(ptr->word,node->word) == 0){
				ptr->freq++;
				return;				
			}

			//printf("%s != %s\n",ptr->word,node->word);
			ptr = ptr->next;
		}
		//printf("\n");

	}
	//new=---------------	

	//If list index is NULL, insert node
	if((* table)->list[code] == NULL){
		//printf("List index is NULL, inserting node at %d\n",code);
		(* table)->list[code] = node;
		return;	
	}



	//If table does contain a node, add to LinkedList chain 
	//(check for dups)
	HashNode *ptr = (* table)->list[code];
	//HashTable *ptr = (* table)->list;
	if(ptr != NULL){
		//printf("A word (%s) is already at index, now looping\n",ptr->word);
	}
	HashNode *prev = NULL;
	while(ptr != NULL){
		//if word already exsists, do nothing
		//printf("ptr->word = %s\n",ptr->word);
		//printf("newNode->word = %s\n",node->word);
		if(strcmp(ptr->word, node->word) == 0){
			printf("DUP found, incrementing freq\n");
			ptr->freq++;
			return;
		}
		prev = ptr;
		ptr = ptr->next;
	}
	//insert word as last item in chained list
	//printf("inserting %s at end of list %s\n", node->word,(*table)->list[code]->word);
	//(*table)->list[code]->next = node;	
	prev->next = node;
	return;
}

void combineHashtables(HashTable** table1, HashTable** table2) {
	int i = 0;
	if (*table1 == NULL || *table2 == NULL) return;
	for (i = 0; i < SIZE; i++) {
		HashNode *ptr = (*table1)->list[i];
		while (ptr != NULL) {
			insertToTable(ptr, table2);
			ptr = ptr->next;
		}	
	}
}

void printTable(HashTable *table){
	printf("-------------------------------\n");
	printf("[[[ HASHTABLE ]]]\n");
	int i = 0;
	for(i = 0; i < SIZE; i++){
		if(table->list[i] == NULL){
			printf("NULL\n");
		} else {
			HashNode *ptr = table->list[i];
			while(ptr != NULL){ 
				//printf("%s at index %d --> ",ptr->word,i);
				printf("(\"%s\" : %d) --> ",ptr->word,ptr->freq);
				ptr = ptr->next;
			}
			printf("\n");
		}
	}
	printf("-------------------------------\n");
}


int getFreqOfToken(char *word, HashTable *table){

	int code = getHashCode((long) word);

	if(table->list[code] == NULL){
		return -1;
	}

	HashNode *ptr = table->list[code];
	while(ptr != NULL){
		if(strcmp(ptr->word,word) == 0){
			return ptr->freq;
		}		
		ptr = ptr->next;
	}



	return 0;
}

void addToEndHashNode(HashNode **head, HashNode *target){
	if(*head == NULL) {
		(*head) = target;
		return;
	}

	HashNode *ptr = (*head);

	while(ptr->next != NULL){
		ptr = ptr->next;
	}
	ptr->next = target;
}

HashNode *addToFrontHashNode(HashNode *head, HashNode *target){
	if(head == NULL){
		head = target;
		head->next = NULL;
		return head;
	}

	//check for dupes
	/*
	   HashNode *ptr = (*head);
	   while(ptr != NULL){
	   if(strcmp(ptr->word,target->word) == 0){
	   return;
	   }
	   ptr = ptr->next;
	   }
	   */
	target->next = head;
	head = target;
	return head;
}

HashNode *pop(HashNode **head){	
	if((*head) == NULL){
		//HashNode *nullNode = createHashNode("NULLNODE");
		//(*head) = nullNode;
		//(*head) = NULL;
		//return NULL;
		return NULL;
	}
	HashNode *temp = (* head);
	(*head) = (* head)->next;
	return temp;
}

void printHashNodeLL(HashNode *head){
	HashNode *ptr = head;
	while(ptr != NULL){
		//printf("i\n");
		printf("(%s : %d)--> ",ptr->word, ptr->freq);
		ptr = ptr->next;
	}	
	printf("\n");
}
