#ifndef __lib_variant_hpp__
#define __lib_variant_hpp__

#include "meta.hpp"

namespace lib
{
  template<typename ... types_t>
  class variant
  {
    char storage[]
  };
}

#endif