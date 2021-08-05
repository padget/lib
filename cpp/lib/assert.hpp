#ifndef __lib_assert_hpp__
#define __lib_assert_hpp__

#ifndef NDEBUG
#define contract(expr, error) ((void)0)
#else
#define contract(expr, error) \
  if (not(expr))              \
    throw error();
#endif
#endif
