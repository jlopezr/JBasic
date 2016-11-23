//
//  bsearch.c
//  JBasic
//
//  Created by Juan López Rubio on 15/5/16.
//  Copyright © 2016 Juan López Rubio. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include "minunit.h"

int cmpfunc(const void * a, const void * b)
{
    return ( *(int*)a - *(int*)b );
}

char* test_bsearch_basic ()
{
    int values[] = { 5, 20, 29, 32, 63 };
    int *item;
    int key;
    
    /* using bsearch() to find value 32 in the array */
    key = 32;
    item = (int*) bsearch (&key, values, 5, sizeof (int), cmpfunc);
    mu_assert("error, element 32 must be found", item != NULL);
    
    key = 22;
    item = (int*) bsearch (&key, values, 5, sizeof (int), cmpfunc);
    mu_assert("error, element 22 must not be found", item == NULL);
    
    return 0;
}

static char* all_tests_bsearch() {
    mu_run_test(test_bsearch_basic);
    return 0;
}

int test_bsearch(int argc, char **argv) {
    tests_run = 0;
    
    char *result = all_tests_bsearch();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);
    
    return result != 0;
}
