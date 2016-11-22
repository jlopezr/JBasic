//
//  program.h
//  JBasic
//
//  Created by Juan López Rubio on 21/11/16.
//  Copyright © 2016 Juan López Rubio. All rights reserved.
//

#ifndef program_h
#define program_h

#include <stdio.h>

#define MAX_PROGRAM_SIZE        1024
#define MAX_LINE_SIZE           255
#define MAX_LINE_SIZE_IN_TEXT   512

typedef struct {
    unsigned int lineNumber;            // line number
    unsigned char length;               // length of the line in bytes
    unsigned char code[MAX_LINE_SIZE];  // opcodes of the line
} Line;

Line* findLine(unsigned int lineNumber);
void addLine(char instruction);
void addLine_int(char instruction, unsigned int n);
void addLine_string(char instruction, char* str);
void run();
void list_opcodes();

extern char program[MAX_PROGRAM_SIZE];
extern char* pc;    // program counter
extern char* sop;   // start of program
extern char* eop;   // end of program

#endif /* program_h */
