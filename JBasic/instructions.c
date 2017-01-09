//
//  instructions.c
//  JBasic
//
//  Created by Juan López Rubio on 21/11/16.
//  Copyright © 2016 Juan López Rubio. All rights reserved.
//

#include "instructions.h"
#include "keywords.h"
#include "errors.h"
#include "sym.h"
#include "eval.h"
#include <strings.h>
#include <math.h>

char _end() {
    return ERR_OK_END;
}

char _not_implemented() {
    printf("NOT IMPLEMENTED!");
    return ERR_OK_END;
}

char _test() {
    printf("TEST\r\n");
    return ERR_OK;
}

char _goto() {
    unsigned int* p = (unsigned int *)pc;
    Line* target = findLine(*p);
    if(target==0) {
        return ERR_LINE_NOT_EXIST;
    } else {
        lc = (Line*)target;
        pc = (char*)&(target->code[0]);
        return ERR_OK_JUMP;
    }
}

void _list_line(Line* line) {
    int i=0;
    unsigned int* p;
    char* p2;
    int len;
    
    while(i<line->length) {
        if(i!=0) {
            printf(": ");
        }
        char opcode = line->code[i];
        printf("%s ",keywords[opcode].name);
        switch(keywords[opcode].parameters) {
            case PARAM_VOID:
            case PARAM_1_INT:
            case PARAM_2_INT:
                i=i+1;
                break;
            case PARAM_NUM:
                i=i+1;
                p = (unsigned int *)&(line->code[i]);
                printf("%u ",*p);
                i=i+sizeof(unsigned int);
                break;
            case PARAM_VAR:
                i=i+1;
                p2 = (char *)&(line->code[i]);
                len = (int)strlen(p2);
                printf("%s ", p2);
                i=i+len+1;
                break;
            case PARAM_EXPR:
                i=i+1;
                i = rpn_to_infix(line, i);
                break;
            case PARAM_ASSIGN:
                i=i+1;
                p2 = (char *)&(line->code[i]);
                len = (int)strlen(p2);
                printf("%s = ", p2);
                i=i+len+1;
                i = rpn_to_infix(line, i);
                break;
            default:
                printf("<WHILE OP LIST PARAM UNKNOWN>");
                i=i+1;
        }
    }
}

char _list() {
    char* pc = sop;
    while(pc<eop) {
        Line* line = (Line*)pc;
        printf("%d ", line->lineNumber);
        _list_line(line);
        printf("\r\n");
        pc = pc + sizeof(unsigned int) + sizeof(char) + line->length;
    }
    
    return ERR_OK;
}

char _zero() {
    char* name = (char*)pc;
    return set_int_var(name, 0);
}

char _inc() {
    printf("INC\r\n");
    char* name = (char*)pc;
    int* p = get_int_var(name);
    
    if(p == 0) {
        return ERR_VAR_NOT_FOUND;
    }
    
    (*p)++;
    return ERR_OK;
}

char _dec() {
    printf("DEC\r\n");
    char* name = (char*)pc;
    int* p = get_int_var(name);
    
    if(p == 0) {
        return ERR_VAR_NOT_FOUND;
    }
    
    (*p)--;
    return ERR_OK;
}

typedef struct {
    char* pc;
    Line* lc;
} GosubState;

GosubState calls[NUM_GOSUB_CALLS];
int number_of_gosub_calls = 0;

char _gosub() {
    unsigned int* p = (unsigned int *)pc;
    Line* target = findLine(*p);
    if(target==0) {
        return ERR_LINE_NOT_EXIST;
    } else {
        if(number_of_gosub_calls < NUM_GOSUB_CALLS) {
            char* ret = (char*)p + sizeof(unsigned int);
            calls[number_of_gosub_calls].pc = ret;
            calls[number_of_gosub_calls].lc = lc;
            number_of_gosub_calls++;            
            lc = (Line*)target;
            pc = (char*)&(target->code[0]);
            return ERR_OK_JUMP;
        } else {
            return ERR_GOSUB_DEPTH;
        }
    }
}

char _return() {
    if(number_of_gosub_calls>0) {
        number_of_gosub_calls--;
        pc = calls[number_of_gosub_calls].pc;
        lc = calls[number_of_gosub_calls].lc;
        return ERR_OK_JUMP;
    } else {
        return ERR_GOSUB_MISSING;
    }
    
}

