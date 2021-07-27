#ifndef __lib_algorithm_hpp__
#define __lib_algorithm_hpp__

#include <lib/utility.hpp>

namespace lib
{
  template <typename iterator>
  constexpr iterator find_if(
      iterator b,
      iterator e,
      auto &&pred)
  {
    for (; b != e and not pred(*b); ++b)
      ;

    return b;
  }

  template <
      typename iterator,
      typename type_t>
  constexpr iterator find(
      iterator b,
      iterator e,
      type_t &&t)
  {
    return find_if(
        b, e,
        [&t](const auto &o)
        { return o == t; });
  }

  template <typename iterator>
  constexpr iterator find_if_not(
      iterator b,
      iterator e,
      auto &&pred)
  {
    return find_if(
        b, e,
        [&pred](const auto &item)
        { return not pred(item); });
  }

  template <
      typename iterator>
  constexpr iterator
  find_nth_if(
      unsigned nth,
      iterator b,
      iterator e,
      auto &&pred)
  {
    for (; b != e and nth > 0; --nth)
      b = lib::find_if(b, e, pred);

    return b;
  }

  template <typename iterator>
  void reverse(
      iterator b,
      iterator e)
  {
    for (; (b != e) and (b != --e); b++)
    {
      auto tmp = lib::move(*b);
      *b = lib::move(*e);
      *e = lib::move(tmp);
    }
  }

  template <typename iterator>
  void for_each(
      iterator b,
      iterator e,
      auto &&act)
  {
    for (; b != e; ++b)
      act(*b);
  }

  template <
      typename iterator1,
      typename iterator2>
  constexpr void copy(
      iterator1 b,
      iterator1 e,
      iterator2 bc)
  {
    for (; b != e; ++b, ++bc)
      *bc = *b;
  }

  template <
      typename iterator1,
      typename iterator2>
  constexpr void move(
      iterator1 b,
      iterator1 e,
      iterator2 bc)
  {
    for (; b != e; ++b, ++bc)
      *bc = move(*b);
  }

  template <typename iterator>
  constexpr unsigned count_if(
      iterator b,
      iterator e,
      auto &&pred)
  {
    unsigned cnt = 0;
    for (; b != e; ++b)
      if (pred(*b))
        ++cnt;
    return cnt;
  }

  template <
      typename iterator,
      typename type_t>
  constexpr unsigned count(
      iterator b,
      iterator e,
      type_t &&o)
  {
    return count_if(
        b, e,
        [&o](const type_t &t)
        { return t == o; });
  }

  template <typename iterator>
  constexpr bool all_of(
      iterator b,
      iterator e,
      auto &&pred)
  {
    return find_if_not(b, e, pred) == e;
  }

  template <typename iterator>
  constexpr bool none_of(
      iterator b,
      iterator e,
      auto &&pred)
  {
    return find_if(b, e, pred) == e;
  }

  template <typename iterator>
  constexpr bool any_of(
      iterator b,
      iterator e,
      auto &&pred)
  {
    return find_if(b, e, pred) != e;
  }

  template <
      typename iterator,
      typename value>
  constexpr void replace_if(
      iterator b,
      iterator e,
      auto &&pred,
      const value &v)
  {
    for (; b != e; ++b)
      if (pred(v))
        *b = v;
  }

  template <
      typename iterator1,
      typename iterator2>
  constexpr bool starts_with(
      iterator1 b,
      iterator1 e,
      iterator2 bo,
      iterator2 eo)
  {
    if (e - b < eo - bo)
      return false;

    for (; bo != eo and *b == *bo; (++b, ++bo))
      ;

    return bo == eo;
  }

  template <
      typename iterator1,
      typename iterator2>
  constexpr bool equals(
      iterator1 b,
      iterator1 e,
      iterator2 b2,
      iterator2 e2)
  {
    return e - b == e2 - b2 and
           starts_with(b, e, b2, e2);
  }

}

#endif