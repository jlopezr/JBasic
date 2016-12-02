//
//  eval.c
//  JBasic
//
//  Created by Juan López Rubio on 15/5/16.
//  Copyright © 2016 Juan López Rubio. All rights reserved.
//

#include "eval.h"
#include "instructions.h"
#include "errors.h"

void stack_init(Stack* s) {
    s->stack_pos = 0;
}

int push(Stack* s, Atom a) {
    if(s->stack_pos==STACK_MAX) {
        printf("Push. Stack overflow\r\n");
        return -1;
    } else {
        s->stack[s->stack_pos++] = a;
        return 0;
    }
}

int peek(Stack* s, Atom* atom) {
    if(s->stack_pos>0) {
        *atom = s->stack[s->stack_pos-1];
        return 0;
    } else {
        printf("Peek. Stack empty\r\n");
        return -1;
    }
}

int pop(Stack* s, Atom* atom) {
    if(peek(s, atom)==0) {
        s->stack_pos--;
        return 0;
    } else {
        printf("Pop. Stack underflow\r\n");
        return -1;
    }
}

void print_stack(Stack* s) {
    for(int i=0;i<s->stack_pos;i++) {
        printf("%d. ", i);
        unsigned char type = s->stack[i].type;
        if(type == ATOM_INT) {
            printf("%d", s->stack[i].integer);
        } else if(type == ATOM_OP) {
            instr_impl* op = s->stack[i].operation;
            if( op == _add) {
                printf("+");
            } else if(op == _sub) {
                printf("-");
            } else if(op == _mul) {
                printf("*");
            } else if(op == _div) {
                printf("/");
            }
        }
        printf("\r\n");
    }
}


Stack input_stack;
Stack eval_stack;
Stack o_stack;
Stack op_stack;

// 2+3*2 => 2 3 2 * +
// 2*3+2 => 2 3 * 2 +

char rpn_to_infix(char* code) {
    stack_init(&input_stack);
    
    return ERR_OK;
}
