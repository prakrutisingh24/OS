#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <unistd.h>

int forkit(char ***tok, int i){
	execvp(tok[i][0],tok[i]);
}

int main(int argc, char *argv[]){

	
	if(argc != 2){
		printf("Invalid number of arguments\n");
		exit(1);
	}

	FILE *f;
	f = fopen(argv[1],"r");

	if(f == NULL){
		exit(1);
	}

	char *ch = fgetc(f);

	int countline = 1;
	const char EOL = '\n';

	while(ch != EOF){
		if(ch == EOL){
			countline = countline + 1;
		}
		ch = fgetc(f);
	}
	fseek(f, 0, SEEK_SET);
	printf("Number of lines: %d\n", countline);

	char **w = malloc(sizeof(char*)*countline);
	for(int i = 0; i < countline; i++){
		w[i] = malloc(sizeof(char)*200);
	}

	char ***tok = malloc(sizeof(char**)*countline);
	for(int i = 0; i < countline; i++){
		tok[i] = malloc(sizeof(char*)*10);
		for(int j=0; j<20; j++){
			tok[i][j] = malloc(sizeof(char)*5);
		}
	}

	ch = fgetc(f);

	int i = 0;

	while(ch != EOF){
		if(i != 0){
			ch = fgetc(f);
		}
		int pos = 0;
		while(ch != EOL){
			w[i][pos++] = ch;
			ch = fgetc(f);
		}

		char* token = strtok(w[i]," ");
		int position = 0;
		while(token != NULL){
			tok[i][position++]= token;
			token = strtok(NULL, " ");

		}
		tok[i][position] = NULL;

		i++;

		ch = fgetc(f);

		if(ch == EOL){
			break;
		}
		else{
			fseek(f,-1,SEEK_CUR);
		}
	}
	fclose(f);

	pid_t piddit[countline];

	for(int i =0; i< countline;i++){
		if((piddit[i] = fork())<0 ){
			//printf("sad error");
			abort();

		}
		else if(piddit[i] == 0){
		
			forkit(tok,i);
			exit(0);

		}
	}

	int stat;
	pid_t piddd;

	while(countline > 0){
		piddd = wait(&stat);
		--countline;
	}

	return 0;
}

