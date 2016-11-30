#include <errno.h> // defines error codes and the appropriate error statement
#include <stdio.h> // standard input output methods
#include <stdlib.h> // standard library
#include <string.h> // string manipulation methods
#include <unistd.h> // standard symbolic constants and types
#include <fcntl.h> //file i/o
#include <signal.h>

#include "bg_pid_list.h" // linked list of BGed processes
#include "parse.h" // defines command parser and splitter
#include "call.h" // defines all interface to call commands
#include "prompt.h" // prints the command prompt for current user
#include "background.h" // background checking
#include "colors.h" // defines color for prompt

static const int MAX_ARGS = 1024; // maximum number of individual shell keywords in a single command statement
static const int MAX_OPS = 1024; // maximum number of redirections and pipes in a single command statement

int main()
{
  signal(SIGINT, SIG_IGN);

  char home[1000]; // holds the home (~) directory
  node *head = NULL; // head of linked list for storing backgrounded pids

  int def[3];
  def[0] = dup(0);
  def[1] = dup(1);
  def[2] = dup(2);

  getcwd(home, sizeof(home));

  while(1)
  {
    prompt(home); // produces prompt
    int background = 0; // sets to 1 if the command to be executed is to be run in the background

    char query[1000];
    fgets(query, 1000, stdin); // accept user command

    if(feof(stdin))
    {
      fprintf(stdout, "\n");
      exit(0);
    }

    char **list = multi(query); // get commands separated by ;

    char **arglist = (char **)malloc(sizeof(char *) * MAX_ARGS);
    char **oplist = (char **)malloc(sizeof(char *) * MAX_OPS);

    int fp[2];
    int i, is_piped, killallbg;
    for(i=0, is_piped = 0, killallbg = 0; list[i] != NULL; i++)
    {
      char **arglist = (char **)malloc(sizeof(char *) * MAX_ARGS);
      char **oplist = (char **)malloc(sizeof(char *) * MAX_OPS);

      redpipe(list[i], &arglist, &oplist); // puts all arguments in arglist and all redirs/pipes in oplist

      int out; // file to write output to
      int in; // file to take inout from

      int argpt = 0, oppt = 0;
      while(arglist[argpt] != NULL)
      {
        if(!strcmp(arglist[argpt], "quit"))
          exit(0);

        if(!strcmp(arglist[argpt], "killallbg"))
        {
          killallbg = 1;
          argpt++;
          continue;
        }


        if(is_piped == 2) // there is data in the pipe
        {
          is_piped = 1;
        }

        // printf("ARG: %s ", arglist[argpt]);
        // if(oplist[oppt]!=NULL)
        //   printf("OP: %s\n", oplist[oppt]);
        // else
        //   printf("OP: NULL\n");

        int expt = argpt;

        // if(is_piped)
        // {
        //   close(0);
        //   dup2(fp[0], 0);
        //   close(fp[0]);
        //   is_piped = 0;
        //
        //   dup2(default_out, 1);
        // }


        if(oplist[oppt] != NULL && strstr(oplist[oppt], ">") != NULL)
        {
          in = open(arglist[argpt], O_RDONLY);

          if (in < 0)
          {
            // arglist[argpt] is a command and not a file
            argpt++;

            if(!strcmp(oplist[oppt], ">>"))
              out = open(arglist[argpt], O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
            else
              out = open(arglist[argpt], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);

            close(1);
            dup2(out, 1);
            close(out);
          }
          else
          {
            close(0);
            dup2(in, 0);

            expt++;
            argpt++;
            oppt++;

            if(oplist[oppt] != NULL && strstr(oplist[oppt], ">") != NULL)
            {
              argpt++;
              if(!strcmp(oplist[oppt], ">>"))
                out = open(arglist[argpt], O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
              else
                out = open(arglist[argpt], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);

              close(1);
              dup2(out, 1);
              close(out);
            }
          }
          close(in);
          oppt++;
          // argpt++;
        }

        if(oplist[oppt] != NULL && strstr(oplist[oppt], "|") != NULL) // put into pipe
        {
          is_piped = 2;

          if(pipe(fp) != 0)
            perror("Pipe failed");

          oppt++;
        }
        argpt++;

        // printf("%s:%d\n", __FILE__ ,__LINE__);
        // printf("!!is_piped: %d\n", is_piped);

        call(arglist[expt], home, &background, &head, def, &is_piped, fp);

        dup2(def[0], 0);
        dup2(def[1], 1);
        dup2(def[2], 2);
      }

      int j = 0;
      while(arglist[j] != NULL) // free arglist
      {
        // printf("%s ", arglist[j]);
        free(arglist[j]);
        j++;
      } // printf("\n");
      free(arglist);
      j = 0;
      while(oplist[j] != NULL) // free oplist
      {
        // printf("%s ", oplist[j]);
        free(oplist[j]);
        j++;
      } // printf("\n");
      free(oplist);

      free(list[i]); //free list
    }

    free(list);

    check_bg_proc(&head, &killallbg);
  }

  return 0;
}
