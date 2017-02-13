//
//  main.c
//  JBasic
//
//  Created by Juan López Rubio on 8/5/16.
//  Copyright © 2016 Juan López Rubio. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "program.h"
#include "keywords.h"
#include "instructions.h"
#include "errors.h"

/**
 * This is the main file of JBasic.
 */

// Parser
void parse(char* input) {
    char* buffer;
    int n=0;
    char ch = input[n];
    
    while(ch!=0 && n < 1024) {
        if(ch==' '||ch=='\t') {
            ch=input[++n];
            continue;
        }
        int keyword = parseKeyword(input);
        if(keyword>=0) {
            buffer = keywords[keyword].name;
            printf("%s", buffer);
            n += strlen(buffer);
            ch = input[n];
        }
    }
}
//parse("LIST");

int main(int argc, const char * argv[]) {

    test_main();
        
    return 0;
}
