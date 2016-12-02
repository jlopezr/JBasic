//
//  eval.h
//  JBasic
//
//  Created by Juan López Rubio on 15/5/16.
//  Copyright © 2016 Juan López Rubio. All rights reserved.
//

#ifndef eval_h
#define eval_h

#include <stdio.h>
#include "keywords.h"

#define ATOM_INT    0
#define ATOM_BOOL   1
#define ATOM_STRING 2
#define ATOM_OP     3

#define OP_ADD      0
#define OP_SUB      1
#define OP_MUL      2
#define OP_DIV      3

typedef struct {
    unsigned char type;
    union {
        int integer;
        char boolean;
        char* string;
        instr_impl* operation;
    };
} Atom;

#define STACK_MAX   64

typedef struct {
    Atom stack[STACK_MAX];
    unsigned char stack_pos;
} Stack;

void stack_init(Stack* s);
int push(Stack* s, Atom a);
int peek(Stack* s, Atom* atom);
int pop(Stack* s, Atom* atom);
void print_stack(Stack* s);

char rpn_to_infix(char* code);

extern Stack input_stack;
extern Stack eval_stack;
extern Stack o_stack;
extern Stack op_stack;

#endif /* eval_h */
