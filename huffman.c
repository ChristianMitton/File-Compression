#include "huffman.h"

HashNode *allTokens = NULL;
int numTokens = 0;
Node *letterList;

void *createCodeBook(char *filename, char *codeBook){	

	/***************************************************************************
	  (1.) create huffman tree for file, and store each token in allTokens linked 
	  list for later use
	 ****************************************************************************/
	HashNode *huffmanTree = generateHuffmanTree(filename);
	/*********************************************************************************
	  (2.) Iterate through allTokens, get each tokens respective code, append the token
	  and code to the codebook
	 *********************************************************************************/	
	int fd = open(codeBook, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (fd >= 0) {
		remove(codeBook);	
		fd = open(codeBook, O_WRONLY | O_CREAT | O_APPEND, 0777);

	}
	generateCodebook(codeBook, huffmanTree);
	printf("Codebook complete\n");
	/*
	   HashNode *tokenPtr = allTokens;
	//	printf("printing alltokens:\n");
	//	pList(tokenPtr);
	while(tokenPtr != NULL){
	HashNode *pathList = NULL;
	HashNode *targetPathList = NULL;
	getPath(huffmanTree,NULL,tokenPtr, pathList, &targetPathList);
	char *tokenCode = getCode(tokenPtr, targetPathList, huffmanTree);
	printf("Huffman code for (\"%s\" : %d) as string: %s\n\n",tokenPtr->word,tokenPtr->freq, tokenCode);
	appendWordToFile(tokenCode,"testCodeBook.txt");
	appendWordToFile("\t","testCodeBook.txt");
	appendWordToFile(tokenPtr->word, "testCodeBook.txt");
	appendWordToFile("\n","testCodeBook.txt");
	tokenPtr = tokenPtr->next;
	*/
}

void appendWordToFile(char *word, char* huffmanCode, char* file){
	if (!huffmanCode && !word) return;
	//printf("File path is %s\n", file);
	int fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (fd < 0) {
		printf("File doesn't exist! Please try agian with a different file\n");
		exit(0);	
	}
	//write(fd, "\n", strlen("\n"));     
	if (huffmanCode) {
		write(fd, huffmanCode, strlen(huffmanCode));
		write(fd, "\t", 1);
	}
	if (word) write(fd, word, strlen(word));
	if (huffmanCode) write(fd, "\n", 1);
	/*
	   if (huffmanCode) {
	   write(fd, huffmanCode, strlen(huffmanCode));          
	   write(fd, "\t", 1);
	   }
	   write(fd, word, strlen(word));
	   if (huffmanCode) write(fd, "\n", 1);
	   */
	close(fd); 
}

void generateCodebook(char* filename, HashNode* tree) {
	if (tree->left) {
		generateCodebook(filename, tree->left);	
	}

	if (tree->right) {
		generateCodebook(filename, tree->right);	
	}

	else if (tree->right == NULL && tree->left == NULL) {
		appendWordToFile(tree->word, tree->huffmanCode, filename);	
	}
}

void freeLetterList(Node** head) {
	Node* temp = (*head);
	
	while (temp != NULL) {
		Node* prev = temp;	
		temp = temp->next;	
//		free(prev);
	}
}


void compress(char *targetFile, char *codeBook, char *hczfile){
	//HashNode *huffmanTree = generateHuffmanTree(targetFile);
		
	Node *letterList = NULL;
	char c;
	int fd = open(targetFile, O_RDONLY, 0777); 
	int test = read(fd, &c, 1);
	if (test < 0) {
		printf("%s cannot be opened, check again\n", targetFile);
		return;
	}

	//HashNode* tree = generateHuffmanTree(targetFile);
	//HashTable *table = tokenizeFile(targetFile);
	//printTable(table);
	
	//Check here if file exists, if it does, erase it

	//read(fd2, &c, 1); 
	//printf("c = % c\n", c);
	int hczFd = open(hczfile, O_RDONLY, 0777);

	if (hczFd >= 0) {
		remove(hczfile);	
		hczFd = open(hczfile, O_RDONLY, 0777);
	}
	
	while(test != 0){
//		printf("c in %s == \'%c\'\n",targetFile, c);

		if(c == ' '){
			//printf("c in %s is SPACE\n",targetFile);
			if (letterList == NULL) {	
				char *spaceCode = searchForWordInCodeBook("SPACE", codeBook);
				appendWordToFile(spaceCode, NULL, hczfile);	
				test = read(fd, &c, 1);
				continue;
			}
			char *currentWord = convertLetterLLToArrayOfLetters(letterList);
			//printf("currentWord = \'%s\'\n",currentWord);
			//search this word in code book
			char *spaceCode = searchForWordInCodeBook("SPACE", codeBook);
			char *code = searchForWordInCodeBook(currentWord, codeBook);
			if (code == NULL) {
				test = read(fd, &c, 1);
			}
			//printf("This is the code for %s: %s\n",currentWord, code);
			//printf("This is the code for SPACE: %s\n",spaceCode);
			//freeLetterList(&letterList);
			//printf("I'm after freeing letterlist\n");
			letterList = NULL;
			appendWordToFile(code, NULL, hczfile);
			appendWordToFile(spaceCode, NULL,hczfile);
		}
		else if(c == '\n'){
			if (letterList != NULL) {
				char* currentWord = convertLetterLLToArrayOfLetters(letterList);
				char* code = searchForWordInCodeBook(currentWord, codeBook);
				appendWordToFile(code, NULL, hczfile);	
				//freeLetterList(&letterList);
				letterList = NULL;
			}
//			printf("c in %s = NEW-LINE\n", targetFile);
			char *code = searchForWordInCodeBook("NEW-LINE", codeBook);
			appendWordToFile(code, NULL,hczfile);
		} else {
			//printf("c in %s = %c\n", targetFile, c);
			addToEndLetterNode(&letterList, createLetterNode(c));	
		}	
		test = read(fd, &c, 1);
	}	

	if (letterList) {
		//freeLetterList(&letterList);	
	}
	close(fd);
	printf("Compress finished\n");
}


HashNode* insertCodeTree(HashNode** root, char* word, char* huffmanCode) {
	HashNode* temp = *root;
	HashNode* top = *root;
	char* tempCode;
	strcpy(tempCode, huffmanCode);
	int huffLength = strlen(huffmanCode);	
	//printf("Hufflength is %d\n", huffLength);		
	int i = 0;
	while (i < huffLength) {
		char branch = tempCode[i];

		if (branch == '0') {
			if (temp->left == NULL) {
				HashNode* newNode = (HashNode*) malloc(sizeof(HashNode));
				temp->left = newNode;	
				temp = temp->left;
			} else {
				temp = temp->left;	
			}
		} else if (branch == '1') {
			if (temp->right == NULL) {
				HashNode* newNode = (HashNode*) malloc(sizeof(HashNode));
				temp->right = newNode;
				temp = temp->right;
			} else {
				temp = temp->right;
			}
		}
		i++;	
	}
	//printf("Trying to insert huffmanCode %s\n", huffmanCode);
	temp->word = word;
	temp->huffmanCode = huffmanCode;
	return top;	
}

HashNode* buildTreeWithCodebook(char* codeBook) {
	HashNode* root = (HashNode*)malloc(sizeof(HashNode));

	int fd = open(codeBook, O_RDONLY, 0777);
	char c;
	int fileBuff = read(fd, &c, 1);

	int i = 0;
	Node* letterList = NULL;
	Node* codeList;
	char* word;
	char* huffmanCode;

	while (fileBuff != 0) {
		if (c == '\t') {
			huffmanCode = convertLetterLLToArrayOfLetters(letterList);
			//printf("huffmanCode = \'%s\'\n",huffmanCode);
			letterList = NULL;
			fileBuff = read(fd, &c, 1);
			continue;
		} else if (c == '\n') {
			word = convertLetterLLToArrayOfLetters(letterList);	
			//printf("word = \'%s\'\n",word);
			letterList = NULL;
			fileBuff = read(fd, &c, 1);
			//Add to tree
			root = insertCodeTree(&root, word, huffmanCode);
			//printf("Printint out the tree here!\n");
			continue;
		}
		addToEndLetterNode(&letterList, createLetterNode(c));		
		fileBuff = read(fd, &c, 1);
		i++;	
	}

	//printTree(root);
	//printf("\n----------\n");
	//freeLetterList(&letterList);
	close(fd);
	return root;
}

void freeList(Node** letterList) {
	Node* temp = *letterList;
	while (temp != NULL) {	
		Node* prev = temp;
		temp = temp->next;
	//	free(prev);	
	}
}

char *decompress(char *targetFile, char *codeBook){

	//111001
	//111000
	//11101
	//1111
	//1101
	//1100
	//10
	//0
	//First we build the tree using the codebook, then we parse the file using our tree as a reference

	//Make compress/decompress interchangeable?

	//tokensAndTheirCodesList is a sorted list of token:code pair
	//(Note: HashNode->freq should be seen as HashNode->code)
	HashNode* tree = buildTreeWithCodebook(codeBook);
	int fd = open(targetFile, O_RDONLY, 0777);
	char c;
	int p = read(fd, &c, 1);
	int i = 0;		
	HashNode* curr = tree;
	char* pathCpy = (char*) malloc(sizeof(targetFile) + sizeof("_out"));
	strcpy(pathCpy, targetFile);
	int j = 2;
	
	char* hczCheck = (char*) malloc(sizeof(5));
	strcpy(hczCheck, &targetFile[strlen(targetFile)-4]);
	
	if (strcmp(hczCheck, ".hcz") != 0) {
		printf("File is not compressed, cannot decompress.\nAborting.\n");	
		return;	
	}	
	while (pathCpy[j] != '.') {
		j++;	
	}
	pathCpy[j] = '\0';
	strcat(pathCpy, "_out");	

	int fOut = open(pathCpy, O_WRONLY | O_APPEND | O_CREAT, 0777);
	
	if (fOut >= 0) {
		remove(pathCpy);	
		fOut = open(pathCpy, O_WRONLY | O_APPEND | O_CREAT, 0777);
	}

	while(p != 0){
		if (c == '1' && curr->right) {	
			curr = curr->right;	
		} else if (c == '0' && curr->left) {
			curr = curr->left;	
		}

		if (curr->left == NULL && curr->right == NULL) {
			if (strcmp("SPACE", curr->word) == 0) {
				//printf(" ", curr->word);
				write(fOut, " ", 1);
			} else if (strcmp("NEW-LINE", curr->word) == 0) {
				write(fOut, "\n", 1);
			} else {
				write(fOut, curr->word, strlen(curr->word));
			}

			curr = tree;	
		}
		p = read(fd, &c, 1);		
		i++;
	}
	printf("Decompress complete\n");
	
	//if (tree) freeTree(tree);
	//read(fd, &c, 1);
	close(fd);
	close(fOut);

}

void freeTree(HashNode* root) {
	if (root == NULL) return;
	
	freeTree(root->left);
	freeTree(root->right);

//	free(root);
}

HashNode *getSortedListOfTokensAndTheirCodes(char *codeBook){

	HashNode *wordNodeList = NULL;
	char c;
	int fd = open(codeBook, O_RDONLY, 0777);
	int p = read(fd, &c, 1);
	Node *codeLetterList = NULL;
	Node *tokenLetterList = NULL;
	char *code;
	char *tokenAsWord;
	bool passedTab = false;
	while(p != 0){
		if(c == '\t'){
			code = convertLetterLLToArrayOfLetters(codeLetterList);
			p = read(fd, &c, 1);
			passedTab = true;
			continue;
		}else if(c == '\n'){
			//printf("c in codeBook == '\\n'\n");
			//printf("Converting tokenLetterList to a word...\n");
			tokenAsWord = convertLetterLLToArrayOfLetters(tokenLetterList);
			code = convertLetterLLToArrayOfLetters(codeLetterList);
			//printf("Word: %s\tCode: %s\n",tokenAsWord, code);

			HashNode *node = createHashNode(tokenAsWord);
			node->freq = atoi(code);
			//sortedLLInsert(&wordNodeList, node);	
			sortedLLInsertDescending(&wordNodeList, node);		
			//addToEndHashNode(&wordNodeList,node);

			codeLetterList = NULL;
			tokenLetterList = NULL;
			passedTab = false;
		} else {
			if(passedTab == true){

				addToEndLetterNode(&tokenLetterList, createLetterNode(c));

			} else {
				addToEndLetterNode(&codeLetterList, createLetterNode(c));			
			}
		}

		p = read(fd, &c, 1);
	}
	close(fd);
	
	//if (codeLetterList) freeLetterList(&codeLetterList);
	//pList(wordNodeList);
	return wordNodeList;
	//HconvertHashNodeLLToArray(HashNode *hashNodeLL, HashNode array[], int numTokens);
}

char *searchForWordInCodeBook(char *word, char *codeBook){

	//printf("searching for \"%s\" in %s\n",word,codeBook);
	char c;
	int fd = open(codeBook, O_RDONLY, 0777);
	if (fd < 0) {
		printf("Codebook does not exist!  Try again by using -b on the target file and then running -c\n");
		exit(0);	
	}
	int p = read(fd, &c, 1);
	Node *codeLetterList = NULL;
	Node *tokenLetterList = NULL;
	char *code;
	char *tokenAsWord;
	bool passedTab = false;

	while(p != 0){
		//printf("TARGET WORD: \"%s\"\n",word);
		if(c == '\t'){
			//printf("c in codeBook == '\\t'\n");
			//printf("Converting codeLetterList to a word...\n");
			code = convertLetterLLToArrayOfLetters(codeLetterList);
			p = read(fd, &c, 1);
			passedTab = true;
			continue;	

		} else if(c == '\n'){
			//printf("c in codeBook == '\\n'\n");
			//printf("Converting tokenLetterList to a word...\n");
			tokenAsWord = convertLetterLLToArrayOfLetters(tokenLetterList);
			//printf("Word == %s\n",tokenAsWord);
			//if code is found, add word to the new file : targetFile.hcz
			if(strcmp(tokenAsWord, word) == 0){
				//printf("%s is equal to %s\n",word, tokenAsWord);
				return code;
			} else {
				//printf("TARGET WORD \"%s\" != tokenAsWord: %s\n",word, tokenAsWord);
			}
	//		freeLetterList(&codeLetterList);
	//		freeLetterList(&tokenLetterList);	
			codeLetterList = NULL;
			tokenLetterList = NULL;
			passedTab = false;
		} else {
			if(passedTab == true){

				//printf("adding \'%c\' to tokenLetterList\n",c);
				//pLetterList(tokenLetterList);
				addToEndLetterNode(&tokenLetterList, createLetterNode(c));
				//pLetterList(tokenLetterList);
			} else {
				//printf("adding \'%c\' to codeLetterList\n",c);
				//pLetterList(codeLetterList);
				addToEndLetterNode(&codeLetterList, createLetterNode(c));
				//pLetterList(codeLetterList);				
			}
		}
		
		
		//printf("incrementing...\n");
		p = read(fd, &c, 1);
	}	

	//if (codeLetterList) freeLetterList(&codeLetterList);
	//if (tokenLetterList) freeLetterList(&tokenLetterList);	
	
	close(fd);
}

HashNode* insertUpdatedFreq(HashNode** sortedLL, HashNode* newNode) {
	HashNode* temp = *sortedLL;
	HashNode* prev = *sortedLL;
	HashNode* front = *sortedLL;

	if (temp == NULL) {
		return newNode;	
	}
	while (temp != NULL) {
		if (newNode->freq < temp->freq) {
			if (temp == prev) {
				//At the front of the list
				newNode->next = front;	
				return newNode;
			} else {
				prev->next = newNode;
				newNode->next = temp;
				return front;	
			}	
		}			
		prev = temp;
		temp = temp->next;
	}

	//Else we're inserting at the rear
	prev->next = newNode;	
	return front;
}

void printLL(HashNode* ll) {
	HashNode* temp;
	while (temp != NULL) {
		//printf("%d -> ", temp->freq);
		temp = temp->next;
	}	
}

HashNode* buildTree(HashNode* sortedLL) {
	//Get two lowest nodes
	// Add together
	// Reinsert
	// heapify
	// Repeat until only one
	if (sortedLL == NULL) {
		printf("File is empty! Exiting\n");
		exit(0);	
	}
	HashNode* temp = sortedLL;
	while(temp->next != NULL) {
	//	printLL(temp);
		HashNode* temp1 = temp;
		HashNode* temp2 = temp->next;

		HashNode* newNode = (HashNode*) malloc(sizeof(HashNode));
		newNode->freq = temp1->freq + temp2->freq;
		newNode->left = temp1;

		newNode->right = temp2;
		temp = temp2->next;
		temp2->next = NULL;
		temp1->next = NULL;

		//printf("Freq for new node is %d\n", newNode->freq);
		temp = insertUpdatedFreq(&temp, newNode);
		//	printf("New front is: %d\n Next val is: %d\n", temp->freq, temp->next->freq);
	}

	return temp;
}

void printTree(HashNode* tree) {
	if (tree == NULL) return;

	printTree(tree->left);
	printf("%s:%s\n", tree->word, tree->huffmanCode);
	printTree(tree->right);
}

void makeCodes(HashNode* tree, char arr[], int index) {
	if (tree->left) {
		arr[index] = '0';
		makeCodes(tree->left, arr, index+1);	
	} 

	if (tree->right) {
		arr[index] = '1';
		makeCodes(tree->right, arr, index+1);	
	} 

	else if (tree->right == NULL && tree->left == NULL) {
		arr[index] = '\0';
		memset(tree->huffmanCode, '\0', sizeof(strlen(arr)));
		strcpy(tree->huffmanCode, arr);
		//printf("%s:%s\n", tree->word, tree->huffmanCode);	
		return;
	}
}

HashNode *generateHuffmanTree(char* filename){
	int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (fd <= 0) {
		printf("%s is not valid file to parse into tree\n");	
		return;
	}

	HashTable *table = tokenizeFile(filename);
	//1.) allTokens becomes a linked list of tokens and numTokens is set
	convertHashtableToLL(table); 

	//2.) Convert HashNode LinkedList to an array
	HashNode array[numTokens];
	convertHashNodeLLToArray(allTokens, array, numTokens);

	//3.) Sort array
	insertionSort(array,numTokens);

	//4.) Convert the sorted HashNode array back to a LinkedList
	HashNode *sortedLL = convertHashNodeArrayToLL(array,numTokens);

	//5.) create BST for huffman coding. New functions available for BST use:


	/*------------------------------------
	  Populating huffman tree
	  --------------------------------------*/
	//Create tree with array
	HashNode *huffmanTree = NULL;	
	int i;
	for (i = 0; i < numTokens; i++) {
		HashNode item = array[i];
	//	printf("%s:%d\n", item.word, item.freq);
	}
	huffmanTree = buildTree(sortedLL);
	//printf("Printing out the tree!\n");	
	//printTree(huffmanTree);
	char arr[10000];
	makeCodes(huffmanTree, arr, 0);	
	//	printLL(huffmanTree);
	return huffmanTree;
}

/*
   char *getCode(HashNode *target, HashNode *targetPath, HashNode *huffmanTree){

   Node *code = NULL;
   HashNode *huffmanPtr = huffmanTree;
   HashNode *pathPtr = targetPath->next;

//printf("Trying to print code: \n");
while(huffmanPtr != NULL && pathPtr != NULL){
//printf("Checking LEFT Node...\n");

if (huffmanPtr->left != NULL) {
if (huffmanPtr->left->word != NULL && pathPtr->word != NULL) {
if(strcmp(huffmanPtr->left->word,pathPtr->word) == 0){
if(huffmanPtr->left->freq == pathPtr->freq){
Node *newLetter = createLetterNode('0');
addToEndLetterNode(&code, newLetter);
huffmanPtr = huffmanPtr->left;
pathPtr = pathPtr->next;
continue;
}
}
}
} else {
huffmanPtr = huffmanPtr->left;
pathPtr = pathPtr->next;	
}
//printf("Checking RIGHT Node...\n");
if (huffmanPtr->right != NULL) {
if (huffmanPtr->right->word != NULL && pathPtr->word != NULL) {
if(strcmp(huffmanPtr->right->word,pathPtr->word) == 0){
if(huffmanPtr->right->freq == pathPtr->freq){
Node *newLetter = createLetterNode('1');
addToEndLetterNode(&code, newLetter);
huffmanPtr = huffmanPtr->right;
pathPtr = pathPtr->next;
continue;
}
}
}	
} else {
huffmanPtr = huffmanPtr->right;
pathPtr = pathPtr->next;	
}
}

if(strcmp(huffmanPtr->left->word,target->word) == 0){
Node *newLetter = createLetterNode('0');
addToEndLetterNode(&code, newLetter);
} 
if(strcmp(huffmanPtr->right->word,target->word) == 0){
Node *newLetter = createLetterNode('1');
addToEndLetterNode(&code, newLetter);
}
//pLetterList(code);

char *stringCode = convertLetterLLToArrayOfLetters(code);
//printf("Code as string: %s\n",stringCode);
return stringCode;
}
*/

HashNode *getPath(struct HashNode *n, struct path_to_root *p, HashNode *target, HashNode *pathList,HashNode **permanentList){
	struct path_to_root pnext;

	if(pathList != NULL){
		return pathList;
	}

	if(!n){
		return pathList;
	}

	if(!n->left && !n->right){
		struct path_to_root *temp = p;
		struct path_to_root *prev = NULL;

		char *word = target->word;
		if (p->n != NULL) {
			if (p->n->left != NULL) {
				if (p->n->left->word != NULL) {
					if(strcmp(p->n->left->word, word)==0){
						temp = p;
						while(temp != NULL){

							pathList = addToFrontHashNode(pathList, temp->n);			
							temp = temp->next;
						}
						(*permanentList) = pathList;
					}
				}
			}
		}
		if (p->n != NULL) {
			if (p->n->right != NULL) {	
				if (p->n->right->word != NULL) {
					if(strcmp(p->n->right->word, word)==0){				
						temp = p;
						while(temp != NULL){	
							pathList = addToFrontHashNode(pathList, temp->n);		
							temp = temp->next;
						}
						(*permanentList) = pathList;
					}
				}
			}
		}
	}
	pnext.n=n;
	pnext.next=p;

	getPath(n->left, &pnext, target, pathList, permanentList);
	getPath(n->right, &pnext, target, pathList, permanentList);
}

void pList(HashNode *list){
	HashNode *ptr = list;
	//printf("pathList: ");
	while(ptr != NULL){
	//	printf("(%s : %d) --> ", ptr->word, ptr->freq);
		ptr = ptr->next;
	}
	//printf("\n");
}

void pLetterList(Node *list){
	//printf("PRINTING LINKED LIST\n");
	Node *ptr = list;
	//printf("LetterList: ");
	while(ptr != NULL){
	//	printf("%c -->", ptr->letter);
		ptr = ptr->next;
	}
	//printf("\n");
}


void sortedLLInsert(HashNode **head, HashNode *target){
	if((*head) == NULL){
		(*head) = target;
		return;
	}

	if((*head)->freq >= target->freq){
		target->next = (*head);
		(*head) = target;
		return;
	}

	HashNode *ptr = (*head);
	HashNode *prev = NULL;
	while(ptr != NULL){
		if(ptr->freq >= target->freq){
			HashNode *temp = ptr;
			prev->next = target;
			target->next = temp;
			return;			
		}

		prev = ptr;
		ptr = ptr->next;
	}

	prev->next = target;

}

void sortedLLInsertDescending(HashNode **head, HashNode *target){
	if((*head) == NULL){
		(*head) = target;
		return;
	}

	if((*head)->freq <= target->freq){
		target->next = (*head);
		(*head) = target;
		return;
	}

	HashNode *ptr = (*head);
	HashNode *prev = NULL;
	while(ptr != NULL){
		if(ptr->freq <= target->freq){
			HashNode *temp = ptr;
			prev->next = target;
			target->next = temp;
			return;			
		}

		prev = ptr;
		ptr = ptr->next;
	}

	prev->next = target;


}

void insertionSort(HashNode arr[], int n) { 
	int i, j;
	HashNode *key = createHashNode(""); 
	for (i = 1; i < n; i++) { 
		key->word = arr[i].word; 
		key->freq = arr[i].freq; 
		key->code = arr[i].code; 
		j = i - 1; 

		while (j >= 0 && arr[j].freq > key->freq) { 
			arr[j + 1].word = arr[j].word; 
			arr[j + 1].freq = arr[j].freq;
			arr[j + 1].code = arr[j].code;
			j = j - 1; 
		} 
		arr[j + 1].word = key->word; 
		arr[j + 1].freq = key->freq; 
		arr[j + 1].code = key->code; 
	} 
} 


HashNode *convertHashNodeArrayToLL(HashNode array[], int numTokens){
	HashNode *head = NULL;
	int i;
	for(i = 0; i < numTokens; i++){
		HashNode *temp = createHashNode("");
		temp->word = array[i].word;
		temp->freq = array[i].freq;
		temp->code = array[i].code;
		addToEndHashNode(&head, temp);
	}
	return head;
}

void convertHashNodeLLToArray(HashNode *hashNodeLL, HashNode array[], int numTokens){
	int i = 0;
	HashNode *ptr = hashNodeLL;

	while(i < numTokens){
		array[i].word = ptr->word;
		array[i].freq = ptr->freq;
		array[i].code = ptr->code;
		array[i].next = ptr->next;
		array[i].left = ptr->left;
		array[i].right = ptr->right;
		i++;
		ptr = ptr->next;
	}
}

void *convertHashtableToLL(HashTable *table){
	HashNode *ptr = table->list[0];
	int i = 0;
	while(i < SIZE){
		ptr = table->list[i];
		if(ptr != NULL){
			HashNode *ptr2 = ptr;
			while(ptr2 != NULL){
				//printf("%s : %d\n",ptr2->word, ptr2->freq);
				HashNode *temp = createHashNode(ptr2->word);
				temp->freq = ptr2->freq;
				temp->next = NULL;
				temp->code = ptr2->code;
				temp->left = ptr2->next;
				temp->right = ptr2->right;
				addToEndHashNode(&allTokens, temp);
				ptr2 = ptr2->next;
				numTokens++;
			}
		}

		i++;
	}
}
