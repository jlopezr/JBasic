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

char* findKeywordByImpl(instr_impl* impl) {
    for(int i=0;i<NUM_KEYWORDS;i++) {
        if(impl == keywords[i].impl) {
            return keywords[i].name;
        }
    }
    return 0;
}

int parseKeyword(char* input) {
    for(int i=0;i<NUM_KEYWORDS;i++) {
        if(strncasecmp(input, keywords[i].name, strlen(keywords[i].name))==0) {
            return i;
        }
    }
    return -1;
}
