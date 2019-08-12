#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#include "huffman.h"


void trimPath(char* path) {
	int pathLen = strlen(path)-1;
	int i = 0;
	while (path[pathLen-i] != '/' || i == pathLen) {	
		i++;	
	}
	//printf("In trim path, i is %d and pathLen-1 is %d\n", i, pathLen-1);	
	if (i > pathLen) return;
	path[pathLen+1-i] = '\0';
}

int containsBackslash(char* filePath) {
	int pathLen = strlen(filePath) -1;
	char* temp = (char*) malloc(sizeof(filePath));
	strcpy(temp, filePath);
	int i = 0;
	while (temp[pathLen-i] != '/' || i == pathLen) {
		i++;	
	}
	if (i > pathLen) return 0;
	return 1;
}

void verifyAndExecuteFlag(char* flag, char* path, char* codebook) {
	if (strcmp("-b", flag) == 0) {
		//printf("Attempting to open %s\n", path);
		if (path == NULL) return;
		char* savedPath = (char*) malloc(sizeof(path)*1);
		strcpy(savedPath, path);
		int cb = containsBackslash(path);
		if (cb) {
			trimPath(savedPath);
		}
	
		//printf("Path is %s, savedPath is %s\n", savedPath, path);
		if (savedPath != NULL && cb) {
			char* codePath = (char*) malloc(sizeof("HuffmanCodebook") + sizeof(savedPath) + 1);
			strcpy(codePath, savedPath);
			strcat(codePath, "HuffmanCodebook");
			strcat(codePath, "\0");
			//printf("Saved path is %s, and codepath is %s\n", path, codePath);
			createCodeBook(path, codePath);	
		} else {
			createCodeBook(path, "HuffmanCodebook");	
		}
	} else if (strcmp("-c", flag) == 0 && codebook != NULL) {
		char* newPath = (char*) malloc(sizeof(path+1)+5);
		char* savedPath = (char*) malloc(sizeof(path+1));;
		strcpy(savedPath, path);
		strcpy(newPath, path);
		strcat(newPath, ".hcz\0");
		compress(path, codebook, newPath);
	} else if (strcmp("-d", flag) == 0 && codebook != NULL) {
		decompress(path, codebook);	
	} else {
		printf("Flag is invalid, please try again.  Type -h for help\n");
		exit(0);
	}
}

void verifyAndExecuteRecursiveFlag(char* flag, char* path, char* codebook) {
	if (strcmp("-b", flag) == 0) {
		//printf("Attempting to open %s\n", path);
		if (path == NULL) return;
		int fd = open(path, O_RDONLY, 0777);
		int temp = open("temp.txt", O_WRONLY | O_CREAT | O_APPEND, 0777);
		if (fd < 0) return;
		char buff;

		while (read(fd, &buff, 1) != 0) {
			write(temp, &buff, 1);
		}
		close(fd);
		close(temp);

	} else if (strcmp("-c", flag) == 0 && codebook != NULL) {
		if (strcmp(path, "HuffmanCodebook") == 0) return;
		verifyAndExecuteFlag(flag, path, codebook);
	} else if (strcmp("-d", flag) == 0 && codebook != NULL) {
		verifyAndExecuteFlag(flag, path, codebook);		
	} else {
		printf("Flag is invalid, please try again.  Type -h for help\n");
		exit(0);
	}	
}

void buildPath(char* currentPath, char* nextDir, char* newPath) {
	//In case we need the extra space
	if (strcmp(currentPath, ".") == 0) {
		strcpy(newPath, currentPath);
		strcat(newPath, "/");
		strcat(newPath, nextDir);
		strcat(newPath, "\0");
	} else {
		strcpy(newPath, currentPath);
		if (newPath[strlen(newPath)-1] != '/') {
			strcat(newPath, "/");	
		}
		strcat(newPath, nextDir);	
		strcat(newPath, "\0");
	}
}


void openRecursive(char* flag, char* name, char* codebook) {
	DIR* dd = opendir(name);

	struct dirent* status = readdir(dd);
	//printf("Attempting to open %s which is a %s\n", status->d_type, status->d_name);
	while(status != NULL) {

		if (status->d_type == DT_DIR) {
			if (strcmp(status->d_name, ".") == 0 || strcmp(status->d_name, "..") == 0) {
				status = readdir(dd);
				continue;	
			}
			char* path = (char*) malloc(sizeof(status->d_name) + sizeof(name) + 2);
			buildPath(name, status->d_name, path);
	//		printf("New dir path is %s\n", path);
			openRecursive(flag, path, codebook);
	//		printf("We have a directory!%s\n", status->d_name);
			status = readdir(dd);
		} else if (status->d_type == DT_REG) {
			//printf("%s\n", status->d_name);
			char* path = (char*) malloc(sizeof(status->d_name) + sizeof(name) + 2);
			buildPath(name, status->d_name, path);
			//printf("New reg file path is %s\n", path);
			verifyAndExecuteRecursiveFlag(flag, path, codebook);
			status = readdir(dd);
		} else {
			printf("Unkown File\n");	
		}
	}
	if (strcmp(flag, "-b") == 0) {
		verifyAndExecuteFlag(flag, "temp.txt", codebook);
		remove("temp.txt");
	}
	closedir(dd);
	return;
}
int openDirectory(char* name) {
	DIR* dd = opendir(name);
	if (dd == NULL) {
		return 0;
	}
	struct dirent* status = readdir(dd);

	while(status != NULL)
	{           
		//printf("%s\n", status->d_name);
		status = readdir(dd);
	}

	closedir(dd);
	return 1;
}

int main(int argc, char** argv) {
	if (argc < 2) {
		printf("Not enough arguments\n");
		exit(0);	
	}
	char* flag;
	char* path;
	char* codebook;	
	if (strcmp(argv[1], "-R") == 0 || strcmp(argv[2], "-R") == 0) {
		//Recursive call	
		flag = strcmp(argv[2], "-R") == 0 ? argv[1] : argv[2];
		//printf("Recursive flag is %s\n", flag);
		path = argv[3];
		codebook = argv[4];
		//		if (verifyFlag(flag) == 1) {
		//			codebook = argv[4];	
		//		}
		int isDir = openDirectory(path);
		if (!isDir) {
			printf("\033[0;33mWarning! Trying to recursively do something on a file, not a directory, continuing with normal operation for file\n\033[0m"); 
			verifyAndExecuteFlag(flag, path, codebook);	
		} else {
			if (path != NULL) {
				openRecursive(flag, path, codebook);	
			}
		}
	} else {
		char* flag = argv[1];
		char* path = argv[2];
		codebook = argv[3];


		if (path != NULL) {
			int isDir = openDirectory(path);	
			if (!isDir) {
				verifyAndExecuteFlag(flag, path, codebook);
			}
		}

	}

	printf("Task finished\n");
	return 0;
}
