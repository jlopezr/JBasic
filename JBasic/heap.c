//
//  heap.c
//  JBasic
//
//  Created by Juan López Rubio on 29/11/16.
//  Copyright © 2016 Juan López Rubio. All rights reserved.
//

#include "heap.h"
#include <stdlib.h>

void* heap;
void* first_free;


void set_free_buffer(void* buf, int size, void* next) {
    int* p = (int*)buf;
    *p = size;
    p++;

    void** p2 = (void**)p;
    *p2 = (void*)next;
}

void get_free_buffer(void* buf, int* size, void** next) {
    int* p = (int*)buf;
    *size = *p;
    p++;
    
    void** p2 = (void**)p;
    *next = *p2;
}

void* set_used_buffer(void* buf, int size) {
    int* p = (int*)buf;
    *p = size;
    p++;
    return p;
}

void init_heap() {
    heap = malloc(64*1024);
    first_free = heap;
    
    set_free_buffer(first_free, 64*1024, 0);
}

void* heap_alloc(int bytes) {
    void* p = first_free;
    int size = 0;
    void* next = 0;
    
    while(p != 0) {
        get_free_buffer(p, &size, &next);
        
        if(size >= bytes + sizeof(int)) {
            void* ret = set_used_buffer(p, bytes);
            
            first_free = (char*)ret + bytes;
            set_free_buffer(first_free, size - bytes - sizeof(int), next);
            return ret;
        }
        
        p = next;
    }
    return 0;
}

void heap_free(void* p) {
    int* p2 = (int*)p;
    p2--;
    
    int size = 0;
    void* dummy = 0;
    get_free_buffer(p2, &size, &dummy);
    set_free_buffer(p2, size + sizeof(int), first_free);
    first_free = p2;
}

int heap_free_space(char debug) {
    int total = 0;
    
    int* p = (int*)first_free;
    while(p != 0) {
        total = total + *p;
        
        if(debug) {
            printf("%p \t %d\r\n", p, *p);
        }
        
        p++;
        
        void** next = (void**)p;
        p = (int*)*next;
    }
    
    if(debug) {
        printf("Total free space %u\r\n", total);
        printf("--------------------------------------\r\n");
    }
    
    return total;
}
