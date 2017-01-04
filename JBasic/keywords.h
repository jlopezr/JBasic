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

#define PARAM_VOID      0       // Nothing
#define PARAM_NUM       1       // A line number (positive number)
#define PARAM_VAR       2       // A variable identifier
#define PARAM_EXPR      3       // An expression
#define PARAM_TEXT      4       // A text
#define PARAM_ASSIGN    5       // An assignation (identifier = expression)
#define PARAM_NUM_OPT   6       // An optional number
#define PARAM_VAR_OPT   7       // An optional variable identifier
#define PARAM_LIST      8       // None | num | num - num
#define PARAM_PRINT     9       // N expressions split by ; or ,
#define PARAM_INPUT     10      // A variable or "text", variable
#define PARAM_1_INT     11      // one integer
#define PARAM_2_INT     12      // two integer
#define PARAM_1_STR     13      // one string
#define PARAM_2_STR     14      // one string
#define PARAM_STR_INT   15      // one string + one integer
#define PARAM_INT_STR   16      // one integer + one string
#define PARAM_STR_2_INT 17      // one string + two integer

/*
    Instruction format

    --------------------------------------------------
    | lineNumber: int | length: char | opcodes ....  |
    --------------------------------------------------
*/

typedef char instr_impl(Line* currentLine);

typedef struct {
    char* name;
    unsigned char onlyCLI : 1;
    unsigned char parameters;
    instr_impl* impl;
} Keyword;

extern Keyword keywords[];

#define KT_EXPAND_AS_ENUMERATION(a,b,c,d) a ,
#define KT_EXPAND_AS_TABLE(a,b,c,d) { #a , b , c , d } ,

#define KEYWORD_TABLE(ENTRY) \
ENTRY(END,      0, PARAM_VOID,      _end)               \
ENTRY(LET,      0, PARAM_ASSIGN,    _let)               \
ENTRY(PRINT,    0, PARAM_EXPR,      _print)             \
ENTRY(INPUT,    0, PARAM_INPUT,     _not_implemented)   \
ENTRY(AUTO,     1, PARAM_NUM_OPT,   _not_implemented)   \
ENTRY(EDIT,     1, PARAM_NUM_OPT,   _not_implemented)   \
ENTRY(LIST,     1, PARAM_NUM_OPT,   _list)              \
ENTRY(RENUM,    1, PARAM_VOID,      _not_implemented)   \
ENTRY(GOTO,     0, PARAM_NUM,       _goto)              \
ENTRY(GOSUB,    0, PARAM_NUM,       _gosub)             \
ENTRY(RETURN,   0, PARAM_VOID,      _return)            \
ENTRY(WHILE,    0, PARAM_EXPR,      _not_implemented)   \
ENTRY(WEND,     0, PARAM_VOID,      _not_implemented)   \
ENTRY(FOR,      0, PARAM_ASSIGN,    _not_implemented)   \
ENTRY(TO,       0, PARAM_EXPR,      _not_implemented)   \
ENTRY(NEXT,     0, PARAM_VAR_OPT,   _not_implemented)   \
ENTRY(IF,       0, PARAM_EXPR,      _not_implemented)   \
ENTRY(THEN,     0, PARAM_VOID,      _not_implemented)   \
ENTRY(ELSE,     0, PARAM_VOID,      _not_implemented)   \
ENTRY(ENDIF,    0, PARAM_VOID,      _not_implemented)   \
ENTRY(INT_K,    0, PARAM_NUM,       _int_constant)      \
ENTRY(VAR,      0, PARAM_VAR,       _var)               \
ENTRY(ADD,      0, PARAM_VOID,      _add)               \
ENTRY(SUB,      0, PARAM_VOID,      _sub)               \
ENTRY(MUL,      0, PARAM_VOID,      _mul)               \
ENTRY(DIV,      0, PARAM_VOID,      _div)               \
ENTRY(LESS,     0, PARAM_2_INT,     _less)              \
ENTRY(GREATER,  0, PARAM_2_INT,     _greater)           \
ENTRY(END_EXPR, 0, PARAM_VOID,      _end_of_expr)       \
ENTRY(MIN,      0, PARAM_2_INT,     _min)               \
ENTRY(MAX,      0, PARAM_2_INT,     _max)               \
ENTRY(POW,      0, PARAM_2_INT,     _pow)               \
ENTRY(SQRT,     0, PARAM_1_INT,     _sqrt)              \
ENTRY(ABS,      0, PARAM_1_INT,     _abs)               \
ENTRY(SGN,      0, PARAM_1_INT,     _sgn)               \
ENTRY(LEN,      0, PARAM_1_STR,     _not_implemented)   \
ENTRY(INSTR,    0, PARAM_2_STR,     _not_implemented)   \
ENTRY(MID,      0, PARAM_STR_2_INT, _not_implemented)   \
ENTRY(LEFT,     0, PARAM_STR_INT,   _not_implemented)   \
ENTRY(RIGHT,    0, PARAM_STR_INT,   _not_implemented)   \
ENTRY(LOWER,    0, PARAM_1_STR,     _not_implemented)   \
ENTRY(UPPER,    0, PARAM_1_STR,     _not_implemented)   \
ENTRY(SPACE,    0, PARAM_1_INT,     _not_implemented)   \
ENTRY(STRING,   0, PARAM_INT_STR,   _not_implemented)   \
ENTRY(TEST,     0, PARAM_VOID,      _test)              \
ENTRY(ZERO,     0, PARAM_VAR,       _zero)              \
ENTRY(INC,      0, PARAM_VAR,       _inc)               \
ENTRY(DEC,      0, PARAM_VAR,       _dec)

// int_k -> div should be marked as expression for use in rpn_to_infix

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
