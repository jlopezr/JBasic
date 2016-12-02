//
//  heap_test.c
//  JBasic
//
//  Created by Juan López Rubio on 29/11/16.
//  Copyright © 2016 Juan López Rubio. All rights reserved.
//

#include <stdio.h>
#include "minunit.h"
#include "heap.h"

static char * test_heap_alloc() {

    init_heap();
    mu_assert("error, heap free space is not 64k", heap_free_space(1) == 64*1024);
    
    char* p1 = heap_alloc(12);
    mu_assert("error, returned pointer is not one int after start of heap", p1 == ((char*)heap + sizeof(int)));
    char* end_p1 = (char*)heap + sizeof(int) + 12;
    mu_assert("error, first_free is not 12 bytes after returned pointer", first_free == (void*)end_p1);
    
    char* p2 = heap_alloc(80);
    mu_assert("error, returned pointer is not one int after last returned pointer", p2 == (void*)(end_p1 + sizeof(int)));
    char* end_p2 = end_p1 + sizeof(int) + 80;
    mu_assert("error, first_free is not 80 bytes after returned pointer", first_free == (void*)end_p2);
    mu_assert("error, heap free space is not 65436", heap_free_space(1) == 64*1024 - 2*sizeof(int) - 12 - 80);
    
    heap_free(p2);
    mu_assert("error, heap free space is not 65520", heap_free_space(1) == 64*1024 - 1*sizeof(int) - 12);
    
    heap_free(p1);
    mu_assert("error, heap free space is not 64k again", heap_free_space(1) == 64*1024);
    
    char* p3 = heap_alloc(100);
    mu_assert("error, returned pointer is not one int after last returned pointer", p3 == (void*)(end_p2 + sizeof(int)));
    char* end_p3 = end_p2 + sizeof(int) + 100;
    mu_assert("error, first_free is not 100 bytes after returned pointer", first_free == (void*)end_p3);
    mu_assert("error, heap free space is not XXX", heap_free_space(1) == 64*1024 - 2*sizeof(int) - 12 - 100);
    return 0;
}

static char * all_tests_heap() {
    mu_run_test(test_heap_alloc);
    return 0;
}

int test_heap(int argc, char **argv) {
//int main(int argc, char **argv) {
    tests_run = 0;
    char *result = all_tests_heap();
    
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);
    
    return result != 0;
}
