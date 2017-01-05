//
//  sym.c
//  JBasic
//
//  Created by Juan López Rubio on 21/11/16.
//  Copyright © 2016 Juan López Rubio. All rights reserved.
//

#include "sym.h"

typedef struct {
    char name[SYMBOL_NAME_LENGTH];
    char value[STRING_MAX_LENGTH];
} StringVar;

typedef struct {
    char name[SYMBOL_NAME_LENGTH];
    int value;
} IntegerVar;

StringVar strings[MAX_STRING_VARS];
IntegerVar ints[MAX_INT_VARS];
int strings_used = 0;
int ints_used = 0;

StringVar* allocate_string_var() {
    if(strings_used<MAX_STRING_VARS) {
        return &strings[strings_used++];
    } else {
        return NULL;
    }
}

StringVar* find_string_var(char* name) {
    for(int i=0;i<strings_used;i++) {
        if(strncmp(name, strings[i].name, SYMBOL_NAME_LENGTH)==0) {
            return &strings[i];
        }
    }
    return NULL;
}

char set_string_var(char* name, char* value) {
    StringVar* v = find_string_var(name);
    if(v==NULL) {
        v = allocate_string_var();
        if(v==NULL) {
            return ERR_STRINGS_FULL;
        } else {
            strncpy(v->name, name, SYMBOL_NAME_LENGTH);
            strncpy(v->value, value, STRING_MAX_LENGTH);
            return ERR_OK;
        }
    } else {
        strncpy(v->value, value, STRING_MAX_LENGTH);
        return ERR_OK;
    }
}

char* get_string_var(char* name) {
    StringVar* v = find_string_var(name);
    if(v==NULL) {
        return 0;
    } else {
        return (char*)&(v->value);
    }
}

void clear_string_vars() {
    strings_used = 0;
}

IntegerVar* allocate_int_var() {
    if(strings_used<MAX_INT_VARS) {
        return &ints[ints_used++];
    } else {
        return NULL;
    }
}

IntegerVar* find_int_var(char* name) {
    for(int i=0;i<ints_used;i++) {
        if(strncmp(name, ints[i].name, SYMBOL_NAME_LENGTH)==0) {
            return &ints[i];
        }
    }
    return NULL;
}

char set_int_var(char* name, int value) {
    IntegerVar* v = find_int_var(name);
    if(v==NULL) {
        v = allocate_int_var();
        if(v==NULL) {
            return ERR_INTS_FULL;
        } else {
            strncpy(v->name, name, SYMBOL_NAME_LENGTH);
            v->value = value;
            return ERR_OK;
        }
    } else {
        v->value = value;
        return ERR_OK;
    }
}

int* get_int_var(char*name) {
    IntegerVar* v = find_int_var(name);
    if(v==NULL) {
        return 0;
    } else {
        return &(v->value);
    }
}

void clear_int_vars() {
    ints_used = 0;
}


