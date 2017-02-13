//
//  keywords.h
//  JBasic
//
//  Created by Juan López Rubio on 21/11/16.
//  Copyright © 2016 Juan López Rubio. All rights reserved.
//

#ifndef keywords_h
#define keywords_h

#include <stdio.h>
#include "program.h"

/// This enumeration encodes the parameters that the BASIC keyword has.
enum ParamType {
    PARAM_VOID,      ///< Nothing
    PARAM_NUM,       ///< A constant number (used in token INT_K)
    PARAM_TEXT,      ///< A constant string (used in token STR_K)
    PARAM_VAR,       ///< A variable identifier
    PARAM_VAR_LIST,  ///< A list of variables (used in token READ)
    PARAM_EXPR,      ///< An expression
    PARAM_EXPR_LIST, ///< An list of expressions (similiar to PRINT, but only separated by "," should used by MAX, MIN..)
    PARAM_ASSIGN,    ///< An assignation (identifier = expression)
    PARAM_NUM_OPT,   ///< An optional number
    PARAM_VAR_OPT,   ///< An optional variable identifier
    PARAM_LIST,      ///< None | num | num - num
    PARAM_PRINT,     ///< N expressions split by ; or ,
    PARAM_DATA,      ///< N constant expressions split by , (i.e numbers or texts")
    PARAM_INPUT,     ///< A variable or "text", variable
    PARAM_1_INT,     ///< Function with one integer
    PARAM_2_INT,     ///< Function with two integer
    PARAM_1_STR,     ///< Function with one string
    PARAM_2_STR,     ///< Function with one string
    PARAM_STR_INT,   ///< Function with one string + one integer
    PARAM_INT_STR,   ///< Function with one integer + one string
    PARAM_STR_2_INT  ///< Function with one string + two integer
};

typedef struct {
    char opcode;
} Instr_Void;

typedef struct {
    char opcode;
    int number;
} Instr_Num;

typedef struct {
    char opcode;
    //TODO Finish this
} Instr_Text;

typedef struct {
    char opcode;
    //TODO Finish this
} Instr_Var;

typedef struct {
    char opcode;
    //TODO Finish this. The pointer should move as we see different variables
} Instr_Var_List;

typedef struct {
    char opcode;
    int number;
} Instr_Num_Opt;

typedef struct {
    char opcode;
    //TODO Finish this
} Instr_Text_Opt;

typedef Instr_Text Instr_Assign;
typedef Instr_Void Instr_Expr;       // Expressions must be executed...
typedef Instr_Void Instr_Expr_List;  // TODO Specific END_OF_LIST must be added to implement this
typedef Instr_Void Instr_1_Int;      // All the function opcodes in reality do not read anything from the program, everything is on the stack.

//TODO Change the instruction format. Add a LINE token that keeps this information
// This allows to find the start the start of line just in case, then
// GOSUB/GOTO do not require to store the line + pc combo, only the pc
// the line info can be found by looking for the token when needed
// then the PARAM_ provide struct* to get easily the info from the code
// Remove the length from this token, loops can be simplified. Perhaps keep the
// * Typically line info is not used in instructions
// * Remove this parameter and make it global, we will have line and pc
// line can be 0 that means that it should be looked up scanning from the pc
// backwards

//TODO add the capability of executing until the end of program or only N lines or instructions
// This will help making unit tests :)
// When testing enabled test/list output should be also stored in a buffer for checks

/*
    Instruction format

    --------------------------------------------------
    | lineNumber: int | length: char | opcodes ....  |
    --------------------------------------------------
*/

/// Definition of a keyword implementation
typedef char instr_impl();

/// This structure defines a keyword and how has to be executed and listed
typedef struct {
    char* name;                  ///< Text representation of the keyword
    unsigned char onlyCLI : 1;   ///< Is this keyword only in command line mode, ie. EDIT or AUTO?
    unsigned char expression: 1; ///< Is this keyword used in expressions?
    unsigned char priority: 4;   ///< Priority of this keyword in a expression
    unsigned char parameters;    ///< Number and type of the parameters. See ::ParamType
    instr_impl* impl;            ///< Pointer to the keyword implementation function
} Keyword;

