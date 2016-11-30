#pragma once
#ifndef BG_PID_LIST_H
#define BG_PID_LIST_H

typedef struct node
{
  int v;
  char pname[50];
  struct node *next;
} node;

void insert(int n, node **head);
void show(node *head);
int delete(int n, node **head);

#endif
