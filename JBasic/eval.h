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

enum AtomType {
    ATOM_INT,         // Integer
    ATOM_BOOL,        // Boolean, not clear if needed. CPC Basic uses 0 false, -1 true
    ATOM_STRING,      // String
    ATOM_OP,          // Operation
    ATOM_TMP_STRING   // String that should be deleted at the end of the processing
};

typedef struct {
    unsigned char type;
    union {
        int integer;
        char boolean;
        char* string;
        instr_impl* operation;
    };
} Atom;

char atom_to_string(Atom* atom);

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

int rpn_to_infix(Line* line, int i);

extern Stack input_stack;
extern Stack eval_stack;
extern Stack o_stack;
extern Stack op_stack;

#endif /* eval_h */
