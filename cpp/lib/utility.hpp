#ifndef __lib_utility_hpp__
#define __lib_utility_hpp__

#include <lib/meta.hpp>
#define rcast reinterpret_cast

namespace lib
{
  using size_t = long unsigned;
  using byte_t = char unsigned;

  template <typename type_t>
  constexpr remove_reference<type_t> &&
  move(type_t &&t)
  {
    return static_cast<remove_reference<type_t> &&>(t);
  }

  template <typename type_t>
  const type_t &const_me(type_t &t)
  {
    return t;
  }

  template <typename type_t>
  type_t &unconst_me(const type_t &t)
  {
    return const_cast<type_t &>(t);
  }

  template <typename type_t>
  constexpr void memcpy(
      const type_t *bsrc,
      const type_t *esrc,
      type_t *dest)
  {
    constexpr size_t cpualign = sizeof(size_t);
    constexpr size_t typealign = sizeof(type_t);

    const size_t allbytes = (esrc - bsrc) * typealign;
    size_t alignedbytes = allbytes / cpualign;

    const size_t *bsrcs = rcast<const size_t *>(bsrc);
    size_t *dests = rcast<size_t *>(dest);

    while (alignedbytes--)
      *dests++ = *bsrcs++;

    const byte_t *bsrcb = rcast<const byte_t *>(bsrcs);
    byte_t *destb = rcast<byte_t *>(dests);
    size_t tailbytes = allbytes % cpualign;

    while (tailbytes--)
      *destb++ = *bsrcb++;
  }

  template <
      size_t i,
      typename arg_t,
      typename... args_t>
  constexpr auto &get(
      const arg_t &arg,
      const args_t &...args)
  {
    static_assert(i <= sizeof...(args));

    if constexpr (i == 0)
      return arg;
    else
      return get<i - 1>(args...);
  }
}

#endif