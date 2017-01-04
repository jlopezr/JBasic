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


static char * test_main_hello() {
    mu_assert("error, C should be BYE BYE", strcmp("BYE BYE","BYE BYE")==0);
    
    return 0;
}

static char * all_tests_main() {
    mu_run_test(test_main_hello);
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
