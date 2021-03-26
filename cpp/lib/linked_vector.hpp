#ifndef __lib_linked_vector_hpp__
#define __lib_linked_vector_hpp__

#include "vector.hpp"

namespace lib
{
  template <typename type_t>
  class linked_vector
  {
    vector<vector<type_t>> data;
    std::size_t step = 100;
    std::size_t lght = 0;

  public:
    explicit linked_vector() = default;
    ~linked_vector() = default;

  public:
    const std::size_t &size() const
    {
      return lght;
    }

  public:
    type_t &operator[](std::size_t i)
    {
      
    }

    const type_t &operator[](std::size_t i) const
    {

    }

  public:
    void push_back(const type_t &t)
    {
      if (size % step == 0)
      {
        data.emplace_back(vector<type_t>(step));

      }

      operator[](size() + 1) = t;
    }

    void emplace_back(type_t &&t)
    {
      if (size % step == 0)
      {
        data.emplace_back(vector<type_t>(step));
      }

      operator[](size() + 1) = static_cast<type_t &&>(t);
    }
  };
}

#endif