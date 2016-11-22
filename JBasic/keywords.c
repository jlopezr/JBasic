//
//  keywords.c
//  JBasic
//
//  Created by Juan López Rubio on 21/11/16.
//  Copyright © 2016 Juan López Rubio. All rights reserved.
//

#include <strings.h>
#include "keywords.h"
#include "instructions.h"

Keyword keywords[NUM_KEYWORDS] = {
    KEYWORD_TABLE(KT_EXPAND_AS_TABLE)
};

char* findKeyword(unsigned char code) {
    return keywords[code].name;
}

int parseKeyword(char* input) {
    for(int i=0;i<NUM_KEYWORDS;i++) {
        if(strncasecmp(input, keywords[i].name, strlen(keywords[i].name))==0) {
            return i;
        }
    }
    return -1;
}
