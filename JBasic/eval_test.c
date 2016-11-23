//
//  eval_test.c
//  JBasic
//
//  Created by Juan López Rubio on 22/11/16.
//  Copyright © 2016 Juan López Rubio. All rights reserved.
//

#include <stdio.h>
#include "minunit.h"
#include "eval.h"

static char * test_eval_stack() {
    Stack s;
    Atom a,a1,a2;
    char ret;
    
    stack_init(&s);
    mu_assert("error, stack_pos should be 0 at init", ret == 0 && s.stack_pos == 0);
    a.type = ATOM_INT;
    a.integer = 10;
    ret = peek(&s, &a1);
    mu_assert("error, stack peek should fail if stack empty", ret == -1 && s.stack_pos == 0);
    ret = push(&s, a);
    mu_assert("error, stack_pos should be 1 after push", ret == 0 && s.stack_pos == 1);
    ret = peek(&s, &a1);
    mu_assert("error, stack peek should return number 1", ret == 0 && a1.type == ATOM_INT && a1.integer == 10);
    ret = pop(&s, &a2);
    mu_assert("error, stack poke should return number 1", ret == 0 && a2.type == ATOM_INT && a2.integer == 10);
    mu_assert("error, stack_pos should be 0 after pop", ret == 0 && s.stack_pos == 0);
    ret = pop(&s, &a2);
    mu_assert("error, pop without elements should return -1", ret == -1 && s.stack_pos == 0);
    
    return 0;
}

static char * test_eval_stack_full() {
    Stack s;
    Atom a = {0, 0};
    char ret;
    
    stack_init(&s);
    for(int i=0;i<STACK_MAX;i++) {
        a.type = ATOM_INT;
        a.integer = i;
        ret = push(&s, a);        
        mu_assert("error, stack_pos should grow ", ret == 0 && s.stack_pos == i+1);
    }
    
    ret = push(&s, a);
    mu_assert("error, stack_pos should not grow when full", ret == -1 && s.stack_pos == STACK_MAX);
    
    print_stack(&s);
    
    for(int i=0;i<STACK_MAX;i++) {
        ret = pop(&s, &a);
        mu_assert("error, stack_pos should shrink ", ret == 0 && s.stack_pos == STACK_MAX-i-1 && a.integer == STACK_MAX-i-1);
    }
    
    ret = pop(&s, &a);
    mu_assert("error, stack_pos should not shrink when empty", ret == -1 && s.stack_pos == 0);
    
    return 0;
}

static char * all_tests_eval() {
    mu_run_test(test_eval_stack);
    mu_run_test(test_eval_stack_full);
    return 0;
}

int test_eval(int argc, char **argv) {
    tests_run = 0;
    char *result = all_tests_eval();
    
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);
    
    return result != 0;
}
