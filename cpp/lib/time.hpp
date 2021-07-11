#ifndef __lib_time_hpp__
#define __lib_time_hpp__
#include <ctime>

namespace lib
{

  class timestamp
  {
  };

  class date
  {
  };

  enum class timeunit
  {
    nanos,
    micros,
    millis,
    second, 
    minute, 
    hour, 
    day
  };

  class duration
  {
    // value
    // unit
  };

  class timer
  {
    // start
    // stop
    // step
  };

}

#endif