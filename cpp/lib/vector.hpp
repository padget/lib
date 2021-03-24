#ifndef __lib_vector_hpp__
#define __lib_vector_hpp__

#include <cstddef>
#include "span.hpp"

namespace lib
{

  template <typename type_t>
  class vector
  {
    std::size_t lgth = 0;
    std::size_t max = 0;
    type_t *data = nullptr;

  public:
    explicit vector(std::size_t cap = 10)
        : max(cap), data(new type_t[max]) {}

    vector(const vector<type_t> &v)
        : vector(v.max)
    {
      for (auto &&i : v)
        push_back(i);
    }

    vector(vector<type_t> &&v)
        : lgth(v.lgth), max(v.max), data(v.data)
    {
      v.lgth = 0;
      v.max = 0;
      v.data = nullptr;
    }

    ~vector()
    {
      delete[] data;
      lgth = 0;
      max = 0;
    }

  public:
    vector<type_t> &operator=(
        const vector<type_t> &v)
    {
      if (this != &v)
      {
        lgth = v.lgth;
        max = v.max;
        reserve(max);

        for (auto &&i : v)
          push_back(i);
      }

      return *this;
    }

    vector<type_t> &operator=(
        vector<type_t> &&v)
    {
      if (this != &v)
      {
        lgth = v.lgth;
        max = v.max;
        data = v.data;
        v.data = nullptr;
        v.max = 0;
        v.lgth = 0;
      }

      return *this;
    }

  public:
    void reserve(
        std::size_t ns)
    {
      if (ns > max)
      {
        type_t *old_data = data;
        type_t *oldb = old_data;
        type_t *olde = old_data + lgth;
        type_t *new_data = new type_t[ns];
        type_t *newb = new_data;

        while (oldb != olde)
        {
          *newb = static_cast<type_t&&>(*oldb);
          ++newb;
          ++oldb;
        }

        delete[] old_data;

        max = ns;
        data = new_data;
      }
      else if (ns < lgth)
        lgth = ns;
    }

  public:
    type_t &operator[](std::size_t i)
    {
      return data[i];
    }

    const type_t &operator[](std::size_t i) const
    {
      return data[i];
    }

  public:
    const std::size_t &size() const
    {
      return lgth;
    }

    const std::size_t &capacity() const
    {
      return max;
    }

  public:
    void push_back(const type_t &t)
    {
      if (size() == capacity())
        reserve(capacity() + capacity() / 2);

      operator[](size()) = t;
      ++lgth;
    }

    void emplace_back(type_t &&t)
    {
      if (size() == capacity())
        reserve(capacity() + capacity() / 2);

      operator[](size()) = static_cast<type_t &&>(t);
      ++data.lgth;
    }

  public:
    template <std::size_t n>
    bool operator==(const type_t (&o)[n])
    {
      return span<type_t>(begin(), end()) == span<const type_t>(o);
    }

  public:
    type_t *begin() { return data; }
    type_t *end() { return data + lgth; }
    const type_t *begin() const { return data; }
    const type_t *end() const { return data + lgth; }
  };
}

#endif