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

char _end(Line* line) {
    return ERR_OK_END;
}

char _not_implemented(Line* line) {
    printf("NOT IMPLEMENTED!");
    return ERR_OK_END;
}

char _test(Line* line) {
    printf("TEST\r\n");
    return ERR_OK;
}

char _goto(Line* line) {
    unsigned int* p = (unsigned int *)pc;
    Line* target = findLine(*p);
    if(target==0) {
        return ERR_LINE_NOT_EXIST;
    } else {
        pc = (char*)target;
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
            case PARAM_EXPR:
            case PARAM_1_INT:
            case PARAM_2_INT:
                i=i+1;
                break;
            case PARAM_NUM:
                i=i+1;
                p = (unsigned int *)&(line->code[i]);
                printf("'%u' ",*p);
                i=i+sizeof(unsigned int);
                break;
            case PARAM_VAR:
                i=i+1;
                p2 = (char *)&(line->code[i]);
                len = (int)strlen(p2);
                printf("'%s' ", p2);
                i=i+len+1;
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

char _list(Line* line) {
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

char _zero(Line* line) {
    char* name = (char*)pc;
    return set_int_var(name, 0);
}

char _inc(Line* line) {
    printf("INC\r\n");
    char* name = (char*)pc;
    int* p = get_int_var(name);
    
    if(p == 0) {
        return ERR_VAR_NOT_FOUND;
    }
    
    (*p)++;
    return ERR_OK;
}

char _dec(Line* line) {
    printf("DEC\r\n");
    char* name = (char*)pc;
    int* p = get_int_var(name);
    
    if(p == 0) {
        return ERR_VAR_NOT_FOUND;
    }
    
    (*p)--;
    return ERR_OK;
}

char* calls[NUM_GOSUB_CALLS];
int number_of_gosub_calls = 0;

//TODO GOSUB must be the last instruction on line ???
char _gosub(Line* line) {
    unsigned int* p = (unsigned int *)pc;
    Line* target = findLine(*p);
    if(target==0) {
        return ERR_LINE_NOT_EXIST;
    } else {
        if(number_of_gosub_calls < NUM_GOSUB_CALLS) {
            char* ret = (char*)line + sizeof(unsigned int)+ sizeof(char)+ line->length;
            calls[number_of_gosub_calls++] = ret;
            pc = (char*)target;
            return ERR_OK_JUMP;
        } else {
            return ERR_GOSUB_DEPTH;
        }
    }
}

char _return(Line* line) {
    if(number_of_gosub_calls>0) {
        pc = calls[--number_of_gosub_calls];
        return ERR_OK_JUMP;
    } else {
        return ERR_GOSUB_MISSING;
    }
    
}

char _int_constant(Line* line) {
    int* p = (int *)pc;
    Atom atom1;
    atom1.type = ATOM_INT;
    atom1.integer = *p;
    
    char err = push(&eval_stack, atom1);
    return err == 0 ? ERR_OK : ERR_STACK_FULL;
}

char _var(Line* line) {
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
char _add(Line* line) {
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

char _sub(Line* line) {
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

char _mul(Line* line) {
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

char _div(Line* line) {
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

char _min(Line* line) {
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

char _max(Line* line) {
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

char _let_(Line* line) {
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

char _let(Line* line) {
    //TODO Must be initialized all the times?
    stack_init(&eval_stack);
    after_expr = _let_;
    after_name = (char*)pc;
    return ERR_OK;
}

char _print_(Line* line) {
    Atom atom1;
    char err = pop(&eval_stack, &atom1);
    if(err!=0 || eval_stack.stack_pos != 0) {
        return ERR_BAD_SYNTAX;
    }
    if(atom1.type!=ATOM_INT) {
        return ERR_BAD_TYPE;
    }
    printf("%d\r\n", atom1.integer);
    return ERR_OK;
}

char _print(Line* line) {
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

char _pow(Line* line) {
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

char _sqrt(Line* line) {
    Atom atom1;
    char err1 = pop(&eval_stack, &atom1);
    
    if(err1!=0 || atom1.type != ATOM_INT) {
        return ERR_BAD_SYNTAX;
    }
    
    atom1.integer = sqrt(atom1.integer);
    
    push(&eval_stack, atom1);
    return ERR_OK;
}

char _sgn(Line* line) {
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

char _abs(Line* line) {
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


