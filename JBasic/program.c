//
//  program.c
//  JBasic
//
//  Created by Juan López Rubio on 21/11/16.
//  Copyright © 2016 Juan López Rubio. All rights reserved.
//

#include <strings.h>
#include "program.h"
#include "keywords.h"
#include "errors.h"

char program[MAX_PROGRAM_SIZE];
char* pc = &program[0];
char* sop = &program[0];
char* eop = &program[0];
int lastLineNumber = 0;

Line* findLine(unsigned int lineNumber) {
    char* tmp;
    tmp = sop;
    while(tmp<eop) {
        Line* line = (Line*)tmp;
        if(line->lineNumber==lineNumber) {
            return line;
        }
        tmp = tmp + sizeof(unsigned int)+ sizeof(char)+ line->length;
    }
    return NULL;
}

void list_opcodes_line(Line* line) {
    int i=0;
    unsigned int* p;
    char* p2;
    int len;
    
    while(i<line->length) {
        char opcode = line->code[i];
        printf("%s ",keywords[opcode].name);
        switch(keywords[opcode].parameters) {
            case PARAM_VOID:
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
                len = strlen(p2);
                printf("%s ", p2);
                i=i+len;
                break;
            default:
                printf("<OPCODES PARAMETERS NOT IMPLEMENTED>");
        }
        i=i+1;
    }
}

void list_opcodes() {
    char* tmp;
    tmp = sop;
    while(tmp<eop) {
        Line* line = (Line*)tmp;
        printf("%u (size: %u) ", line->lineNumber, line->length);
        list_opcodes_line(line);
        printf("\r\n");
        tmp = tmp + sizeof(unsigned int)+ sizeof(char)+ line->length;
    }  
}

void addLine(char instruction) {
    Line* line = (Line*)eop;
    lastLineNumber = lastLineNumber + 10;
    line->lineNumber = lastLineNumber;
    line->length = 1;
    line->code[0] = instruction;
    eop = eop + sizeof(unsigned int)+ sizeof(char)+ line->length;
}

void addLine_int(char instruction, unsigned int n) {
    Line* line = (Line*)eop;
    lastLineNumber = lastLineNumber + 10;
    line->lineNumber = lastLineNumber;
    line->length = 1 + sizeof(unsigned int);
    line->code[0] = instruction;
    unsigned int* p = (unsigned int *)&(line->code[1]);
    *p = n;
    eop = eop + sizeof(unsigned int)+ sizeof(char)+ line->length;
}

void addLine_string(char instruction, char* str) {
    Line* line = (Line*)eop;
    lastLineNumber = lastLineNumber + 10;
    line->lineNumber = lastLineNumber;
    line->code[0] = instruction;
    char* p = (char *)&(line->code[1]);
    char* q = stpcpy(p, str);
    line->length = 2 + (q-p);
    eop = eop + sizeof(unsigned int)+ sizeof(char)+ line->length;
}

unsigned char doTrace = 0;

void run() {
    unsigned char end = 0;
    
    pc = sop;
    while(pc<eop && !end) {
        Line* line = (Line*)pc;
        Keyword k = keywords[line->code[0]];
        instr_impl* impl = k.impl;
        
        if(doTrace) {
            //printf("[%d %ld ", line->lineNumber, pc-sop);
            printf("[%d ", line->lineNumber);
        }
        
        if(impl==0) {
            printf("OPCODE Not known in line %d\r\n", line->lineNumber);
            break;
        } else {
            char result = impl(line);

            if(doTrace) {
                printf(" %d]", result);
            }
            
            switch(result) {
                case ERR_OK:
                    pc = pc + sizeof(unsigned int)+ sizeof(char)+ line->length;
                    break;
                case ERR_OK_JUMP:
                    break;
                case ERR_OK_END:
                    end = 1;
                    break;
                default:
                    printf("ERROR %s at line %d\r\n", errors[result], line->lineNumber);
                    end = 1;
            }
        }
    }
}
