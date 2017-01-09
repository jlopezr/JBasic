//
//  program.c
//  JBasic
//
//  Created by Juan López Rubio on 21/11/16.
//  Copyright © 2016 Juan López Rubio. All rights reserved.
//

#include <strings.h>
#include <assert.h>
#include "program.h"
#include "keywords.h"
#include "errors.h"
#include "heap.h"
#include "sym.h"

char program[MAX_PROGRAM_SIZE];
char* pc = &program[0];
char* sop = &program[0];
char* eop = &program[0];
Line* lc = 0;

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
            case PARAM_TEXT:
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
                printf("'%s' = ", p2);
                i=i+len+1;
                break;
            default:
                printf("<WHILE LISTING PARAM UNKNOWN>");
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

// Used for editing
Line* lastLine = 0;
// TODO lastLineNumber or equivalent variable can be updated when executing LINE token
int lastLineNumber = 0;

void addLine() {
    if(lastLine != 0) {
        endLine();
    }
    Line* line = (Line*)eop;
    lastLine = line;
    lastLineNumber = lastLineNumber + 10;
    line->lineNumber = lastLineNumber;
    eop = eop + sizeof(unsigned int) + sizeof(char);
}

void endLine() {
    lastLine->length = (eop - (char*)lastLine) - sizeof(unsigned int) - sizeof(char);
    lastLine = 0;
}

void addOp(char opcode) {
    *eop = opcode;
    eop++;
}

void addIntOp(char opcode, int n) {
    *eop = opcode;
    int* p = (int *)(eop+1);
    *p = n;
    eop = eop + sizeof(char) + sizeof(int);
}

void addStringOp(char opcode, char* str) {
    *eop = opcode;
    eop++;
    char* q = stpcpy(eop, str);
    eop = q+1;
}

unsigned char doTrace = 0;
unsigned char doStep = 0;
unsigned char doInstruction = 0;

void reset() {
    init_heap();
    
    pc = &program[0];
    sop = &program[0];
    eop = &program[0];
    lc = 0;
    
    lastLine = 0;
    lastLineNumber = 0;
    
    clear_int_vars();
    clear_string_vars();
}

char tron() {
    doTrace = 1;
    return ERR_OK;
}

char troff() {
    doTrace = 0;
    return ERR_OK;
}

void run() {
    step(0);
}

char each_line() {
    static char step = 0;
    if(step) {
        step = 0;
        return 1;
    } else {
        step = 1;
        return 0;
    }
}

int step_lineNumber;

void set_step_until_line(int line) {
    step_lineNumber = line;
}

char on_line() {
    if(lc->lineNumber == step_lineNumber) {
        return 1;
    } else {
        return 0;
    }
}

void step(stepping_func* step) {
    unsigned char end = 0;
    unsigned char out = 0;
    
    while(pc<eop && !end) {
        
        if(out==1) {
            out = 0;
        } else {
            lc = (Line*)pc;
            pc = (char*)&(lc->code[0]);
        }
        char* eol = pc + (lc->length);
        out = 0;
        
        if(step!=0) {
            if(step()) {
                //TODO remove this hack, goes back to the beginning of line
                pc = (char*)lc;
                return;
            }
        }
        
        while(pc<eol && !end && !out) {
            Keyword k = keywords[*pc];
            instr_impl* impl = k.impl;
            
            if(doTrace) {
                printf("[%d %ld %s", lc->lineNumber, pc-sop, k.name);
                //printf("[%d ", lc->lineNumber);
            }
            
            if(impl==0) {
                printf("OPCODE %s Not known in line %d\r\n", k.name, lc->lineNumber);
                break;
            } else {
                pc++;
                char result = impl();
                
                if(doTrace) {
                    printf(" %d]", result);
                }
                
                switch(result) {
                    case ERR_OK:
                        switch(k.parameters) {
                            case PARAM_VOID:
                            case PARAM_EXPR:
                            case PARAM_1_INT:
                            case PARAM_2_INT:
                                break;
                            case PARAM_NUM:
                                pc=pc+sizeof(unsigned int);
                                break;
                            case PARAM_VAR:
                            case PARAM_TEXT:
                            case PARAM_ASSIGN:
                                //TODO try recover this info from instruction execution... some use the pointer without copy... :(
                                pc=pc+(int)strlen(pc)+1;
                                break;
                            default:
                                printf("<WHILE RUNNING PARAM UNKNOWN>");
                        }
                        break;
                    case ERR_OK_JUMP:
                        out = 1;
                        break;
                    case ERR_OK_END:
                        end = 1;
                        break;
                    default:
                        printf("ERROR %s at line %d\r\n", errors[result], lc->lineNumber);
                        end = 1;
                }
            }
        }
        
        //Instead of jumping to the next line, the code pointer is already on the new line
        //pc = pc + sizeof(unsigned int)+ sizeof(char)+ line->length;
    }
    
    // This puts PC at the beginning when the program ends
    pc = eop;
}
