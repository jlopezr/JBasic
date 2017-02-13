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

/// Maximum program size in bytes
#define MAX_PROGRAM_SIZE        1024
/// Maximum size of a line in bytes
#define MAX_LINE_SIZE           255
/// Maximum size of a line in characters
#define MAX_LINE_SIZE_IN_TEXT   512


/// This structure holds a line of code
//TODO This struct will be the Instr_Line
typedef struct {
    unsigned int lineNumber;            ///< line number
    unsigned char length;               ///< length of the line in bytes
    unsigned char code[MAX_LINE_SIZE];  ///< opcodes of the line
} Line;

/// Looks up a line in the program memory
Line* findLine(unsigned int lineNumber);

/// Adds a new line in the program memory and makes it the current line
void addLine();

/// Closes the current line
void endLine();

/// Adds an opcode without parameters to the current line
void addOp(char opcode);

/// Adds an opcode with integer parameter to the current line
void addIntOp(char opcode, int n);

/// Adds an opcode with string parameter to the current line
void addStringOp(char opcode, char* str);

/// Switch on the tracing of intructions
char tron();

/// Switch off the tracing of intructions
char troff();

/// Resets both program and data memory
void reset();

/// Starts the progran execution
void run();

/// Lists the opcodes of the current rogram
void list_opcodes();

/// Definition of a function that can control the stepping of a program
/// @return true if the interpreter should stop in current line
typedef char stepping_func();

/// Runs until the stepping function returns false
void step(stepping_func* step);

/// This stepping functions stops in all the lines
char each_line();

/// This stepping functions runs until the program reaches the line set with set_step_until_line()
char on_line();
/// Sets the line where on_line() will stop
void set_step_until_line(int line);

extern char program[MAX_PROGRAM_SIZE];  ///< program memory
extern char* pc;    ///< program counter
extern Line* lc;    ///< line counter
extern char* sop;   ///< start of program
extern char* eop;   ///< end of program

#endif /* program_h */
