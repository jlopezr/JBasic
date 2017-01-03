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
//parse("LIST");

int main(int argc, const char * argv[]) {
    
//    // 10 ZERO VAR
//    addLine_string(ZERO, "VAR");
//    // 20 INC VAR
//    addLine_string(INC, "VAR");
//    // 30 INC VAR
//    addLine_string(INC, "VAR");
//    
//    // 40 LET OTHER_VAR = 8 + VAR
//    addLine_string(LET, "OTHER_VAR");
//    addExpr_int(INT_K, 8);
//    addExpr_string(VAR, "VAR");
//    addExpr_op(ADD);
//    addExpr_op(END_EXPR);
//    endLine();
//    
//    // 50 LET JUAN = MAX(29, SQRT(POW(8,2)))
//    addLine_string(LET, "JUAN");
//    addExpr_int(INT_K, 8);
//    addExpr_int(INT_K, 2);
//    addExpr_op(POW);
//    addExpr_op(SQRT);
//    addExpr_int(INT_K, 29);
//    addExpr_op(MAX);
//    addExpr_op(END_EXPR);
//    endLine();
//    
//    // 60 PRINT VAR
//    addLine(PRINT);
//    addExpr_string(VAR, "VAR");
//    addExpr_op(END_EXPR);
//    endLine();
//    
//    // 70 PRINT OTHER_VAR
//    addLine(PRINT);
//    addExpr_string(VAR, "OTHER_VAR");
//    addExpr_op(END_EXPR);
//    endLine();
//    
//    // 80 PRINT JUAN
//    addLine(PRINT);
//    addExpr_string(VAR, "JUAN");
//    addExpr_op(END_EXPR);
//    endLine();
//     
//    // 90 LIST
//    addLine(LIST);
//    
//    /*
//    addLine_string(ZERO, "VAR");
//    addLine_int(GOSUB, 60);
//    addLine_string(INC, "VAR");
//    addLine_int(GOSUB, 60);
//    addLine(END);
//    addLine_string(PRINT, "VAR");
//    addLine(RETURN);
//    */
//    
//    // 100 END
//    addLine(END);
    
    
    //10 LET A = 23 + 45
    addLine_string(LET, "A");
    addExpr_int(INT_K, 23);
    addExpr_int(INT_K, 45);
    addExpr_op(ADD);
    addExpr_op(END_EXPR);
    endLine();
    
    //20 PRINT A
    addLine(PRINT);
    addExpr_string(VAR, "A");
    addExpr_op(END_EXPR);
    endLine();

    //30 END
    addLine(END);
    
    printf("====================\r\n");
    list_opcodes();
    printf("--------------------\r\n");
    run();
    
    return 0;
}
