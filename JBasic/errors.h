///
///  @file errors.h
///
///  This file encodes error codes and their messages.
///
//  Created by Juan López Rubio on 21/11/16.
//  Copyright © 2016 Juan López Rubio. All rights reserved.
//

#ifndef errors_h
#define errors_h

/// This array stores the error messages during runtime. See ::STATE_TABLE
extern char* errors[];

/// This macro expands the entry returning only the code (first parameter)
#define EXPAND_AS_ENUMERATION(a,b) a,
/// This macro expands the entry returning only the error text (second parameter)
#define EXPAND_AS_TABLE(a,b) b,

/// This macro encodes the table of error codes and messages.
/// Error codes inspired by http://www.nachocabanes.com/amstrad/curso/cpc12.php
#define STATE_TABLE(ENTRY) \
ENTRY(ERR_OK,             "OK1")                    /**< Keyword implementation finished correctly */ \
ENTRY(ERR_STRINGS_FULL,   "STRING SPACE FULL")      /**< Maximum number of string variables reached */ \
ENTRY(ERR_INTS_FULL,      "INTEGER SPACE FULL")     /**< Maximum number of integer variables reached */ \
ENTRY(ERR_VAR_NOT_FOUND,  "VAR NOT FOUND")          /**< Look up a variable that it is not in the symbol table */ \
ENTRY(ERR_LINE_NOT_EXIST, "LINE DOES NOT EXIST")    /**< Look up a line that it is not found */ \
ENTRY(ERR_GOSUB_DEPTH,    "MAX GOSUB DEPTH")        /**< Too many nested GOSUB calls */ \
ENTRY(ERR_GOSUB_MISSING,  "UNEXPECTED RETURN")      /**< A RETURN is found without GOSUB */ \
ENTRY(ERR_BAD_SYNTAX,     "SYNTAX ERROR")           \
ENTRY(ERR_BAD_TYPE,       "TYPE MISMATCH")          \
ENTRY(ERR_STACK_FULL,     "EXPRESSION TOO COMPLEX") /**< Stack gets full while parsing expression */ \
ENTRY(ERR_DIV_ZERO,       "DIVISION BY ZERO")       /**< An expression tried to divide by zero */ \
ENTRY(ERR_MEMORY_FULL,    "MEMORY FULL")            /**< A memory allocation failed */ \
ENTRY(ERR_OK_JUMP,        "OK2")                    /**< Keyword implementation finished correctly and signaled a jump to other line */ \
ENTRY(ERR_OK_END,         "OK3")                    /**< Keyword implementation finished correctly and signaled end of program */

/// This enumeration stores the posible errors codes for development. See ::STATE_TABLE
enum Error {
    STATE_TABLE(EXPAND_AS_ENUMERATION)
    NUM_ERRORS                              ///< Total number of errors
};

#endif /* errors_h */
