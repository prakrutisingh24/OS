#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
//

int pipee[2];
pid_t p;
char input[200];
//char input2[200];

void work(char *input){
    for(int i=0;i<=200;i++){
        if(input[i]=='\n'){
            input[i] = '\0';
            break;
        }
    }

}
/*
void part2(char *input, int *pipee){
    p=fork();


    //while(1){
       // p = fork();

        if(p>0){
            //parent process

            close(pipee[0]);
            write(pipee[1],input,strlen(input)+1);
            close(pipee[1]);
            fgets(input, 200, stdin);
            work(input);
            wait(NULL);
        }
        else if(p==0){
            //child process
            close(pipee[1]);
            read(pipee[0], input,strlen(input)+1);
            char** toki = (char**)malloc(20*sizeof(char*));
            for(int i=0;i<10;i++){
                toki[i]=(char*)malloc(20*sizeof(char));
            }

            printf("%s",input);

            //taki is the token for tokeniser stuff
            char* taki = strtok(input, " ");
            int pos = 0;
            while (taki!=NULL){
                toki[pos++] = taki;
                taki = strtok(NULL, " "); 

            }
            toki[p] = "\0";
            //exit(0);

            printf("%s",toki[0]);
            close(pipee[1]);
            close(pipee[0]);
            execvp(toki[0], toki);
        }
    //}
}


int main(int argc, char* argv[]){

    if(pipe(pipee) == -1){
        printf("PIPE FAILED, SORRY\n");
        exit(1);
    }
    char input[200];
    fgets(input,200,stdin);
    work(input);
    part2(input, pipee);
}
*/

void part2(char *input, int *pipee){
    // Allocate memory for the first read
    char **token = (char **)malloc(10*sizeof(char *));
    for(int i = 0; i < 10; i++){
        token[i] = (char *)malloc(100*sizeof(char));
    }

    p=fork();
    while(1){
        if(strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0){
            exit(0);
        }else{
            int len = 0;
            for(int i = 0; i < 200; i++){
                if(input[i] == 0){
                    len = i + 1;
                    break;
                }
            }

            pid_t p = fork();

            if (p > 0){
                write(pipee[1], input, len);

                fgets(input, 200, stdin);
                for(int x = 0; x <= 200; x++){
                    if(input[x] == '\n'){
                        input[x] = '\0';
                        break;
                    }
                }
                wait(NULL);
            }else{
                char concat_str[200];
                read(pipee[0], concat_str, len + 1);
                char* tok = strtok(concat_str, " ");
                int pos = 0;
                while (tok != NULL) {
                    token[pos++] = tok;
                    tok = strtok(NULL, " ");
                }

                int p = 0;
                for(int j = 0; token[j][0] != 0; j++){
                    p = j + 1;
                }
                token[p] = NULL;
                execvp(token[0], token);
            }
        }
    }
}

int main(int argc, char* argv[]){
    if(pipe(pipee) == -1){
        printf("PIPE FAILED, SORRY\n");
        exit(1);
    }
    char input[200];
    fgets(input,200,stdin);
    work(input);
    part2(input, pipee);
}