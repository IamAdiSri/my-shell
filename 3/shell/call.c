#include "call.h"
#include "colors.h"
#include "parse.h"

#include <fcntl.h> // defines file control functions
#include <stdio.h> // standard input output methods
#include <stdlib.h> // standard library
#include <string.h> // string manipulation methods
#include <sys/wait.h> //defines wait() and waitpid()
#include <unistd.h> // standard symbolic constants and types

void call_cd(char *path, char *home) // changes dierctory to path
{
  if(path == NULL || !strcmp(path, "~"))
    call_cd(home, home);
  else if (strstr(path, "~") != NULL)
  {
    char new[100];
    strcpy(new, home);
    strcat(new, path + 1);

    if(chdir(new))
      perror("ERROR");
  }
  else if(chdir(path))
    perror("ERROR");
}

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
    close(1);

  return;
}

void call_echo(char **list) // carries out echo
{
  char *no_newline = "-n";
  int n = 0;
  char *backslash = "-e"; // to be implemented
  int e = 0;

  int i = 1;
  if(!strcmp(list[1], no_newline))
  {
    n = 1;
    i++;
  }

  if(!strcmp(list[1], backslash)) // t o be implemented
  {
    e = 1;
    i++;
  }

  char temp[100];
  while(list[i] != NULL)
  {
    strcpy(temp, list[i]);
    i++;
    fprintf(stdout, "%s", temp);

    if(list[i] != NULL)
      fprintf(stdout, " ");
  }

  if(n == 0)
    fprintf(stdout, "\n");
}

void sig_handler()
{
  kill(0, SIGKILL);
}

void call_exec(char **list, int *background_ptr, node **head_ptr, int *is_piped, int fp[]) // executes shell commands
{
  int pid = fork();

  if(pid == 0)
  {
    if(*is_piped == 1) //in
    {
      dup2(fp[0], 0);
      close(fp[0]);
      close(fp[1]);
      *is_piped = 0;
    }
    if(*is_piped == 2) //out
    {
      close(fp[0]);
      dup2(fp[1], 1);
      // close(fp[1]);
      // close(1);
    }

    if(*background_ptr != 1)
    {
      signal(SIGINT, &sig_handler);
    }
    if(execvp(list[0], list))
    {
      fprintf(stderr, "ERROR: \"%s\" command unrecognised\n", list[0]);
    }
  }
  else
  {
    if(*background_ptr != 1)
    {
      waitpid(pid, NULL, 0);
    }
    else if(*background_ptr == 1)
    {
      fprintf(stdout, "[+] %d Started\n", pid);
      usleep(70); // waiting for 0.00007 seconds to prevent race condition
      insert(pid, head_ptr);
    }
  }

  *background_ptr = 0;
}

void call_pinfo(char *pid_str)
{
  int pid = atoi(pid_str);

  char path[30] = "";
  strcat(path, "/proc/");
  strcat(path, pid_str);
  strcat(path, "/status");

  int fp = open(path, O_RDONLY);
  if(fp == -1)
  {
    fprintf(stderr, "ERROR : Process with PID %s is does not exist\n", pid_str);
    return;
  }

  char file[10000] = "";

  read(fp, file, sizeof(file));

  char *line;
  line = strtok(file, "\n");

  int line_num = 0;
  while(line != NULL && line_num < 17)
  {
    if(line_num == 0 || line_num == 4 || line_num == 16)
    {
      fprintf(stdout, "%s\n", line);
    }
    line = strtok(NULL, "\n");
    line_num++;
  }

  close(fp);

  memset(path, 0, sizeof(path));
  strcat(path, "/proc/");
  strcat(path, pid_str);
  strcat(path, "/stat");

  fp = open(path, O_RDONLY);

  memset(file, 0, sizeof(file));
  read(fp, file, sizeof(file));

  // memset(line, 0, sizeof(line));
  line = strtok(file, " ");

  line_num = 0;
  while(line != NULL && line_num < 3)
  {
    if(line_num == 2)
    {
      fprintf(stdout, "Process status: %s\n", line);
    }
    line = strtok(NULL, " ");
    line_num++;
  }

  close(fp);

  memset(path, 0, sizeof(path));
  strcat(path, "/proc/");
  strcat(path, pid_str);
  strcat(path, "/exe");

  char exec_path[50] = "";
  int len = readlink(path, exec_path, sizeof(exec_path));
  exec_path[len] = '\0';

  fprintf(stdout, "Executable path: %s\n", exec_path);
}

void call(char *query, char *home, int *background_ptr, node **head_ptr, int def[], int *is_piped, int fp[]) // calls the other call_* functions
{
  if(!strcmp(query, "\n") || !strcmp(query, ""))
    return;

  char *cd = "cd";      // defining commands
  char *pwd = "pwd";
  char *echo = "echo";
  char *pinfo = "pinfo";

  char query_cpy[1000] = "";
  strcpy(query_cpy, query);

  char **list = parser(query_cpy, background_ptr);

  if(!strcmp(list[0], cd)) // calls cd
  {
    if(list[1] != NULL)
      call_cd(list[1], home);
    else
      call_cd(home, home);
  }
  else if(!strcmp(list[0], pwd)) //calls pwd
  {
    call_pwd(home, is_piped, fp);

    char f_content[102];
    //read(fp[0], f_content, 100);
    dup2(def[1], 1);
    //printf("Contents of fp[0]: %s\n", f_content);

    fprintf(stdout, "\n");
  }
  else if(!strcmp(list[0], echo)) // calls echo
  {
    call_echo(list);
  }
  else if(!strcmp(list[0], pinfo)) // calls pinfo
  {
    call_pinfo(list[1]);
  }
  else
  {
    call_exec(list, background_ptr, head_ptr, is_piped, fp);
  }

  // free list
  int i;
  for(i = 0; list[i] != NULL; i++)
  {
    free(list[i]);
  }
  free(list);
}
