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


//TODO This struct will be the Instr_Line
typedef struct {   
    unsigned int lineNumber;            // line number
    unsigned char length;               // length of the line in bytes
    unsigned char code[MAX_LINE_SIZE];  // opcodes of the line
} Line;

Line* findLine(unsigned int lineNumber);

void addLine();
void endLine();
void addOp(char opcode);
void addIntOp(char opcode, int n);
void addStringOp(char opcode, char* str);

char tron();
char troff();

void reset();
void run();
void list_opcodes();

typedef char stepping_func();
void step(stepping_func* step);
char each_line();
char on_line();
void set_step_until_line(int line);

extern char program[MAX_PROGRAM_SIZE];
extern char* pc;    // program counter
extern Line* lc;    // Line counter
extern char* sop;   // start of program
extern char* eop;   // end of program


#endif /* program_h */
