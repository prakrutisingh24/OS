#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>


#define HOST_IT_MAX 100
#define LOGIN_NAME_MAX 100

//commandsit THAT BELONG TO A SINGLE PROCESS, ARE CONCATENATED
int concatenate_it(char **inputit, char **commandsit, int count_of_pipe, int length_of_input){
  int i = 0;
  // THIS ENSURES THAT THE LOOP IS RUN ATLEAST ONCE
  while(i < (count_of_pipe + 1)){
    for(int j=0 ; j < length_of_input; j++){
      if(strcmp(inputit[j], "|") != 0){
        strcat(commandsit[i], inputit[j]);
        if(!((j + 1) >= length_of_input)){
          strcat(commandsit[i], " ");
        }
      }else{
        j++;
        break;
      }
    }
    i++;
  }
  return 0;
}

//THE PURPOSE OF THIS FUNCTION IS TO counttt THE NUMBER OF PIPES PER PROCESS
// FINDING THE NUMBER OF PIPES IS HELFUL WHEN THE SCENARIO IS ABOUT redirectionit WITH MULTIPLE PIPES
int countpipes(char **inputit, int lenlen){

  int i = 0, counttt = 0;
  for(i = 0; i <lenlen; i++){
    // WHEN "|" IS FOUND, IT INDICATES THAT A PIPE EXISTS
    if(strcmp(inputit[i], "|") == 0){
      // THIS IS TO KEEP A TRACK OF THE NUMBER OF PIPES THAT EXIST ino THE PROCESS
      counttt+= 1;
    }
  }
  return counttt;
}

//function trims leading and trailing whitespaces
void trim(char *str)
{

    int i;
    int begino = 0;

    int endo = strlen(str) - 1;

    while (isspace((unsigned char) str[begino]))
        begino++;

    while ((endo >= begino) && isspace((unsigned char) str[endo]))
        endo--;

    // Shift all characters back to the start of the string array.
    for (i = begino; i <= endo; i++)
        str[i - begino] = str[i];

    str[i - begino] = '\0'; // Null terminate string.
}


//THE PURPOSE OF THIS FUNCTION IS TO counttt THE NUMBER OF WHITESPACES ino THE inputit STRING THAT IS GIVEN
// THIS IS JUST TO TRACK THE NUMBER OF WHITESPACES
int count_whitespace(char *ino, int lenlen){
  int i = 0;
  int counttt = 0;
  // THIS FUNCTION RUNS FOR "lenlen" NUMBER OF TIMES SUCH THAT IT CAN FIND THE NUMBER OF WHITE SPACES PRESENT ino THE STRING
  for(i = 0; i < lenlen; i++){
    if(ino[i] == ' '){
      counttt++;
    }
  }
  return counttt;
}


//takes an inputit array and returns a char ** that points to
//the array of strings delimited by a space
int stitcho(char *ino, char **outo){
  int i = 1;
  char *temp, cpyit[100];

  sprintf(cpyit, "%s", ino);
  
  temp = strtok(ino, " ");
  sprintf(outo[0], "%s", temp);

  while((temp = strtok( NULL, " ")) != NULL){
    sprintf(outo[i], "%s", temp);
    i++;
  }
  outo[i] = NULL;
  return i;
}


//THE PURPOSE OF THIS FUNCTION SI TO CHECK FOR REDIRCTION AND RETURN ITS CONDITION
// IT BASICALLY RETURNS IF IT IS AN inputit redirectionit OR OUTPUT redirectionit ORINPUT AS WELL AS OUTPUT redirectionit
int redirectionCheck(char *commandu){
  char *outo = strstr(commandu, ">");
  char *ino = strstr(commandu, "<");

  if((outo != NULL) && (ino != NULL)){
    //inputit + OUTPUT
    // < and >
    return 3;
  }else if(outo != NULL){
    //ONLY OUTPUT
    // >
    return 2;
  }else if(ino != NULL){
    //ONLY inputit
    // <
    return 1;
  }else{
    return -1;
  }
}




