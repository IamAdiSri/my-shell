#include "parse.h"

#include <stdlib.h> // standard library
#include <string.h> // string manipulation methods

char **multi(char *query) // takes mutiple ";" separated commands statements and return a list of individual command statements
{
  char *query_field;
  query_field = strtok(query, ";\n"); // tokenise query and store in query_field with delimiters " " and \n

  char **list = (char **)malloc(100 * sizeof(char *));

  int i = 0;
  while(query_field != NULL)
  {
    list[i] = (char *)malloc(strlen(query_field) + 1);
    strcpy(list[i], query_field);

    i++;

    query_field = strtok(NULL, ";\n");
  }

  list[i] = NULL;
  return list;
}

char **parser(char *query, int *background_ptr) // takes a single command statement and breaks it into list of tokens
{
  char *query_field;
  query_field = strtok(query, " \t\n"); // tokenise query and store in query_field with delimiters " " and \n and tab

  char **list = (char **)malloc(100 * sizeof(char *));

  int i = 0;
  while(query_field != NULL)
  {
    if(strcmp(query_field, "&"))
    {
      list[i] = (char *)malloc(strlen(query_field) + 1);
      strcpy(list[i], query_field);

      i++;
    }
    else
    {
      *background_ptr = 1;
    }
    query_field = strtok(NULL, " \t\n");
  }

  list[i] = NULL;
  return list;
}

void redpipe(char *query, char ***arglist, char ***oplist)
{
  char phrase[100] = "";

  int l = strlen(query);

  int argpt = 0, oppt = 0;

  int i = 0;
  int turnaround = 0;
  char temp[100] = "";

  while (i < l)
  {
    if (query[i] == '>') // >
    {
      if (turnaround == 1)
      {
        strcpy(temp, (*arglist)[argpt - 1]);
        strcpy((*arglist)[argpt - 1], (*arglist)[argpt - 2]);
        strcpy((*arglist)[argpt - 2], temp);
        turnaround = 0;
      }

      (*arglist)[argpt] = (char *)malloc(sizeof(phrase));
      strcpy((*arglist)[argpt], phrase);
      argpt++;

      strcpy(phrase, "");

      (*oplist)[oppt] = (char *)malloc(4 * sizeof(char));
      (*oplist)[oppt][0] = '\0';

      strcat((*oplist)[oppt], ">");

      i++;
      if (query[i] == '>') // >>
      {
        strcat((*oplist)[oppt], ">");
        i++;
      }
      oppt++;
    }
    else if (query[i] == '<') // <
    {
      turnaround = 1;

      (*arglist)[argpt] = (char *)malloc(sizeof(phrase));
      strcpy((*arglist)[argpt], phrase);
      argpt++;

      strcpy(phrase, "");

      (*oplist)[oppt] = (char *)malloc(4 * sizeof(char));
      (*oplist)[oppt][0] = '\0';
      // strcat((*oplist)[oppt], "<");
      strcat((*oplist)[oppt], ">");

      i++;
      if (query[i] == '<') // <<
      {
        // strcat((*oplist)[oppt], "<");
        strcat((*oplist)[oppt], ">");
        i++;
      }
      oppt++;
    }
    else if (query[i] == '|') // |
    {
      if (turnaround == 1)
      {
        strcpy(temp, (*arglist)[argpt - 1]);
        strcpy((*arglist)[argpt - 1], (*arglist)[argpt - 2]);
        strcpy((*arglist)[argpt - 2], temp);
        turnaround = 0;
      }

      (*arglist)[argpt] = (char *)malloc(sizeof(phrase));
      strcpy((*arglist)[argpt], phrase);
      argpt++;

      strcpy(phrase,"");

      (*oplist)[oppt] = (char *)malloc(2 * sizeof(char));
      (*oplist)[oppt][0] = '\0';
      strcat((*oplist)[oppt], "|");

      i++;
      oppt++;
    }
    else
    {
      char c[2];
      c[0] = query[i];
      c[1] = '\0';
      strcat(phrase, c);
      i++;
    }
  }

  (*arglist)[argpt] = (char *)malloc(sizeof(phrase));
  strcpy((*arglist)[argpt], phrase);
  argpt++;

  if (turnaround == 1)
  {
    strcpy(temp, (*arglist)[argpt - 1]);
    strcpy((*arglist)[argpt - 1], (*arglist)[argpt - 2]);
    strcpy((*arglist)[argpt - 2], temp);
    turnaround = 0;
  }

  (*arglist)[argpt] = NULL;
  (*oplist)[oppt] = NULL;

  for(argpt=0; (*arglist)[argpt] != NULL; argpt++)
  {
    if((*arglist)[argpt][0] == ' ') //remove whitespace from beginning
    {
      int k;
      for(k=1; k<= strlen((*arglist)[argpt]); k++)
        (*arglist)[argpt][k-1] = (*arglist)[argpt][k];
    }
    if((*arglist)[argpt][strlen((*arglist)[argpt]) - 1] == ' ') //remove whitespace from end
    {
      (*arglist)[argpt][strlen((*arglist)[argpt]) - 1] = '\0';
    }
  }
}
