//
//  sym_test.c
//  JBasic
//
//  Created by Juan López Rubio on 21/11/16.
//  Copyright © 2016 Juan López Rubio. All rights reserved.
//

#include <stdio.h>
#include "minunit.h"
#include "sym.h"

static char * test_sym_simple() {
    set_string_var("A", "HELLO");
    set_string_var("B", "WORLD");
    set_string_var("C", "BYE BYE");
    
    mu_assert("error, D should not be found", get_string_var("D")==NULL);
    mu_assert("error, A should be HELLO", strcmp(get_string_var("A"),"HELLO")==0);
    mu_assert("error, B should be WORLD", strcmp(get_string_var("B"),"WORLD")==0);
    mu_assert("error, C should be BYE BYE", strcmp(get_string_var("C"),"BYE BYE")==0);
    
    return 0;
}

static char * test_sym_long_names() {
    set_string_var("1234567890", "LONG!");
    set_string_var("1234567890123456789012345678901234567890", "VERY LONG!");

    mu_assert("error, 1234567890 should be LONG!",
              strcmp(get_string_var("1234567890"),"LONG!")==0);
    mu_assert("error, 0987654321 should not be found",
              get_string_var("0987654321")==NULL);
    
    mu_assert("error, 1234567890123456789012345678901234567890 should be VERY LONG!",
              strcmp(get_string_var("1234567890123456789012345678901234567890"),"VERY LONG!")==0);
    mu_assert("error, 0987654321098765432109876543210987654321 should not be found",
              get_string_var("0987654321098765432109876543210987654321")==NULL);
    
    return 0;
}

static char * test_sym_fill_strings() {
    char buffer[40];
    
    clear_string_vars();
    for(int i=0;i<MAX_STRING_VARS;i++) {
        sprintf(buffer, "VAR%d", i);
        mu_assert("error, At least MAX_STRING_VARS can be set", set_string_var(buffer, buffer) == ERR_OK);
    }
    
    sprintf(buffer, "VAR%d", MAX_STRING_VARS+1);
    mu_assert("error, Only MAX_STRING_VARS can be set", set_string_var(buffer, buffer) == ERR_STRINGS_FULL);
    
    for(int i=0;i<MAX_STRING_VARS;i++) {
        sprintf(buffer, "VAR%d", i);
        mu_assert("error, VARxx value must be VARxx", strcmp(buffer,get_string_var(buffer))==0);
    }
    return 0;
}

static char * all_tests_sym() {
    mu_run_test(test_sym_simple);
    mu_run_test(test_sym_long_names);
    mu_run_test(test_sym_fill_strings);
    return 0;
}

int test_sym(int argc, char **argv) {
    tests_run = 0;
    char *result = all_tests_sym();
    
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);
    
    return result != 0;
}
