#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include <math.h>
#include <errno.h>

void* TraversePath(void *path);
int GetThreadCount(char *path);
void WriteFilesToMP3();

const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char**paths;

int main (int argc, char* argv[]){

	int thread_count;

	char* dir_head = NULL;
	dir_head = argv[1];

	if(dir_head == NULL) {
		printf("ERROR\n");
		return -1;
	}

	thread_count = GetThreadCount(dir_head);
	paths = malloc(sizeof(char*) * 65);

	pthread_t pids[thread_count];

	for (int i = 0; i < thread_count; i++) {
		
		char* npath;
		char* path = "dirs/dir";
		char num;
		num = digits[i % 10];
		char *numPath = &num;

		npath=malloc(strlen(path)+strlen(numPath) + 1);

		strcat(npath,path);
		strcat(npath,numPath);
		pthread_create(&pids[i], NULL, TraversePath, npath);
	}
	for (int i = 0; i < thread_count; i++){
		pthread_join(pids[i],NULL);
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
		
		while (fread(&c,sizeof(char),1,file)){		
			fwrite(&c,sizeof(char),1,mp3_file);
		}
		fclose(file);
			
	}
	
	fclose(mp3_file);

}
int GetThreadCount(char *path){

	DIR *dir;
	struct dirent *entry;
	int dir_head_count = 0;

	
	if ((dir = opendir(path)) != NULL){
	while( (entry = readdir(dir))) {

		if (strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..") == 0) continue;

		switch (entry->d_type) {
           		case DT_DIR:
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
	char *file_path = (char *)path;
	char *backslash = "/";
    	if( (dir = opendir(file_path)) == NULL ) return 0;

	while( (entry = readdir(dir))) {

        if (strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..") == 0) continue;

	npath=malloc(strlen(file_path)+strlen(entry->d_name)+2);  
	strcat(npath,file_path);
	strcat(npath,backslash);
	strcat(npath,entry->d_name);
	char *token;

        switch (entry->d_type) {

            case DT_REG:
		token = strtok(entry->d_name, ".");
		int index = atoi(token);
		paths[index - 1] = npath;
                break;

            case DT_DIR:
		TraversePath(npath);
                break;
        }
    	}


}

