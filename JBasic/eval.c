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
#include "heap.h"
#include <stdlib.h>
#include <string.h>

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
            printf("%s", findKeywordByImpl(s->stack[i].operation));
        }
        printf("\r\n");
    }
}


Stack input_stack;
Stack eval_stack;
Stack o_stack;
Stack op_stack;

char numberOfDigits(int n) {
    char ret = 0;
    char negative = 0;
    
    if(n<0) {
        n = -n;
        negative = 1;
    }
    
    if(n<10) {
        ret=1;
    } else if(n<100) {
        ret=2;
    } else if(n<1000) {
        ret=3;
    } else if(n<10000) {
        ret=4;
    } else if(n<100000) {
        ret=5;
    } else if(n<1000000) {
        ret=6;
    } else if(n<10000000) {
        ret=7;
    } else if(n<100000000) {
        ret=8;
    } else if(n<1000000000) {
        ret=9;
    }
    
    if(negative) {
        ret = ret + 1;
    }
    
    return ret;
}

void atom_free(Atom* atom) {
    if(atom->type == ATOM_TMP_STRING) {
        heap_free(atom->string);
    }
}

char atom_to_string(Atom* atom) {
    int n, digits;
    char* buf;
    switch(atom->type) {
        case ATOM_INT:
            n = atom->integer;
            digits = numberOfDigits(n);
            atom->type = ATOM_TMP_STRING;
            atom->string = heap_alloc(digits+1);
            //TODO Check error
            if(atom->string == 0) {
                return ERR_MEMORY_FULL;
            }
            snprintf(atom->string, digits+1, "%d", n);
            return ERR_OK;
        case ATOM_OP:
            //TODO Atom stores the pointer to impl... but here we need the name
            buf = findKeywordByImpl(atom->operation);
            if(buf!=0) {
                atom->type = ATOM_STRING;
                atom->string = buf;
                return ERR_OK;
            } else {
                return ERR_BAD_SYNTAX;
            }
            break;
        case ATOM_STRING:
            return ERR_OK;
        case ATOM_BOOL:
        default:
            return ERR_BAD_SYNTAX;
    }
}

/*
 
 123 (N)
 al sacar el elemento lo imprime
 
 1 (N) 2 (N) + (OP)
 al sacar el elemento
 imprimo el top-2
 imprimo el elemento
 imprimo el top-1
 
 1+2*3 = 7
 1 2 3 * +
 1
 1 2
 1 2 3
 1 2 3 *
 1 2 3 * (* es mas prioritario que + asi que) 1 “2 * 3” +
 “1 + 2 * 3”
 
 (1+2)*3 = 9
 1 2 + 3 *
 1
 1 2
 1 2 +
 1 2 + 3 (3 no es una op asi que) “(1 + 2)” 3
 “(1 + 2) * 3”

 */

/*

        *       <- top
0 1 2 3 4       <- pos
1 2 3 * +
      1 0       <- left
      2 3       <- right

    .   *       <- top
0 1 2 3 4       <- pos
1 2 + 3 *
    0   2       <- left
    1   3       <- right

 */
 
int rpn_to_infix(Line* line, int i) {
    Atom a1, a2;
    char end = 0;
    int* p;
    char* buf;
    int ret, len1, len2;
    stack_init(&input_stack);
    
    
    while(i<line->length && !end) {
        char opcode = line->code[i];
        switch(opcode) {
            case ADD:
            case SUB:
            case MUL:
            case DIV:
                
                /* CASE PUSH */
                /*
                a1.type = ATOM_OP;
                a1.operation = keywords[opcode].impl;
                ret = push(&input_stack, a1);
                 
                //TODO Check ERROR
                */
                /* CASE JOIN */
                ret = pop(&input_stack,&a2);
                len2 = atom_to_string(&a2);
                //TODO Check ERRORS
                ret = pop(&input_stack,&a1);
                len1 = atom_to_string(&a1);
                //TODO Check ERRORS
                
                //JUAN HERE
                //TODO Store priorities for expr
                //TODO Store printed name for expr, ADD is +
                //TODO Store if op must be stored (i.e is an expression)
                
                printf("%s %s %s", a1.string, findKeyword(opcode), a2.string);
                
                atom_free(&a1);
                atom_free(&a2);
                i=i+1;
                break;
            case INT_K:
                p = (int*)(&line->code[i+1]);
                a1.type = ATOM_INT;
                a1.integer = *p;
                ret = push(&input_stack, a1);
                //TODO Check ERROR
                i=i+sizeof(int)+1;
                break;
            case VAR:
                //TODO JUAN MUST PUT IN THE INPUT STACK !!!!
                buf = (char*)(&line->code[i+1]);
                printf("%s ",buf);
                i = i+(int)strlen(buf)+2;
                break;
            case END_EXPR:
                i++;
                end = 1;
                break;
            default:
                printf("<OPCODE NOT IMPLEMENTED IN RPN_TO_INFIX>\r\n");
                i=i+1;
        }
        opcode = line->code[i];
    }
    print_stack(&input_stack);
    return i;
}
