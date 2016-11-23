//
//  errors.h
//  JBasic
//
//  Created by Juan López Rubio on 21/11/16.
//  Copyright © 2016 Juan López Rubio. All rights reserved.
//

#ifndef errors_h
#define errors_h

extern char* errors[];

#define EXPAND_AS_ENUMERATION(a,b) a,
#define EXPAND_AS_TABLE(a,b) b,

#define STATE_TABLE(ENTRY) \
ENTRY(ERR_OK,             "OK1")                                \
ENTRY(ERR_STRINGS_FULL,   "OUT OF STRING MEM")                  \
ENTRY(ERR_INTS_FULL,      "OUT OF INT MEM")                     \
ENTRY(ERR_VAR_NOT_FOUND,  "VAR NOT FOUND")                      \
ENTRY(ERR_LINE_NOT_EXIST, "LINE DOES NOT EXISTS")               \
ENTRY(ERR_GOSUB_DEPTH,    "MAX GOSUB DEPTH")                    \
ENTRY(ERR_GOSUB_MISSING,  "RETURN WITHOUT GOSUB")               \
ENTRY(ERR_BAD_SYNTAX,     "BAD SYNTAX")                         \
ENTRY(ERR_DIV_ZERO,       "DIV BY ZERO")                        \
ENTRY(ERR_OK_JUMP,        "OK2")                                \
ENTRY(ERR_OK_END,         "OK3")

enum Error {
    STATE_TABLE(EXPAND_AS_ENUMERATION)
    NUM_ERRORS
};

#endif /* errors_h */
