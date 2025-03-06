#ifndef ARRAY_H
#define ARRAY_H
#include <string.h>

// You can't look up the values of preprocessed macros in the debugger
// So, if you want to look at the value of JMAX and IMAX compile w/
// -DUSE_GLOBALS
#ifdef USE_GLOBALS
   int JMAX = 1000
   int IMAX = 1000
#else
//kah   #define JMAX 1000
   #define JMAX 100
   #define IMAX 100
#endif

/*** some global vars **/
extern double b[];

void array();

#endif
