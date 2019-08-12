#include "tokenizer.h"

Node *allWordsLL = NULL;
//Node *tokenizedWords(char **argv){
HashTable *tokenizeFile(char *filename){	
	//
	/*
	FILE *file = fopen(filename, "r");
	if(file == NULL){
		printf("Empty File\n");
		return 0;
	}
	*/
	int fd = open(filename, O_RDONLY, 0777);
	if (fd < 0) {
		printf("%s is an empty file\n", filename);
		return;
	}
	char letter;		

	/*------------------------------
 	Put all words from string into a linked list
  	--------------------------------*/
	Node *letterLL = NULL;	

	HashTable *hashTable = createTable();
	
	//while(letterCount < wordLen){
	//TODO: Keep track of strings encountered in a linked list, that way traversal...
	//TODO: ...through table is efficent
	//while(fscanf(file,"%c",&letter) == 1){
	while(read(fd, &letter, 1) != 0){
		//char character = argv[wordIndex][letterCount];
		
		//if(isALetter(character) == true) {
		if((letter != ' ') && (letter != '\n')) {
			//Node *letterNode = createLetterNode(character);
			Node *letterNode = createLetterNode(*(&letter));
			addToEndLetterNode(&letterLL, letterNode);
		} else {
			if(letter == ' '){
				HashNode *spaceToken = createHashNode("SPACE");
				insertToTable(spaceToken, &hashTable);
			} else if(letter == '\n'){
				HashNode *newLineToken = createHashNode("NEW-LINE");
				insertToTable(newLineToken, &hashTable);
			}
			//Node *symbolNode = createSymbolIndexNode(letterCount);
			//addToEndIndexOfSymbolNode(&symbolLL, symbolNode);		
			if (letterLL != NULL){
				char *innerWordString = convertLetterLLToArrayOfLetters(letterLL);
				int lenOfInnerWord = strlen(innerWordString);
				Node *innerWord = createWordNode(lenOfInnerWord, innerWordString);
				addToEndWordNode(&allWordsLL, innerWord);
				insertToTable(createHashNode(innerWord->word), &hashTable);
				//printf("Word inserted: %s\n",innerWord->word);
				//printf("Word code = %ld\n",getHashCode((long)innerWord));
				//TODO: Change way to get code
				letterLL = NULL;
			}
		}
		//letterCount++;
	}
	close(fd);
	//fclose(file);
	return hashTable;

}

//Methods: ------------------------

void printArray(char **array, int size){
	int i;
	for(i = 0; i<size; i++){
		//printf("%s\n",array[i]);
	}
}

bool isALetter(char character){
	if(isalpha(character)) {
		return true;
	} else {
		return false;
	}
}

Node *createWordNode(int wordLength, char *word){
    	Node *ptr = malloc(sizeof(Node));
    	ptr->wordLength = wordLength;
    	ptr->word = strdup(word);
	ptr->next = NULL;
    	return ptr;
}

Node *createLetterNode(char letter){
	Node *ptr = malloc(sizeof(Node));	
	ptr->next = NULL;
	ptr->letter = letter;
	return ptr;
}

Node *createSymbolIndexNode(int symbolIndex){
	Node *ptr = malloc(sizeof(Node));
	ptr->symbolIndex = symbolIndex;
	ptr->next = NULL;
	return ptr;
}

void addToEndWordNode(Node **head, Node *target){
	if(*head == NULL) {
		(*head) = target;
		return;
	}
	Node *temp = createWordNode(target->wordLength, target->word);
	Node *ptr = (*head);
	while(ptr->next != NULL){
		ptr = ptr->next;
	}
	ptr->next = temp;
}

void addToEndLetterNode(Node **head, Node *target){
	if (*head == NULL) {
		(*head) = target;
		return;
	}
	Node *temp = createLetterNode(*(&target->letter));
	Node *ptr = (*head);
	while(ptr->next != NULL){
		ptr = ptr->next;
	}
	ptr->next = temp;
}

void addToEndIndexOfSymbolNode(Node **head, Node *target){
	if (*head == NULL) {
		(*head) = target;
		return;
	}
	Node *temp = createLetterNode(*(&target->symbolIndex));
	Node *ptr = (*head);
	while(ptr->next != NULL){
		ptr = ptr->next;
	}
	ptr->next = temp;
}

char *convertLetterLLToArrayOfLetters(Node *head){
	Node *ptr = head;
	int numLetters = 0;
   	while(ptr != NULL){
        	numLetters++;
        	ptr = ptr->next;
    	}
    	
	char *letters = (char *) malloc(sizeof(char)*(numLetters+1));
	letters[numLetters+1] = '\0';    
	
    	ptr = head;
    
    	int i = 0;
    	while (ptr != NULL) {
        	letters[i] = ptr->letter;
        	i++;
        	ptr = ptr->next;
    	}
	return letters;
}
