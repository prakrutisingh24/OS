#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 


int pipe[2];
pid_t process;
char input1[200];




void forkit(){
  process = fork();
}



void parentchild(){

  if(process<0){
    //SCAMMMM
    printf("Sorry, we cant fork it\n");
  }

  else if(process>0){
    //parent process
    printf("you are in the parent process\n");

  }

  else if(process==0){
    //child process
    printf("you are in the child process\n");




  }
}





































while(1){
 if(strcmp(input_str, ex) == 0 || strcmp(input_str, qu) == 0){
  exit(0);
 }else{
  int len = 0;
  for(int i = 0; i < 100; i++){
   if(input_str[i] == 0){
    len = i + 1;
    break;
   }
  }

  pid_t p = fork();

  if (p > 0){
   write(fd1[1], input_str, len);

   fgets(input_str, 100, stdin);
   for(int x = 0; x <= 100; x++){
    if(input_str[x] == '\n'){
     input_str[x] = '\0';
     break;
    }
   }
   wait(NULL);
  }else{
   char concat_str[100];
   read(fd1[0], concat_str, len + 1);
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