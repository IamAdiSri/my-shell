#include "prompt.h"
#include "colors.h"
#include "call.h"

#include <pwd.h> // defines struct passwd
#include <stdio.h> // standard input output methods
#include <string.h> // string manipulation methods
#include <sys/utsname.h> // defines the utsname struct
#include <unistd.h> // standard symbolic constants and types

void prompt(char *home) // generates the command prompt as a string
{
  char string[1000] = "";
  struct passwd *name;
  struct utsname node;
  if( (name = getpwuid(getuid())) != NULL)
  {
    fprintf(stdout, RED "%s@" RESET, name->pw_name);
  }
  else
  {
    fprintf(stdout, RED "<_error_>@" RESET);
  }

  if(!uname(&node))
  {
    fprintf(stdout, RED "%s" RESET, node.nodename);
  }
  else
  {
    fprintf(stdout, RED "<_error_>" RESET);
  }

  // char hostname[128];
  // gethostname(hostname, sizeof hostname);  // not using as gethostname actually just calls uname

  fprintf(stdout, ":");

  char cwd[1000];
  getcwd(cwd, sizeof(cwd));
  if(strcmp(cwd, home) == 0)
    fprintf(stdout, YEL "~" RESET);
  else
    call_pwd(home);

  fprintf(stdout, " $> ");
}