//THE PURPOSE OF THIS FUNCTION IS TO IMPLEMENT redirection PROCESS
int redirectionit(char *inputit, int state){

  char cpyit[1001], pathit[1001], commandu[1001], name_of_file[1001], **args, *temp;
  int number_of_spaces, i = 0, fd;

  sprintf(cpyit, "%s", inputit);

  temp = strtok(cpyit, " ");
  sprintf(pathit, "%s", temp);

  sprintf(cpyit, "%s", inputit);

  if(state == 1){
    temp = strtok(cpyit, "<");
    sprintf(commandu, "%s", temp);
    commandu[strlen(commandu) - 1] = '\0';
    temp[strlen(temp) - 1] = '\0';

    temp = strtok(NULL, "\0");
    sprintf(name_of_file, "%s", temp);
    trim(name_of_file);

    number_of_spaces = count_whitespace(commandu, strlen(commandu));

    args = calloc((number_of_spaces)+2, sizeof(char *));
    for(i = 0; i < number_of_spaces + 1; i ++){
      args[i] = calloc(strlen(commandu)+10, sizeof(char));
    }

    i = stitcho(commandu, args);

    if((fd = open(name_of_file, O_RDONLY, 0644)) < 0){
      perror("open error");
      return -1;
    }

    dup2(fd, 0);
    close(fd);

    execvp(pathit, (char *const *)args);
    perror("execvp error");
    _exit(EXIT_FAILURE);

  }else if(state == 2){
    temp = strtok(cpyit, ">");
    sprintf(commandu, "%s", temp);
    commandu[strlen(commandu) - 1] = '\0';
    temp[strlen(temp) - 1] = '\0';

    temp = strtok(NULL, "\0");
    sprintf(name_of_file, "%s", temp);
    trim(name_of_file);

    number_of_spaces = count_whitespace(commandu, strlen(commandu));

    args = calloc((number_of_spaces)+2, sizeof(char *));

    for(i = 0; i < number_of_spaces + 1; i ++){
      args[i] = calloc(strlen(commandu)+10, sizeof(char));
    }

    i = stitcho(commandu, args);

    if((fd = open(name_of_file, O_CREAT | O_WRONLY | O_TRUNC, 0644)) < 0){
      perror("open error");
      return -1;
    }

    dup2(fd, 1);
    close(fd);
    
    execvp(pathit, (char *const *)args);
    perror("execvp error");
    _exit(EXIT_FAILURE);    

  }else if(state == 3){
    char filename2[1000];
    int fd2;

    temp = strtok(cpyit, "<");
    sprintf(commandu, "%s", temp);
    commandu[strlen(commandu) - 1] = '\0';
    temp[strlen(temp) - 1] = '\0';

    temp = strtok(NULL, ">");
    sprintf(name_of_file, "%s", temp);
    name_of_file[strlen(name_of_file) - 1] = '\0';
    temp[strlen(temp) - 1] = '\0';
    trim(name_of_file);

    temp = strtok(NULL, "\0");
    sprintf(filename2, "%s", temp);
    trim(filename2);

    number_of_spaces = count_whitespace(commandu, strlen(commandu));

    args = calloc((number_of_spaces)+2, sizeof(char *));
    for(i = 0; i < number_of_spaces + 1; i ++){
      args[i] = calloc(strlen(commandu)+10, sizeof(char));
    }

    i = stitcho(commandu, args);

    if((fd = open(name_of_file, O_RDONLY, 0644)) < 0){
      perror("open error");
      return -1;
    }

    if((fd2 = open(filename2, O_CREAT | O_WRONLY | O_TRUNC, 0644)) < 0){
      perror("open error");
      return -1;
    }

    dup2(fd, 0);
    close(fd);

    dup2(fd2, 1);
    close(fd2);

    execvp(pathit, (char *const *)args);
    perror("execvp error");
    _exit(EXIT_FAILURE);

  }
  return 1;
}


