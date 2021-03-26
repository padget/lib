#ifndef __lib_tuple_hpp__
#define __lib_tuple_hpp__

#include <cstddef>

namespace lib
{
  template <std::size_t i, typename type_t>
  struct tuple_element
  {
    type_t t;
  };

  template <typename... type_t>
  class tuple : public tuple_element<type_t, 0>...
  {
  };
}

#endif