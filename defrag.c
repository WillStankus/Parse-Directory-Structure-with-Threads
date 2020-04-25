#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include <math.h>

void* TraversePath(void *path);
int GetThreadCount(char *path);
void WriteFilesToMP3();

const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char**paths;

int main (int argc, char* argv[]){

	int thread_count;

	char* dir_head = NULL;
	dir_head = argv[1];	//Set dir_head as first command line argument

	if(dir_head == NULL) {
		printf("ERROR\n");
		return -1;
	}

	thread_count = GetThreadCount(dir_head);	//Count number of sub directories in head
	paths = malloc(sizeof(char*) * 65);

	pthread_t pids[thread_count];			//Create array of threads the size of thread_count

	for (int i = 0; i < thread_count; i++) {
		
		char* npath;
		char* path = "dirs/dir";
		char num;
		num = digits[i % 10];
		char *numPath = &num;

		npath=malloc(strlen(path)+strlen(numPath) + 1);

		strcat(npath,path);		//Concatnate npath and path of subdirectory
		strcat(npath,numPath);		//Concatnate npath with the directory number
		pthread_create(&pids[i], NULL, TraversePath, npath);	//Create thread and pass in the path name
	}
	for (int i = 0; i < thread_count; i++){
		pthread_join(pids[i],NULL);	//Join all the threads
	}

	WriteFilesToMP3();

	return 0;
}
void WriteFilesToMP3() {

	FILE *file;
	FILE *mp3_file;
	
	char c;

	mp3_file = fopen("music.mp3","w");

	for (int i = 0; i < 65; i++) {

		file = fopen(paths[i],"rb");
		
		while (fread(&c,sizeof(char),1,file)){		//Read character by character
			fwrite(&c,sizeof(char),1,mp3_file);	//Write character by character
		}
		fclose(file);
			
	}
	
	fclose(mp3_file);

}
int GetThreadCount(char *path){

	DIR *dir;
	struct dirent *entry;		//Struct of entries
	int dir_head_count = 0;

	
	if ((dir = opendir(path)) != NULL){
	while( (entry = readdir(dir))) {	//Loop for every sub-file

		if (strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..") == 0) continue;	//If name is . or .., dismiss it

		switch (entry->d_type) {
           		case DT_DIR:		//If file is a directory add to count
				dir_head_count++;
                		break;
		}
	}
	return dir_head_count;
	}
	else {
		printf("ERROR: Directory does not exists.\n");
		exit(0);
	}

}

void* TraversePath(void *path){
	
	DIR *dir = NULL;
    	struct dirent *entry;
   	char *npath;
	char *file_path = (char *)path;	//Convert void* to char*
	char *backslash = "/";		//Backslash string
    	if( (dir = opendir(file_path)) == NULL ) return 0;

	while( (entry = readdir(dir))) {	//For every entry in dir

        if (strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..") == 0) continue;	//If . or .., dismiss them

	npath=malloc(strlen(file_path)+strlen(entry->d_name)+2);  
	strcat(npath,file_path);	//Concatnate npath with the path of directory
	strcat(npath,backslash);	//Concatnate npath with the backslash string
	strcat(npath,entry->d_name);	//Concatnate npath with the entry name
	char *token;

        switch (entry->d_type) {

	    case DT_REG: //Its file
		token = strtok(entry->d_name, ".");	//Split string on .
		int index = atoi(token);		//Get first token and set to index
		paths[index - 1] = npath;		//index of path and set to the path in the directory
                break;

            case DT_DIR:	//Its a directory
		TraversePath(npath);	//Recursively call function again with new path
                break;
        }
    	}


}

