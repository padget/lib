#ifndef __lib_vector_hpp__
#define __lib_vector_hpp__

#include <lib/utility.hpp>

namespace lib
{
  template <typename type_t>
  class vector
  {
    size_t lgth = 0;
    size_t max = 0;
    type_t *data = nullptr;

  public:
    explicit vector(
        size_t cap = 10)
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
        size_t ns)
    {

      if (ns > max)
      {
        type_t *old_data = data;
        data = new type_t[ns];
        memcpy(old_data, old_data + lgth, data);
        delete[] old_data;
        max = ns;
      }
      else if (ns < lgth)
        lgth = ns;
    }

  public:
    inline type_t &
    operator[](
        size_t i)
    {
      return data[i];
    }

    inline const type_t &
    operator[](
        size_t i) const
    {
      return data[i];
    }

  public:
    inline size_t
    size() const
    {
      return lgth;
    }

    inline bool
    empty() const
    {
      return size() == 0;
    }

    inline size_t
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

    inline size_t
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

    inline size_t
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