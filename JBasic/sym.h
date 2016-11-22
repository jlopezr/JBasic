//
//  sym.h
//  JBasic
//
//  Created by Juan López Rubio on 21/11/16.
//  Copyright © 2016 Juan López Rubio. All rights reserved.
//

#ifndef sym_h
#define sym_h

#include <stdio.h>
#include <string.h>
#include "errors.h"

#define SYMBOL_NAME_LENGTH  40
#define STRING_MAX_LENGTH   255

#define MAX_STRING_VARS     100
#define MAX_INT_VARS        100

char set_string_var(char* name, char* value);
char* get_string_var(char* name);
void clear_string_vars();
char set_int_var(char* name, int value);
char get_int_var(char* name, int* value);
void clear_int_vars();

#endif /* sym_h */
