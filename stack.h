/*
 * stack.h
 */

#ifndef STACK_H
#define STACK_H

typedef struct stack_elt_tag {
  int numerator, denominator;
  struct stack_elt_tag *next;
} stack_elt;

typedef struct stack_tag {
  stack_elt *first, *last;
  int size;
} stack;

void init_stack (stack *s);
void add (stack *s, int num, int denom);
void pop (stack *s);
void delete_all (stack *s);

#endif