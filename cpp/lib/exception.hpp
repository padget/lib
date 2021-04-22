#ifndef __lib_exception_hpp__
#define __lib_exception_hpp__

#define BASIC_EXCEPTION(exception) \
  struct exception                 \
  {                                \
  };

#define INHERITED_EXCEPTION(exception, base) \
  struct exception : public base             \
  {                                          \
  };

#endif