#pragma once
#ifndef PARSE_H
#define PARSE_H

// takes mutiple ";" separated commands statements and return a list of individual command statements
char **multi(char *query);

// takes a single command statement and breaks it into list of tokens
char **parser(char *query, int *background_ptr);

#endif