char _int_constant() {
    int* p = (int *)pc;
    Atom atom1;
    atom1.type = ATOM_INT;
    atom1.integer = *p;
    
    char err = push(&eval_stack, atom1);
    return err == 0 ? ERR_OK : ERR_STACK_FULL;
}

char _str_constant() {
    char* p = (char *)pc;
    Atom atom1;
    atom1.type = ATOM_STRING;
    atom1.string = p;
    
    char err = push(&eval_stack, atom1);
    return err == 0 ? ERR_OK : ERR_STACK_FULL;
}

char _var() {
    char* name = (char*)pc;
    Atom atom1;
    char ret;
    
    int* p = get_int_var(name);
    if(p == 0) {
        return ERR_VAR_NOT_FOUND;
    }
    atom1.type = ATOM_INT;
    atom1.integer = *p;
    
    ret = push(&eval_stack, atom1);
    return ret == 0 ? ERR_OK : ERR_STACK_FULL;
}

//TODO reuse code for multiple expression operations
char _add() {
    Atom atom1;
    Atom atom2;
    char err1 = pop(&eval_stack, &atom1);
    char err2 = pop(&eval_stack, &atom2);
    
    if(err1!=0 || err2!=0 || atom1.type != ATOM_INT || atom2.type != ATOM_INT) {
        return ERR_BAD_SYNTAX;
    }
    
    int result = atom1.integer + atom2.integer;
    atom1.integer = result;
    
    push(&eval_stack, atom1);
    
    return ERR_OK;
}

char _sub() {
    Atom atom1;
    Atom atom2;
    char err1 = pop(&eval_stack, &atom1);
    char err2 = pop(&eval_stack, &atom2);
    
    if(err1!=0 || err2!=0 || atom1.type != ATOM_INT || atom2.type != ATOM_INT) {
        return ERR_BAD_SYNTAX;
    }
    
    int result = atom1.integer - atom2.integer;
    atom1.integer = result;
    
    push(&eval_stack, atom1);
    
    return ERR_OK;
}

char _mul() {
    Atom atom1;
    Atom atom2;
    char err1 = pop(&eval_stack, &atom1);
    char err2 = pop(&eval_stack, &atom2);
    
    if(err1!=0 || err2!=0 || atom1.type != ATOM_INT || atom2.type != ATOM_INT) {
        return ERR_BAD_SYNTAX;
    }
    
    int result = atom1.integer * atom2.integer;
    atom1.integer = result;
    
    push(&eval_stack, atom1);
    
    return ERR_OK;
}

char _div() {
    Atom atom1;
    Atom atom2;
    char err1 = pop(&eval_stack, &atom1);
    char err2 = pop(&eval_stack, &atom2);
    
    if(err1!=0 || err2!=0 || atom1.type != ATOM_INT || atom2.type != ATOM_INT) {
        return ERR_BAD_SYNTAX;
    }
    
    if(atom2.integer == 0) {
        return ERR_DIV_ZERO;
    }
    
    int result = atom1.integer / atom2.integer;
    atom1.integer = result;
    
    push(&eval_stack, atom1);
    
    return ERR_OK;
}

char _less() {
    Atom atom1;
    Atom atom2;
    char err1 = pop(&eval_stack, &atom1);
    char err2 = pop(&eval_stack, &atom2);
    
    if(err1!=0 || err2!=0 || atom1.type != ATOM_INT || atom2.type != ATOM_INT) {
        return ERR_BAD_SYNTAX;
    }
    
    atom1.type = ATOM_BOOL;
    if(atom1.integer < atom2.integer) {
        atom1.boolean = 1;
    } else {
        atom1.boolean = 0;
    }
    
    push(&eval_stack, atom1);
    return ERR_OK;
}

//TODO Seems that CPC Basic uses -1 as true and 0 as false ...
char _greater() {
    Atom atom1;
    Atom atom2;
    char err1 = pop(&eval_stack, &atom1);
    char err2 = pop(&eval_stack, &atom2);
        
    if(err1!=0 || err2!=0 || atom1.type != ATOM_INT || atom2.type != ATOM_INT) {
        return ERR_BAD_SYNTAX;
    }
        
    atom1.type = ATOM_BOOL;
    if(atom1.integer > atom2.integer) {
        atom1.boolean = 1;
    } else {
        atom1.boolean = 0;
    }
        
    push(&eval_stack, atom1);
    return ERR_OK;
}

