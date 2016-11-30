#include "bg_pid_list.h"

#pragma once
#ifndef CALL_H
#define CALL_H

// changes dierctory to path
void call_cd(char *path, char *home);

 // carries out pwd
void call_pwd(char *home);
 // carries out echo
void call_echo(char **list);
// executes shell commands
void call_exec(char **list, int *background_ptr, node **head_ptr);
void call_pinfo(char *pid_str);
// calls the other call_* functions
void call(char *query, char *home, int *background_ptr, node **head_ptr);

#endif
