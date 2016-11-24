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
        tmp = tmp + sizeof(unsigned int) + sizeof(char) + line->length;
    }
    return NULL;
}

void list_opcodes_hex(Line* line) {
    int i=0;
    
    while(i<line->length) {
        printf("%x ", line->code[i]);
        i++;
    }
    printf("\r\n");
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
            case PARAM_EXPR:
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
            default:
                printf("<WHILE LISTING PARAM UNKWNOWN>");
                i=i+1;
        }
        
    }
}

void list_opcodes() {
    char* tmp;
    tmp = sop;
    while(tmp<eop) {
        Line* line = (Line*)tmp;
        printf("%u (size: %u) ", line->lineNumber, line->length);
        list_opcodes_hex(line);
        list_opcodes_line(line);
        printf("\r\n");
        tmp = tmp + sizeof(unsigned int) + sizeof(char) + line->length;
    }
}

Line* lastLine;

void addLine(char instruction) {
    Line* line = (Line*)eop;
    lastLine = line;
    lastLineNumber = lastLineNumber + 10;
    line->lineNumber = lastLineNumber;
    line->length = 1;
    line->code[0] = instruction;
    eop = eop + sizeof(unsigned int) + sizeof(char) + line->length;
}

void addLine_int(char instruction, unsigned int n) {
    Line* line = (Line*)eop;
    lastLineNumber = lastLineNumber + 10;
    line->lineNumber = lastLineNumber;
    line->length = 1 + sizeof(unsigned int);
    line->code[0] = instruction;
    unsigned int* p = (unsigned int *)&(line->code[1]);
    *p = n;
    eop = eop + sizeof(unsigned int) + sizeof(char) + line->length;
}

void addLine_string(char instruction, char* str) {
    Line* line = (Line*)eop;
    lastLineNumber = lastLineNumber + 10;
    line->lineNumber = lastLineNumber;
    line->code[0] = instruction;
    char* p = (char *)&(line->code[1]);
    char* q = stpcpy(p, str);
    line->length = 2 + (q-p);
    eop = eop + sizeof(unsigned int)+ sizeof(char) + line->length;
}

void endLine() {
    lastLine->length = (eop - (char*)lastLine) - sizeof(unsigned int) - sizeof(char);
}

void addExpr_int(char opcode, int n) {
    *eop = opcode;
    int* p = (int *)(eop+1);
    *p = n;
    eop = eop + sizeof(char) + sizeof(int);
}

void addExpr_op(char opcode) {
    *eop = opcode;
    eop++;
}


void addExpr_string(char opcode, char* str) {
    *eop = opcode;
    eop++;
    char* q = stpcpy(eop, str);
    eop = q+1;
}

unsigned char doTrace = 0;

void run() {
    unsigned char end = 0;
    
    pc = sop;
    while(pc<eop && !end) {
        Line* line = (Line*)pc;
        pc = (char*)&(line->code[0]);
        char* eol = pc + (line->length);
        
        while(pc<eol && !end) {
            Keyword k = keywords[*pc];
            instr_impl* impl = k.impl;
            
            if(doTrace) {
                //printf("[%d %ld ", line->lineNumber, pc-sop);
                printf("[%d ", line->lineNumber);
            }
            
            if(impl==0) {
                printf("OPCODE Not known in line %d\r\n", line->lineNumber);
                break;
            } else {
                pc++;
                char result = impl(line);
                
                if(doTrace) {
                    printf(" %d]", result);
                }
                
                switch(result) {
                    case ERR_OK:
                        switch(k.parameters) {
                            case PARAM_VOID:
                            case PARAM_EXPR:
                                break;
                            case PARAM_NUM:
                                pc=pc+sizeof(unsigned int);
                                break;
                            case PARAM_VAR:
                                //TODO try recover this info from instruction execution... some use the pointer without copy... :(
                                pc=pc+(int)strlen(pc)+1;
                                break;
                            default:
                                printf("<WHILE RUNNING PARAM UNKWNOWN>");
                        }
                        break;
                    case ERR_OK_JUMP:
                        // This makes exit the inner loop
                        pc=eol;
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
        
        //Instead of jumping to the next line, the code pointer is already on the new line
        //pc = pc + sizeof(unsigned int)+ sizeof(char)+ line->length;
    }
}
