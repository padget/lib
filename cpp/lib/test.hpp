#ifndef __clon_test_hpp__
#define __clon_test_hpp__

#include "format.hpp"
#include "ios.hpp"

#define test_equals(actual, expected)             \
  lib::printfln("===== at l.# test # == # : #",   \
                __LINE__, (#actual), (#expected), \
                (((actual) == (expected)) ? "OK" : "KO"));

#define test_not_equals(actual, expected)         \
  lib::printfln("===== at l.# test # != # : #",   \
                __LINE__, (#actual), (#expected), \
                (((actual) != (expected)) ? "OK" : "KO"));

#define test_catch(torun, exception)  \
  try                                 \
  {                                   \
    (torun);                          \
    lib::println("no exception KO");  \
  }                                   \
  catch (exception & e)               \
  {                                   \
    lib::println("OK");               \
  }                                   \
  catch (...)                         \
  {                                   \
    lib::println("bad exception KO"); \
  }

#define run_test(testname)                      \
  lib::println("\n--------------------------"); \
  lib::printfln("=  test file #", __FILE__);    \
  lib::printfln("=== run #", #testname);        \
  testname();

#endif
