#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const int MAX_ARGS = 1024;
static const int MAX_OPS = 1024;


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
}

int main()
{
  char query[100];
  scanf("%[^\n]s", query);

  char **arglist = (char **)malloc(sizeof(char *) * MAX_ARGS);
  char **oplist = (char **)malloc(sizeof(char *) * MAX_OPS);

  redpipe(query, &arglist, &oplist);

  int i = 0;
  while(arglist[i] != NULL)
  {
    printf("%s ", arglist[i]);
    free(arglist[i]);
    i++;
  } printf("\n");
  free(arglist);

  i=0;
  while(oplist[i] != NULL)
  {
    printf("%s ", oplist[i]);
    free(oplist[i]);
    i++;
  } printf("\n");
  free(oplist);

  return 0;
}