//implements piping
int do_pipo(char **ino, int *countpipes, int i){
  if(i == *countpipes - 1){
    //last process

    int x, number_of_spaces, c;
    char ** args, cpyit[1000];
    sprintf(cpyit, "%s", ino[i]);

    number_of_spaces = count_whitespace(ino[i], strlen(ino[i]));


    args = calloc((number_of_spaces)+2, sizeof(char *));
    for(c = 0; c < number_of_spaces + 1; c++){
      args[c] = calloc(strlen(ino[i])+10, sizeof(char));
    }
    stitcho(cpyit, args);

    if((x = redirectionCheck(ino[i])) < 0){
        execvp(args[0], args);
        perror("execvp error");
        _exit(EXIT_FAILURE);
      }
      redirectionit(ino[i], x);
      return 1;
  }
  if(i < *countpipes){
    int fd[2], x, stat, number_of_spaces, c;
    pid_t pid;
    char ** args, cpyit[1000];

    if(pipe(fd) < 0){
      perror("pipe");
      _exit(EXIT_FAILURE);
    }

    if((pid = fork()) < 0){
      perror("fork");
      _exit(EXIT_FAILURE);
    }

    if(pid != 0){
      dup2(fd[1], 1);
      close(fd[0]);
      ino[i+1] = NULL;

      sprintf(cpyit, "%s", ino[i]);

      number_of_spaces = count_whitespace(ino[i], strlen(ino[i]));

      args = calloc((number_of_spaces)+2, sizeof(char *));
      for(c = 0; c < number_of_spaces + 1; c++){
        args[c] = calloc(strlen(ino[i])+10, sizeof(char));
      }
      stitcho(cpyit, args);

      if((x = redirectionCheck(ino[i])) < 0){
        execvp(args[0], args);
        perror("execvp error");
        _exit(EXIT_FAILURE);
      }

      redirectionit(ino[i], x);
      waitpid(-1, &stat, 0); //wait for all child processes to finish

    }else{
      if(i != *countpipes-1){
        dup2(fd[0], 0);
      }
      close(fd[1]);
      
      i+=1;
      do_pipo(ino, countpipes, i);
    }
  }
  return 1;
}

int main() {
  int i, argc = 0, stat;
  char inp[100];
  char **inputit = (char **)malloc(sizeof(char*)*10);
  for(int i = 0; i < 100; i++){
    inputit[i] = (char *)malloc(sizeof(char)*100);
  }
  char name_of_host[HOST_IT_MAX];
  char name_of_user[LOGIN_NAME_MAX];
  gethostname(name_of_host, HOST_IT_MAX);
  getlogin_r(name_of_user, LOGIN_NAME_MAX);

  char pathit[] = "~";

  while (1) {

  	printf("<%s@%s:%s> ", name_of_user, name_of_host, pathit);
    fgets(inp, 100 ,stdin);

    char* toke = strtok(inp," ");
    int posi = 0;
    while(toke != NULL){
      inputit[posi++]= toke;
      toke = strtok(NULL, " ");
    }

    inputit[posi - 1][strlen(inputit[posi - 1]) - 1] = '\0';
    inputit[posi] = NULL;

    argc = 0;
    if(inputit[0] == NULL){continue;}

    for (int i = 0; inputit[i] != NULL; ++i) { 
      argc++;
    }

    if(strcmp(inputit[0], "cd") == 0){
    	if(argc == 1){
    		char *home = getenv("HOME");
    		if(chdir(home) != 0){
    			perror("chdir error");
    		}
    	}else{
    		if(chdir(inputit[1]) != 0){
    			perror("chdir error");
    		}
    	}
    	continue; //prompt user for next commandu
    }

    if(strcmp(inputit[0], "exit") == 0){
      waitpid(-1, &stat, 0);
      break;
    }

    if (fork() != 0){
    	//parent
    	waitpid(-1, &stat, 0); //wait for all child processes to finish
      continue;
    }else{
    	int numPipes = countpipes(inputit, argc);

      //allocate array to obtain commandsit for each process 
      char **commandsit =  calloc(numPipes +1, sizeof(char*));
      for(i = 0; i < (numPipes +1); i++){
        commandsit[i] = calloc((2*argc + 1), sizeof(char));
      }
      concatenate_it(inputit, commandsit, numPipes, argc);

      if(numPipes == 0){
        //int x = 0;
        int v;

        char *outo = strstr(commandsit[0], ">");
        char *ino = strstr(commandsit[0], "<");

        if((outo != NULL) && (ino != NULL)){
          //both inut and output redirectionit 
          // < and >
          v = 3;
        }
        else if(outo != NULL){
          //output redirectionit only
          // >
          v = 2;
        }
        else if(ino != NULL){
          //inputit redirectionit only
          // <
          v = 1;
        }

        if(v < 0){
          execvp(inputit[0], inputit);
          perror("ERROR OCCURED (EXECVP)");
          _exit(EXIT_FAILURE);
        }
        redirectionit(commandsit[0], v);
      }

      //number of pipes seen exceeds zero
      for(i = 0; i < (numPipes + 1); i++){
        trim(commandsit[i]);
      }
      int counttt = numPipes+1;
      do_pipo(commandsit, &counttt, 0);

    }
  } 
  return 0;
}