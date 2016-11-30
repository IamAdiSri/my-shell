#include <errno.h> // defines error codes and the appropriate error statement
#include <stdio.h> // standard input output methods
#include <stdlib.h> // standard library
#include <string.h> // string manipulation methods
#include <unistd.h> // standard symbolic constants and types

#include "bg_pid_list.h" // linked list of BGed processes
#include "parse.h" // defines command parser and splitter
#include "call.h" // defines all interface to call commands
#include "prompt.h" // prints the command prompt for current user
#include "background.h" // background checking
#include "colors.h" // defines color for prompt

int main()
{
  char home[1000]; // holds the home (~) directory
  node *head = NULL; // head of linked list for storing backgrounded pids

  getcwd(home, sizeof(home));

  while(1)
  {
    prompt(home); // produces prompt
    int background = 0; // sets to 1 if the command to be executed is to be run in the background

    char query[1000];
    //fscanf(stdin, "%[^\n]s", query); // goes into infinite loop :/
    fgets(query, 1000, stdin); // accept user command
    //fprintf(stdout, "\"%s\"", query);

    if(strstr(query, ";") != NULL) // if semicolon in query
    {
      char **list = multi(query); // separate out the individual commands statement

      int i;
      for(i=0; list[i] != NULL; i++)
      {
        //printf("::%s::\n", list[i]);

        call(list[i], home, &background, &head); // call on each command statement

        free(list[i]);
      }

      free(list);
    }
    else
      call(query, home, &background, &head);

    check_bg_proc(&head);
  }

  return 0;
}