extern Keyword keywords[];

#define KT_EXPAND_AS_ENUMERATION(a,b,c,d,e,f) a ,
#define KT_EXPAND_AS_TABLE(a,b,c,d,e,f) { #a , b , c , d , e , f} ,

//TODO priority 0 can be used as not expression?

// Tokens from INT_K to STRING are marked as an expression
// NAME - CLI - EXPRESSION - PRIORITY - PARAMS - IMPLEMENTATION
// Priority https://en.wikipedia.org/wiki/Order_of_operations

#define KEYWORD_TABLE(ENTRY) \
ENTRY(END,      0, 0, 0, PARAM_VOID,      _end)               \
ENTRY(LET,      0, 0, 0, PARAM_ASSIGN,    _let)               \
ENTRY(TRON,     0, 0, 0, PARAM_VOID,      tron)               \
ENTRY(TROFF,    0, 0, 0, PARAM_VOID,      troff)              \
ENTRY(PRINT,    0, 0, 0, PARAM_EXPR,      _print)             \
ENTRY(INPUT,    0, 0, 0, PARAM_INPUT,     _not_implemented)   \
ENTRY(AUTO,     1, 0, 0, PARAM_NUM_OPT,   _not_implemented)   \
ENTRY(EDIT,     1, 0, 0, PARAM_NUM_OPT,   _not_implemented)   \
ENTRY(LIST,     1, 0, 0, PARAM_NUM_OPT,   _list)              \
ENTRY(RENUM,    1, 0, 0, PARAM_VOID,      _not_implemented)   \
ENTRY(GOTO,     0, 0, 0, PARAM_NUM,       _goto)              \
ENTRY(GOSUB,    0, 0, 0, PARAM_NUM,       _gosub)             \
ENTRY(RETURN,   0, 0, 0, PARAM_VOID,      _return)            \
ENTRY(WHILE,    0, 0, 0, PARAM_EXPR,      _not_implemented)   \
ENTRY(WEND,     0, 0, 0, PARAM_VOID,      _not_implemented)   \
ENTRY(FOR,      0, 0, 0, PARAM_ASSIGN,    _not_implemented)   \
ENTRY(TO,       0, 0, 0, PARAM_EXPR,      _not_implemented)   \
ENTRY(NEXT,     0, 0, 0, PARAM_VAR_OPT,   _not_implemented)   \
ENTRY(IF,       0, 0, 0, PARAM_EXPR,      _not_implemented)   \
ENTRY(THEN,     0, 0, 0, PARAM_VOID,      _not_implemented)   \
ENTRY(ELSE,     0, 0, 0, PARAM_VOID,      _not_implemented)   \
ENTRY(ENDIF,    0, 0, 0, PARAM_VOID,      _not_implemented)   \
ENTRY(INT_K,    0, 1, 0, PARAM_NUM,       _int_constant)      \
ENTRY(STR_K,    0, 1, 0, PARAM_TEXT,      _str_constant)      \
ENTRY(VAR,      0, 1, 0, PARAM_VAR,       _var)               \
ENTRY(ADD,      0, 1, 4, PARAM_VOID,      _add)               \
ENTRY(SUB,      0, 1, 4, PARAM_VOID,      _sub)               \
ENTRY(MUL,      0, 1, 3, PARAM_VOID,      _mul)               \
ENTRY(DIV,      0, 1, 3, PARAM_VOID,      _div)               \
ENTRY(LESS,     0, 1, 6, PARAM_2_INT,     _less)              \
ENTRY(GREATER,  0, 1, 6, PARAM_2_INT,     _greater)           \
ENTRY(END_EXPR, 0, 1, 0, PARAM_VOID,      _end_of_expr)       \
ENTRY(MIN,      0, 1, 1, PARAM_2_INT,     _min)               \
ENTRY(MAX,      0, 1, 1, PARAM_2_INT,     _max)               \
ENTRY(POW,      0, 1, 1, PARAM_2_INT,     _pow)               \
ENTRY(SQRT,     0, 1, 1, PARAM_1_INT,     _sqrt)              \
ENTRY(ABS,      0, 1, 1, PARAM_1_INT,     _abs)               \
ENTRY(SGN,      0, 1, 1, PARAM_1_INT,     _sgn)               \
ENTRY(LEN,      0, 1, 1, PARAM_1_STR,     _not_implemented)   \
ENTRY(INSTR,    0, 1, 1, PARAM_2_STR,     _not_implemented)   \
ENTRY(MID,      0, 1, 1, PARAM_STR_2_INT, _not_implemented)   \
ENTRY(LEFT,     0, 1, 1, PARAM_STR_INT,   _not_implemented)   \
ENTRY(RIGHT,    0, 1, 1, PARAM_STR_INT,   _not_implemented)   \
ENTRY(LOWER,    0, 1, 1, PARAM_1_STR,     _not_implemented)   \
ENTRY(UPPER,    0, 1, 1, PARAM_1_STR,     _not_implemented)   \
ENTRY(SPACE,    0, 1, 1, PARAM_1_INT,     _not_implemented)   \
ENTRY(STRING,   0, 1, 1, PARAM_INT_STR,   _not_implemented)   \
ENTRY(DEG,      0, 0, 0, PARAM_VOID,      _not_implemented)   \
ENTRY(RAD,      0, 0, 0, PARAM_VOID,      _not_implemented)   \
ENTRY(DATA,     0, 0, 0, PARAM_DATA,      _not_implemented)   \
ENTRY(READ,     0, 0, 0, PARAM_VAR_LIST,  _not_implemented)   \
ENTRY(RESTORE,  0, 0, 0, PARAM_NUM_OPT,   _not_implemented)   \
ENTRY(TEST,     0, 0, 0, PARAM_VOID,      _test)              \
ENTRY(ZERO,     0, 0, 0, PARAM_VAR,       _zero)              \
ENTRY(INC,      0, 0, 0, PARAM_VAR,       _inc)               \
ENTRY(DEC,      0, 0, 0, PARAM_VAR,       _dec)

