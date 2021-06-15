#ifndef __lib_algorithm_hpp__
#define __lib_algorithm_hpp__

namespace lib
{
  template <typename iterator>
  iterator find_if(
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
  iterator find(
      iterator b,
      iterator e,
      type_t &&t)
  {
    return find_if(
        b, e,
        [&t](auto &&o)
        { return o == t; });
  }

  template <typename iterator>
  iterator find_if_not(
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
  iterator
  find_nth_if(
      std::size_t nth,
      iterator b,
      iterator e,
      auto &&pred)
  {
    while (b != e and nth > 0)
    {
      b = lib::find_if(b, e, pred);
      --nth;
    }

    return b;
  }

  template <typename iterator>
  void reverse(
      iterator b,
      iterator e)
  {
    while ((b != e) && (b != --e))
    {
      auto tmp = lib::move(*b);
      *b = lib::move(*e);
      *e = lib::move(tmp);
      b++;
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
  void copy(
      iterator1 b,
      iterator1 e,
      iterator2 bc)
  {
    for (; b != e; ++b, ++bc)
      *bc = *b;
  }

  template <typename iterator>
  std::size_t count_if(
      iterator b,
      iterator e,
      auto &&pred)
  {
    std::size_t cnt = 0;
    for (; b != e; ++b)
      if (pred(*b))
        ++cnt;
    return cnt;
  }

  template <
      typename iterator,
      typename type_t>
  std::size_t count(
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
  bool all_of(
      iterator b,
      iterator e,
      auto &&pred)
  {
    return find_if_not(b, e, pred) == e;
  }

  template <typename iterator>
  bool none_of(
      iterator b,
      iterator e,
      auto &&pred)
  {
    return find_if(b, e, pred) == e;
  }

  template <typename iterator>
  bool any_of(
      iterator b,
      iterator e,
      auto &&pred)
  {
    return find_if(b, e, pred) != e;
  }

  template <
      typename iterator,
      typename value>
  void replace_if(
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
  bool starts_with(
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
  bool equals(
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