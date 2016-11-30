#include "background.h"

#include <stdio.h> // standard input output methods
#include <sys/wait.h> //defines wait() and waitpid()

void check_bg_proc(node **head)
{
  node *temp = *head;
  int status;
  int wait_ret;
  while(temp != NULL)
  {
      wait_ret = waitpid(temp->v, &status, WNOHANG);

      if(wait_ret != 0 && wait_ret !=  -1)
      {
        if(WIFEXITED(status))
        {
          fprintf(stderr, "\n[-] %d %s exited successfully\n", temp->v, temp->pname);

          delete(temp->v, head);
        }
        else if(WIFSIGNALED(status))
        {
          fprintf(stderr, "\n[-] %d %s interrupted\n", temp->v, temp->pname);

          delete(temp->v, head);
        }
      }

      temp = temp->next;
  }
}
