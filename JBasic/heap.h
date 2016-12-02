//
//  heap.h
//  JBasic
//
//  Created by Juan López Rubio on 29/11/16.
//  Copyright © 2016 Juan López Rubio. All rights reserved.
//

#ifndef heap_h
#define heap_h

#include <stdio.h>

extern void* heap;
extern void* first_free; //for unit testing

void init_heap();
void* heap_alloc(int size);
void heap_free(void* p);
int heap_free_space(char debug);
void heap_gc();

#endif /* heap_h */
