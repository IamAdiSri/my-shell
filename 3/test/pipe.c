#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // standard symbolic constants and types
#include <fcntl.h> //file i/o

#define RED   "\x1B[31m" // colors for printing
#define YEL   "\x1B[33m"
#define RESET "\x1B[0m"

void call_pwd(char *home, int *is_piped, int fp[]) // carries out pwd
{
  char string[1000] = "";

  char cwd[1000];
  getcwd(cwd, sizeof(cwd));

  if(strstr(cwd, home) != NULL) // if home in cwd
  {
    if(strlen(cwd) != strlen(home))
    {
      strcat(string, "~");
      strcat(string, cwd + strlen(home));
    }
    else
      strcat(string, cwd);
  }
  else  // cwd is root or cwd is in home but not equal
  {
    strcat(string, cwd);
  }

  if (*is_piped == 2)
  {
    //close(fp[0]);
    //close(fp[1]);
    dup2(fp[1], 1);
    close(fp[1]);
  }

  fprintf(stdout, YEL "%s" RESET, string);
  //write(1, string, strlen(string));

  if (*is_piped == 2)
  {
    close(1);
    //close(fp[1]);
    //close(fp[0]);
  }

  return;
}

int main()
{
  int def[3] = {0, 0, 0};
  dup2(0, def[0]);
  dup2(1, def[1]);
  dup2(2, def[2]);

  char home[1000]; // holds the home (~) directory
  getcwd(home, sizeof(home));

  int is_piped = 2;
  int fp[2];
  if(pipe(fp) != 0)
    perror("Pipe failed");

  //dup2(def[1], 1);
  //close(def[1]);

  //printf("%d, %d\n", fp[0], fp[1]);

  call_pwd(home, &is_piped, fp);

  // char f_content[102];
  // read(fp[1], f_content, 100);
  // printf("Contents of fp[1]: %s\n", f_content);
  // read(1, f_content, 100);
  // printf("Contents of 1: %s\n", f_content);
  // read(fp[0], f_content, 100);
  // printf("Contents of fp[0]: %s\n", f_content);

  //close(def[1]);
  dup2(def[1], 1);

  fprintf(stdout, "~\n");

  return 0;
}
