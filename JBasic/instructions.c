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

//TODO remove line number here at put it at error

char _goto(Line* line) {
    unsigned int* p = (unsigned int *)&(line->code[1]);
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
    char* name = &(line->code[1]);
    return set_int_var(name, 0);
}

char _inc(Line* line) {
    printf("INC\r\n");
    char* name = &(line->code[1]);
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
    char* name = &(line->code[1]);
    int value;
    
    //TODO be able to get the pointer to the symbol table
    char ret = get_int_var(name, &value);
    if(ret != ERR_OK) {
        return ret;
    }
    value--;
    return set_int_var(name, value);
}

char _print(Line* line) {
    char* name = &(line->code[1]);
    int value;
    
    //TODO be able to get the pointer to the symbol table
    char ret = get_int_var(name, &value);
    if(ret != ERR_OK) {
        return ret;
    }
    printf("%d\r\n", value);
    return ERR_OK;
}

char* calls[NUM_GOSUB_CALLS];
int number_of_gosub_calls = 0;

char _gosub(Line* line) {
    unsigned int* p = (unsigned int *)&(line->code[1]);
    Line* target = findLine(*p);
    if(target==0) {
        return ERR_LINE_NOT_EXIST;
    } else {
        if(number_of_gosub_calls < NUM_GOSUB_CALLS) {
            char* ret = pc + sizeof(unsigned int)+ sizeof(char)+ line->length;
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

