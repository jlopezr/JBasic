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

char _end();
char _test();
char _goto();
char _list();
char _zero();
char _inc();
char _dec();
char _print();
char _gosub();
char _return();
char _let();

char _int_constant();
char _str_constant();
char _var();
char _add();
char _sub();
char _mul();
char _div();
char _less();
char _greater();
char _end_of_expr();

char _min();
char _max();
char _pow();
char _sqrt();
char _abs();
char _sgn();

char _not_implemented();

#endif /* instructions_h */
