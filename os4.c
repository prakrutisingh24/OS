#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_CMD_LEN  128
#define HISTORY_COUNT 20

void execute(char **tok){
	pid_t piddit = fork();
	if(piddit == 0){
		if(execvp(tok[0],tok) != -1){
			printf("Not an Error\n");
		}
	}else{
		wait(NULL);
	}
}


int history(char *hist[], int current)
{
        int i = current;
        int hist_num = 1;

        do {
                if (hist[i]) {
                        printf("%4d  %s\n", hist_num, hist[i]);
                        hist_num++;
                }

                i = (i + 1) % HISTORY_COUNT;

        } while (i != current);

        //("Hi\n");

        return 0;
}

void exec_number(char *hist){

}

int clear_history(char *hist[])
{
        int i;

        for (i = 0; i < HISTORY_COUNT; i++) {
                free(hist[i]);
                hist[i] = NULL;
        }

        return 0;
}


int main(int argc, char *argv[]){

	//char **history = (char**)malloc(10*sizeof(char));
	//for(int i=0; i<10; i++){
	//	history[i] = (char *)malloc(100*sizeof(char));
	//}

    char cmd[MAX_CMD_LEN];
    char *hist[HISTORY_COUNT];
    int i, current = 0;

    for (i = 0; i < HISTORY_COUNT; i++)
            hist[i] = NULL;



//toku is an array that contains the tokens
//toke is a token
	char **toku = (char **)malloc(sizeof(char*)*10);
	for(int i = 0; i < 10; i++){
		toku[i] = (char *)malloc(sizeof(char)*100);
	}

	char buffer[100];

	while(1){

		int i=0;

		//printf("Enter the command: ");
		fgets(buffer, 100, stdin);

		char* toke = strtok(buffer," ");
		int posi = 0;
		while(toke != NULL){
			toku[posi++]= toke;
			toke = strtok(NULL, " ");
		}
		toku[posi - 1][strlen(toku[posi - 1]) - 1] = '\0';
		toku[posi] = NULL;

        /* NULL to `free` is a NO-OP */
        free(hist[current]);

        hist[current] = strdup(buffer);

        current = (current + 1) % HISTORY_COUNT;
        //history(hist, current);

        if (strcmp(buffer, "historybrief") == 0)
                history(hist, current);
        else if (strcmp(buffer, "CLEAR HISTORY") == 0)
                clear_history(hist);
        else if (strcmp(buffer, "stop") == 0)
                exit(0);
        else if (strcmp(buffer, "exec") == 0)
        		printf("hi");
        		execute(toku);
                //clear_history(hist);
		//execute(toku);

	}


	return 0;

}





















