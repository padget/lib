#ifndef __lib_evector_hpp__
#define __lib_evector_hpp__

#include <lib/vector.hpp>
#include <lib/utility.hpp>

namespace lib
{
  template <typename type_t>
  class evector;

  template <typename type_t>
  struct evector_iterator
  {
    evector<type_t> *ev;
    mutable size_t item;

    inline type_t &operator*() const
    {
      return (*ev)[item];
    }

    inline const evector_iterator &operator++() const
    {
      if (ev != nullptr)
        item += 1;

      return *this;
    }

    inline bool operator==(
        const evector_iterator &o) const
    {
      return ev == o.ev and
             item == o.item;
    }

    inline bool operator!=(
        const evector_iterator &o) const
    {
      return not operator==(o);
    }
  };

  template <typename type_t>
  struct evector_citerator
  {
    const evector<type_t> *ev;
    mutable size_t item;

    inline const type_t &operator*() const
    {
      return (*ev)[item];
    }

    inline const evector_citerator &operator++() const
    {
      if (ev != nullptr)
        item += 1;

      return *this;
    }

    inline bool operator==(
        const evector_citerator &o) const
    {
      return ev == o.ev and
             item == o.item;
    }

    inline bool operator!=(
        const evector_citerator &o) const
    {
      return not operator==(o);
    }
  };

  template <typename type_t>
  struct evector
  {
    size_t lgth = 0;
    size_t wdth = 0;
    vector<vector<type_t> *> index;

  public:
    explicit evector(size_t _wdth = 10)
        : wdth(_wdth) {}

    evector(const evector &o)
        : lgth(o.lgth),
          wdth(o.wdth),
          index(o.index) {}

    evector(evector &&o)
        : lgth(o.lgth),
          wdth(o.wdth),
          index(move(o.index))
    {
      o.lgth = 0;
    }

    inline evector &operator=(const evector &o)
    {
      if (this != &o)
      {
        wdth = o.wdth;

        for (const type_t &ot : o)
          push_back(ot);
      }

      return *this;
    }

    inline evector &operator=(evector &&o)
    {

      if (this != &o)
      {
        wdth = o.wdth;
        lgth = o.lgth;
        index = move(o.index);
        o.lgth = 0;
      }

      return *this;
    }

    ~evector()
    {
      for (auto *v : index)
        delete v;

      lgth = 0;
    }

    inline size_t width() const
    {
      return wdth;
    }

    inline size_t size() const
    {
      return lgth;
    }

    inline type_t &operator[](size_t i)
    {
      return unconst_me(const_me(*this)[i]);
    }

    inline const type_t &operator[](size_t i) const
    {
      return (*index[i / wdth])[i % wdth];
    }

    inline void push_back(const type_t &t)
    {
      if (lgth % wdth == 0)
        index.push_back(new vector<type_t>(wdth));

      (*index.back()).push_back(t);
      lgth += 1;
    }

    inline void push_back(type_t &&t)
    {
      if (lgth % wdth == 0)
        index.push_back(new vector<type_t>(wdth));

      (*index.back()).push_back(move(t));
      lgth += 1;
    }

    inline const evector_citerator<type_t> begin() const
    {
      return {this, 0};
    }

    inline const evector_citerator<type_t> end() const
    {
      return {this, size()};
    }

    inline evector_iterator<type_t> begin()
    {
      return {this, 0};
    }

    inline evector_iterator<type_t> end()
    {
      return {this, size()};
    }
  };
}

#endif