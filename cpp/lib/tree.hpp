#ifndef __lib_tree_hpp__
#define __lib_tree_hpp__

#include "vector.hpp"

namespace lib
{
  namespace detail
  {
    template <typename key_t, typename type_t>
    struct tree_node
    {
      key_t k;
      type_t t;
      std::int64_t next;
      std::int64_t child;
    };

    template <typename type_t>
    struct tree
    {
      lib::vector<tree_node<type_t>> data;
    };
  }
}

#endif