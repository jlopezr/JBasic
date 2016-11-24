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

char _list(Line* line) {
    char buffer[MAX_LINE_SIZE_IN_TEXT];
    unsigned int* p;
    char* p2;
    
    char* pc = sop;
    while(pc<eop) {
        Line* line = (Line*)pc;
        
        switch(keywords[line->code[0]].parameters) {
            case PARAM_VOID:
                strcpy(buffer,"");
                break;
            case PARAM_NUM:
                p = (unsigned int *)&(line->code[1]);
                sprintf(buffer,"%u",*p);
                break;
            case PARAM_NUM_OPT:
                if(line->length==1+sizeof(unsigned int)) {
                    p = (unsigned int *)&(line->code[1]);
                    sprintf(buffer,"%u",*p);
                } else {
                    strcpy(buffer,"");
                }
                break;
            case PARAM_VAR:
                p2 = (char*)&(line->code[1]);
                sprintf(buffer,"%s", p2);
                break;
            default:
                printf("LIST PARAMETERS NOT IMPLEMENTED\r\n");
        }
        
        printf("%d %s %s\n", line->lineNumber, findKeyword(line->code[0]), buffer);
        pc = pc + sizeof(unsigned int)+ sizeof(char)+ line->length;
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
    int value;
    
    //TODO be able to get the pointer to the symbol table
    char ret = get_int_var(name, &value);
    if(ret != ERR_OK) {
        return ret;
    }
    value++;
    return set_int_var(name, value);
}

char _dec(Line* line) {
    printf("DEC\r\n");
    char* name = (char*)pc;
    int value;
    
    //TODO be able to get the pointer to the symbol table
    char ret = get_int_var(name, &value);
    if(ret != ERR_OK) {
        return ret;
    }
    value--;
    return set_int_var(name, value);
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
    atom1.type = ATOM_INT;
    
    //TODO be able to get the pointer to the symbol table
    char ret = get_int_var(name, &(atom1.integer));
    if(ret != ERR_OK) {
        return ret;
    }
    
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

instr_impl* after_expr;
char* after_name;

char _eval_(Line* line) {
    printf("EVAL:\r\n");
    print_stack(&eval_stack);
    return ERR_OK;
}

char _eval(Line* line) {
    //TODO Must be initialized all the times?
    stack_init(&eval_stack);
    after_expr = _eval_;
    return ERR_OK;
}

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
