#ifndef __lib_assert_hpp__
#define __lib_assert_hpp__ 

#ifndef NDEBUG
  #define contract(expr) ((void) 0)
#else
  #define contract(expr) if(not expr) throw error();
#endif

#endif
