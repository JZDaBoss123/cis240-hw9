/*
 * stack.c
 */

#include "stack.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


void init_stack (stack *s)
{
  s->first = NULL;
  s->last = NULL;
  s->size = 0;
}

void add (stack *s, int num, int denom)
{
  stack_elt *elt;
  elt = malloc (sizeof(*elt));

  if (elt == NULL) {
    delete_all (s);
    exit (1);
  }

  elt->numerator = num;
  elt->denominator = denom;
  elt->next = NULL;

  if (s->first) {
    elt->next = s->first;
  } else {
    s->last = elt;
  }
  s->first = elt;

  s->size++;
}

void pop (stack *s)
{
  stack_elt *elt;
  
  if (s->first) {
    elt = s->first;

    if (s->first == s->last) {
      s->first = NULL;
      s->last = NULL;
    } else {
      s->first = elt->next;
    }
    free(elt);
    s->size--;
  }
}

void delete_all (stack *s)
{
  while (s->first) {
    pop (s);
  }
}