//TODO MAX and MIN retrieves a list of numbers !!!!

//http://www.cpcwiki.eu/index.php/Locomotive_BASIC

//LEN nos dice la longitud de una cadena: PRINT LEN("Nacho") escribiría 5.
//LEFT$ extrae ciertos caracteres del comienzo (la izquierda) de una cadena: PRINT LEFT$("Nacho", 2) escribiría "Na".
//RIGHT$ extrae ciertos caracteres del final (la derecha) de una cadena: PRINT RIGHT$("Nacho", 3) escribiría "cho"
//MID$ extrae ciertos caracteres del cualquier posición de una cadena (se le indica el punto de partida, o opcionalmente, la cantidad de letras a tomar; si no se detalla este segundo dato, tomará hasta el final del texto: PRINT MID$("Nacho", 2,3) escribiría "ach"
//INSTR indica en qué posición de una cadena se encuentra cierto texto: PRINT INSTR("Nacho", "ach") diría "2". Si no existe, el resultado será 0. Se puede indicar un dato adicional, que es a partir de qué posición queremos buscar (este dado, si se incluye, debe ser el primero): PRINT INSTR(4, "Nacho", "ach") diría "0", porque no existe "ach" a partir de la cuarta letra de "Nacho".
//LOWER$ convierte a minúsculas: PRINT LOWER$("Nacho") mostraría "nacho"
//UPPER$ convierte a mayúsculas: PRINT UPPER$("Nacho") mostraría "NACHO"
//SPACE$ crea una cadena formada por varios espacios: separador$ = SPACE$(10)
//STRING$ crea una cadena formada por una letra repetida varias veces: guiones$ = STRING$(10,"-")

enum Keyword {
    KEYWORD_TABLE(KT_EXPAND_AS_ENUMERATION)
    NUM_KEYWORDS
};

char* findKeyword(unsigned char code);
char* findKeywordByImpl(instr_impl* impl);
int parseKeyword(char* input);

#endif /* keywords_h */
