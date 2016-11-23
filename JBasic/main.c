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
#include "errors.h"

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

// Interpreter
int main(int argc, const char * argv[]) {
    
    /*
    addLine(EVAL);
    addExpr_int(INT_K, 1234);
    addExpr_int(INT_K, 4567);
    addExpr_op(ADD);
    endLine();
    */
    
    addLine(TEST);
    addExpr_op(TEST);
    addExpr_op(TEST);
    endLine();
    

    /*
    addLine_string(ZERO, "VAR");
    addLine_int(GOSUB, 60);
    addLine_string(INC, "VAR");
    addLine_int(GOSUB, 60);
    addLine(END);
    addLine_string(PRINT, "VAR");
    addLine(RETURN);
    */
    
    /*
    addLine(LIST);
    addLine(TEST);
    */
    
    addLine(END);
    
    list_opcodes();
    printf("--------------------\r\n");
    //parse("LIST");
    run();
    
    return 0;
}
