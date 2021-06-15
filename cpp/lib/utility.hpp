#ifndef __lib_utility_hpp__
#define __lib_utility_hpp__

#include <lib/meta.hpp>

namespace lib
{
  template <typename type_t>
  constexpr remove_reference<type_t> &&
  move(type_t &&t)
  {
    return static_cast<remove_reference<type_t> &&>(t);
  }
}

#endif