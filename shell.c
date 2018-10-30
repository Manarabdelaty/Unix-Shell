#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


int flag;            // For & sign
char redirect_flag; // For < and >
char file_name[64];
int file;
int saved_stdout; 

int main(void) {

 char *args[64];            // Command arguments tokoens
 char line[1024];          // Command line string

 int  hist_flag = 0;      // History buffer flag
 char hist_buff[1024];   // History Buffer
 char temp_buff[1024];  // Temp buff

 while (1) {
    flag = 0;             // & flag
    redirect_flag = '\0';
    printf("osh>");
    fflush(stdout);        // Flush ouput buffer
    gets(line);           // read input command line
    printf("\n");
    
    
    strcpy(temp_buff, line);


    parse(line, args);    // Seperate line to tokens

    if (strcmp(args[0] ,"exit") ==0)   // If it is an exit command
             exit(0);                  // terminate
     
    if (strcmp(args[0], "!!") == 0){   // If it is a history command
       if (hist_flag){                 // If history buffer isn't empty
           strcpy(temp_buff, hist_buff);
           printchar(hist_buff);
           parse(temp_buff,args);      // Parse history buffer to tokens
           execute(args);              // Execute command in the history buffer
       }  
       else printf("No Commands in the history buffer\n");   // If history buffer is empty
    }

    else {
    hist_flag = 1;                 // set history buffer flag with one
    strcpy(hist_buff, temp_buff);     // Store command line in the history buffer
    execute(args);               // Execute command arguments

    }

 }

  return 0;
}

void redirect_out(){
  
     file = open(file_name, O_APPEND | O_WRONLY);
     if (file < 0){
          printf("Couldn't open file\n");
          exit(1);
      }
     saved_stdout = dup(STDOUT_FILENO);
     
     if (dup2(file,STDOUT_FILENO) < 0){
         printf("Couldn't redirect\n");
 	 close(file);
	 exit(1);
      }

}
void redirect_in(){

 file = open(file_name, O_RDONLY);
 if (file < 0){
        printf("Couldn't open file\n");
         exit(1);
  }

  saved_stdout = dup(STDIN_FILENO);
 if (dup2(file,STDIN_FILENO) < 0) {
        printf("Failed to redirect Input");
	close(file);
	exit(1);
 }

}
void strcopy(char *line, char *hist){
  int k2= 0;
  while (line[k2] != '\0'){
       hist[k2] = line[k2];
       k2++;
   }
   hist[k2] = '\0';
}

void execute(char ** argv){
 int status;
 int pid;
 
 pid = fork();   
 
 if(pid < 0) {
   printf("Failed to create a child process\n");
   exit(1);
 }
 else if (pid ==0){
  
     if (redirect_flag == '>') redirect_out();            // Redirect Ouput

     else if (redirect_flag == '<') redirect_in();    // Redirect Input
               		
    if (execvp(*argv , argv) < 0 ) {
       printf("Failed to execute child process\n");
       exit(1);
    }
    
    if (redirect_flag == '>') {
        fflush(stdout);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
    }
   else if (redirect_flag == '<'){

        fflush(stdin);
        dup2(saved_stdout, STDIN_FILENO);
        close(saved_stdout);
    }

 }
 else if (!flag){
   while(wait(&status) != pid);
 }

}
void parse(char line[], char * argv[]){
    int i=0;
    int j=0;

    while (*line != '\0'){

          while (*line == '\t' || *line== '\n' || *line == ' '){
                     *line++ = '\0';

          }
          if (strcmp(line, "&") ==0){
              flag = 1;
          }
          else if (  *line == '<'||  *line =='>') {
               redirect_flag = *line;
 	  }
          else if (redirect_flag != '\0'){
 		strcpy(file_name ,line);
          }
          else {
          argv[j] = line;
          j++;
          }
          while (*line != '\t' && *line != '\0' && *line != '\n' && *line != ' '){
          line++;
          }
    }

    argv[j] = '\0';
}

void printargs(char * argv[]){
   printf("\n");
   int k2= 0;

   while (argv[k2] != '\0'){
       int k=0;
       while (argv[k2][k] != '\0' ){
            printf("%c" , argv[k2][k]);
            k++;
        }
       k2++;
       printf("\n");
   }
   printf("\n");
}
void printchar(char str[]){
  while(*str != '\0'){
        printf("%c", *str++);
  }
  printf("%c", '\n');
}
