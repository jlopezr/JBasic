//
//  minunit.h
//  JBasic
//
//  From http://www.jera.com/techinfo/jtns/jtn002.html
//

#ifndef minunit_h
#define minunit_h

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; if (message) return message; } while (0)
extern int tests_run;

#endif /* minunit_h */
