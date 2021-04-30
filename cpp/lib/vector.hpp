#ifndef __lib_vector_hpp__
#define __lib_vector_hpp__

#include <cstddef>
#include <lib/span.hpp>
#include <lib/utility.hpp>

namespace lib
{

  template <typename type_t>
  class vector
  {
    std::size_t lgth = 0;
    std::size_t max = 0;
    type_t *data = nullptr;

  public:
    explicit vector(
        std::size_t cap = 10)
        : max(cap),
          data(new type_t[max]) {}

    vector(
        const vector<type_t> &v)
        : vector(v.max)
    {
      for (auto &&i : v)
        push_back(i);
    }

    vector(
        vector<type_t> &&v)
        : lgth(v.lgth),
          max(v.max),
          data(v.data)
    {
      v.lgth = 0;
      v.max = 0;
      v.data = nullptr;
    }

    template <typename iterator_t>
    explicit vector(
        iterator_t b,
        iterator_t e)
        : vector(e - b)
    {
      for (; b != e; ++b)
        push_back(*b);
    }

    ~vector()
    {
      delete[] data;
      lgth = 0;
      max = 0;
    }

  public:
    inline vector<type_t> &
    operator=(
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

    inline vector<type_t> &
    operator=(
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
    inline void 
    reserve(
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
          *newb = move(*oldb);
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
    inline type_t &
    operator[](
        std::size_t i)
    {
      return data[i];
    }

    inline const type_t &
    operator[](
        std::size_t i) const
    {
      return data[i];
    }

  public:
    inline std::size_t
    size() const
    {
      return lgth;
    }

    inline bool
    empty() const
    {
      return size() == 0;
    }

    inline std::size_t
    capacity() const
    {
      return max;
    }

  public:
    inline void
    push_back(
        const type_t &t)
    {
      if (size() == capacity())
        reserve(capacity() +
                capacity() / 2);

      operator[](size()) = t;
      ++lgth;
    }

    inline void
    emplace_back(
        type_t &&t)
    {
      if (size() == capacity())
        reserve(capacity() +
                capacity() / 2);

      operator[](size()) = move(t);
      ++lgth;
    }

  public:
    template <std::size_t n>
    inline bool operator==(
        const type_t (&o)[n])
    {
      return span<type_t>(begin(), end()) ==
             span<const type_t>(o);
    }

  public:
    inline type_t *
    begin()
    {
      return data;
    }

    inline type_t *
    end()
    {
      return data + lgth;
    }

    inline const type_t *
    begin() const
    {
      return data;
    }

    inline const type_t *
    end() const
    {
      return data + lgth;
    }

    inline std::size_t
    front_index() const
    {
      return 0;
    }

    inline const type_t &
    front() const
    {
      return *begin();
    }

    inline type_t &
    front()
    {
      return *begin();
    }

    inline std::size_t
    back_index() const
    {
      return size() - 1;
    }

    inline const type_t &
    back() const
    {
      return *(end() - 1);
    }

    inline type_t &
    back()
    {
      return *(end() - 1);
    }
  };
}

#endif