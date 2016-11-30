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
  query_field = strtok(query, " \n"); // tokenise query and store in query_field with delimiters " " and \n

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
    query_field = strtok(NULL, " \n");
  }

  list[i] = NULL;
  return list;
}
