//
//  test.c
//  JBasic
//
//  Created by Juan López Rubio on 4/1/17.
//  Copyright © 2017 Juan López Rubio. All rights reserved.
//

#include <stdio.h>
#include <strings.h>
#include "minunit.h"
#include "sym.h"
#include "program.h"
#include "keywords.h"
#include "instructions.h"
#include "heap.h"


static char * test_main_hello() {
    reset();
    
    addLine();
    addStringOp(LET, "SAX");
    addIntOp(INT_K, 10);
    addOp(END_EXPR);

    addLine();
    addOp(END);
    endLine();
    
    run();
    
    int* p = get_int_var("SAX");
    mu_assert("Variable SAX must have a value", p != 0);
    mu_assert("Variable SAX must have a 10", *p == 10);
    p = get_int_var("SEX");
    mu_assert("Variable SEX must have not have a value", p == 0);
    
    return 0;
}

static char * test_main_expression1() {
    reset();
    
    addLine();
    addStringOp(LET, "A");
    addIntOp(INT_K, 6);
    addIntOp(INT_K, 4);
    addOp(MUL);
    addOp(END_EXPR);
    
    addLine();
    addOp(END);
    endLine();
    
    run();
    
    int* p = get_int_var("A");
    mu_assert("Variable A must have a value", p != 0);
    mu_assert("Variable A must have a 24", *p == 24);
    
    return 0;
}

static char * test_main_expression2() {
    reset();
    
    addLine();
    addStringOp(LET, "M");
    addIntOp(INT_K, 8);
    addIntOp(INT_K, 2);
    addOp(MAX);
    addOp(END_EXPR);

    addLine();
    addStringOp(LET, "N");
    addIntOp(INT_K, 8);
    addIntOp(INT_K, 2);
    addOp(MIN);
    addOp(END_EXPR);
    
    addLine();
    addOp(END);
    endLine();
    
    run();
    
    int* p = get_int_var("M");
    mu_assert("Variable M must have a value", p != 0);
    mu_assert("Variable M must have a 16", *p == 8);
    p = get_int_var("N");
    mu_assert("Variable N must have a value", p != 0);
    mu_assert("Variable N must have a 16", *p == 2);
    
    return 0;
}

static char * test_main_goto() {
    reset();
    
    addLine();
    addStringOp(LET, "V");
    addIntOp(INT_K, 1);
    addOp(END_EXPR);
    
    addLine();
    addIntOp(GOTO, 40);
    
    addLine();
    addStringOp(LET, "V");
    addIntOp(INT_K, 2);
    addOp(END_EXPR);
    
    addLine();
    addOp(END);
    endLine();
    
    run();
    
    int* p = get_int_var("V");
    mu_assert("Variable V must have a value", p != 0);
    mu_assert("Variable V must have a 1", *p == 1);
    
    return 0;
}

static char * test_main_gosub() {
    reset();

    addLine();
    addStringOp(LET, "V");
    addIntOp(INT_K, 0);
    addOp(END_EXPR);
    
    addLine();
    addIntOp(GOSUB, 60);
    addIntOp(GOSUB, 60);
    addIntOp(GOSUB, 60);
    
    addLine();
    addIntOp(GOSUB, 60);
    addIntOp(GOSUB, 60);
    
    addLine();
    addIntOp(GOSUB, 60);
    
    addLine();
    addOp(END);
    
    addLine();
    addStringOp(LET, "V");
    addIntOp(INT_K, 1);
    addStringOp(VAR, "V");
    addOp(ADD);
    addOp(END_EXPR);
    
    addLine();
    addOp(RETURN);
    endLine();
    
    list_opcodes();
    printf("____\r\n");
    _list();
    printf("____\r\n");
    run();
    
    int* p = get_int_var("V");
    mu_assert("Variable V must have a value", p != 0);
    mu_assert("Variable V must have a 1", *p == 1);
    
    return 0;
}

static char * test_main_stepping() {
    reset();
    
    addLine();
    addStringOp(LET, "V");
    addIntOp(INT_K, 1);
    addOp(END_EXPR);
    
    addLine();
    addStringOp(LET, "V");
    addIntOp(INT_K, 2);
    addOp(END_EXPR);
    
    addLine();
    addStringOp(LET, "V");
    addIntOp(INT_K, 3);
    addOp(END_EXPR);
    
    addLine();
    addOp(END);
    endLine();
    
    step(each_line);
    int* p = get_int_var("V");
    mu_assert("1 Variable V must have a value", p != 0);
    mu_assert("Variable V must have a 1", *p == 1);
    
    step(each_line);
    p = get_int_var("V");
    mu_assert("2 Variable V must have a value", p != 0);
    mu_assert("Variable V must have a 2", *p == 2);
    
    run();
    p = get_int_var("V");
    mu_assert("3 Variable V must have a value", p != 0);
    mu_assert("Variable V must have a 3", *p == 3);

    return 0;
}

static char * all_tests_main() {
    mu_run_test(test_main_hello);
    mu_run_test(test_main_expression1);
    mu_run_test(test_main_expression2);
    mu_run_test(test_main_goto);
    //mu_run_test(test_main_gosub);
    mu_run_test(test_main_stepping);
    return 0;
}

int test_main(int argc, char **argv) {
    tests_run = 0;
    char *result = all_tests_main();
    
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);
    
    return result != 0;
}
