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

// 2+3*2 => 2 3 2 * +
// 2*3+2 => 2 3 * 2 +

Stack input_stack;
Stack eval_stack;
Stack o_stack;
Stack op_stack;

/*
int main2() {
    Atom op1, op2, op3;
    
    stack_init(&input_stack);
    stack_init(&eval_stack);
    stack_init(&op_stack);
    
    op1.type = ATOM_INT;
    op1.integer = 2;
    push(&input_stack, op1);

    op1.type = ATOM_OP;
    op1.integer = OP_MUL;
    push(&input_stack, op1);
    
    op1.type = ATOM_INT;
    op1.integer = 3;
    push(&input_stack, op1);
    
    op1.type = ATOM_OP;
    op1.integer = OP_ADD;
    push(&input_stack, op1);
    
    op1.type = ATOM_INT;
    op1.integer = 2;
    push(&input_stack, op1);
    
    for(int i=0;i<input_stack.stack_pos;i++) {
        unsigned char type = input_stack.stack[i].type;
        if(type == ATOM_INT) {
            push(&o_stack, input_stack.stack[i]);
        } else if(type == ATOM_OP) {
            push(&op_stack, input_stack.stack[i]);
        }
    }
    
    //TODO Esto da invalid stack pq intenta sacar...
    while(pop(&op_stack,&op1)==0) {
        push(&o_stack, op1);
    }
    
    print_stack(&o_stack);
    
    //main4();
    
    return 0;
}

int main4() {
    Atom op1, op2, op3;
    
    stack_init(&eval_stack);

    for(int i=0;i<o_stack.stack_pos;i++) {
        unsigned char type = o_stack.stack[i].type;
        if(type == ATOM_INT) {
            push(&eval_stack, o_stack.stack[i]);
        } else if(type == ATOM_OP) {
            switch(o_stack.stack[i].operation) {
                case OP_ADD:
                    pop(&eval_stack, &op1);
                    pop(&eval_stack, &op2);
                    
                    op3.type = ATOM_INT;
                    op3.integer = op1.integer + op2.integer;
                    
                    push(&eval_stack, op3);
                    break;
                case OP_SUB:
                    pop(&eval_stack, &op1);
                    pop(&eval_stack, &op2);
                    
                    op3.type = ATOM_INT;
                    op3.integer = op1.integer - op2.integer;
                    
                    push(&eval_stack, op3);
                    break;
                case OP_MUL:
                    pop(&eval_stack, &op1);
                    pop(&eval_stack, &op2);
                    
                    op3.type = ATOM_INT;
                    op3.integer = op1.integer * op2.integer;
                    
                    push(&eval_stack, op3);
                    break;
                case OP_DIV:
                    pop(&eval_stack, &op1);
                    pop(&eval_stack, &op2);
                    
                    op3.type = ATOM_INT;
                    op3.integer = op1.integer / op2.integer;
                    
                    push(&eval_stack, op3);
                    break;
                default:
                    printf("OPERATION UNKNOWN\r\n");
            }
        }
    }
    
    peek(&eval_stack, &op1);
    printf("RESULT %d\r\n", op1.integer);
    return 0;
}
*/
