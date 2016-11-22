//
//  instructions.h
//  JBasic
//
//  Created by Juan López Rubio on 21/11/16.
//  Copyright © 2016 Juan López Rubio. All rights reserved.
//

#ifndef instructions_h
#define instructions_h

#include <stdio.h>
#include "program.h"

#define NUM_GOSUB_CALLS  5

char _end(Line* line);
char _test(Line* line);
char _goto(Line* line);
char _list(Line* line);
char _zero(Line* line);
char _inc(Line* line);
char _dec(Line* line);
char _print(Line* line);
char _gosub(Line* line);
char _return(Line* line);
char _not_implemented(Line* line);

#endif /* instructions_h */