char _min() {
    Atom atom1;
    Atom atom2;
    char err1 = pop(&eval_stack, &atom1);
    char err2 = pop(&eval_stack, &atom2);
    
    if(err1!=0 || err2!=0 || atom1.type != ATOM_INT || atom2.type != ATOM_INT) {
        return ERR_BAD_SYNTAX;
    }
    
    if(atom1.integer > atom2.integer) {
            atom1.integer = atom2.integer;
    }
    
    push(&eval_stack, atom1);
    return ERR_OK;
}

char _max() {
    Atom atom1;
    Atom atom2;
    char err1 = pop(&eval_stack, &atom1);
    char err2 = pop(&eval_stack, &atom2);
    
    if(err1!=0 || err2!=0 || atom1.type != ATOM_INT || atom2.type != ATOM_INT) {
        return ERR_BAD_SYNTAX;
    }
    
    if(atom1.integer < atom2.integer) {
        atom1.integer = atom2.integer;
    }
    
    push(&eval_stack, atom1);
    return ERR_OK;
}

instr_impl* after_expr;
char* after_name;

char _let_() {
    Atom atom1;
    char err = pop(&eval_stack, &atom1);
    if(err!=0 || eval_stack.stack_pos != 0) {
        return ERR_BAD_SYNTAX;
    }
    if(atom1.type!=ATOM_INT) {
        return ERR_BAD_TYPE;
    }
    return set_int_var(after_name, atom1.integer);
}

char _let() {
    //TODO Must be initialized all the times?
    stack_init(&eval_stack);
    after_expr = _let_;
    after_name = (char*)pc;
    return ERR_OK;
}

char _print_() {
    Atom atom1;
    char err = pop(&eval_stack, &atom1);
    if(err!=0 || eval_stack.stack_pos != 0) {
        return ERR_BAD_SYNTAX;
    }
    switch(atom1.type) {
        case ATOM_INT:
            printf("%d", atom1.integer);
            break;
        case ATOM_STRING:
        case ATOM_TMP_STRING:
            printf("%s", atom1.string);
            break;
        default:
            return ERR_BAD_TYPE;
    }
    printf("\r\n");
    return ERR_OK;
}

char _print() {
    //TODO Must be initialized all the times?
    stack_init(&eval_stack);
    after_expr = _print_;
    return ERR_OK;
}

char _end_of_expr(Line* line) {
    return after_expr(line);
}

/*** MATH ***/

//TODO Do not pop but peek the eval stack for minimizing operations on stack

char _pow() {
    Atom atom1;
    Atom atom2;
    char err1 = pop(&eval_stack, &atom1);
    char err2 = pop(&eval_stack, &atom2);
    
    if(err1!=0 || err2!=0 || atom1.type != ATOM_INT || atom2.type != ATOM_INT) {
        return ERR_BAD_SYNTAX;
    }
    
    atom1.integer = pow(atom2.integer, atom1.integer);
    
    push(&eval_stack, atom1);
    return ERR_OK;
}

char _sqrt() {
    Atom atom1;
    char err1 = pop(&eval_stack, &atom1);
    
    if(err1!=0 || atom1.type != ATOM_INT) {
        return ERR_BAD_SYNTAX;
    }
    
    atom1.integer = sqrt(atom1.integer);
    
    push(&eval_stack, atom1);
    return ERR_OK;
}

char _sgn() {
    Atom atom1;
    int n, ret;
    char err1 = pop(&eval_stack, &atom1);
    
    if(err1!=0 || atom1.type != ATOM_INT) {
        return ERR_BAD_SYNTAX;
    }
    
    n = atom1.integer;
    if(n == 0) {
        ret = 0;
    } else if(n > 0) {
        ret = 1;
    } else {
        ret = -1;
    }
    
    atom1.integer = ret;
    
    push(&eval_stack, atom1);
    return ERR_OK;
}

char _abs() {
    Atom atom1;
    int n;
    char err1 = pop(&eval_stack, &atom1);
    
    if(err1!=0 || atom1.type != ATOM_INT) {
        return ERR_BAD_SYNTAX;
    }
    
    n = atom1.integer;
    if(n<0) {
        n = -n;
    }
    
    atom1.integer = n;
    
    push(&eval_stack, atom1);
    return ERR_OK;
}


