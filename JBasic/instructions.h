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
char _let(Line* line);

char _int_constant(Line* line);
char _var(Line* line);
char _add(Line* line);
char _sub(Line* line);
char _mul(Line* line);
char _div(Line* line);
char _eval(Line* line);
char _end_of_expr(Line* line);

char _min(Line* line);
char _max(Line* line);
char _pow(Line* line);
char _sqrt(Line* line);

char _not_implemented(Line* line);

#endif /* instructions_h */
