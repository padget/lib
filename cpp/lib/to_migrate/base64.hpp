#ifndef __base64_base64_hpp__
#define __base64_base64_hpp__

#include <lib/array.hpp>
#include <lib/string_view.hpp>

namespace std
{
  template <typename iterator>
  void advance_if_valid(
      iterator &b, const iterator &e,
      unsigned nb)
  {
    unsigned stp = 0;

    while (b != e and stp < nb)
    {
      lib::advance(b, 1);
      stp++;
    }
  }

  template <typename iterator>
  iterator next_if_valid(iterator b, const iterator &e)
  {
    if (b == e)
      return b;
    else
      return lib::next(b);
  }
}

namespace base64::model
{
  using uchar_t = unsigned char;
  using uint_t = unsigned int;
  using uchar3_t = lib::array<uchar_t, 3>;
  using uchar4_t = lib::array<uchar_t, 4>;
}

namespace base64::constraint
{
  template <typename type_t>
  concept reservable_container =
      requires(
          type_t &&c,
          unsigned size,
          base64::model::uchar_t uc)
  {
    c.reserve(size);
    c.push_back(uc);
  };
}

namespace base64::encoding
{
  constexpr lib::string_view b64table =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz"
      "0123456789+/";

  using uchar_t = base64::model::uchar_t;

  template <typename iterator>
  const uchar_t encode0(
      const iterator &b0,
      const iterator &e)
  {
    auto &&c0 = b0 == e ? '\0' : *b0;
    return b64table[c0 >> 2];
  }

  template <typename iterator>
  const uchar_t encode1(
      const iterator &b0,
      const iterator &b1,
      const iterator &e)
  {
    auto &&c0 = b0 == e ? '\0' : *b0;
    auto &&c1 = b1 == e ? '\0' : *b1;
    return b64table[((c0 & 0b00000011) << 4) +
                    ((c1 >> 4) & 0b00001111)];
  }

  template <typename iterator>
  const uchar_t encode2(
      const iterator &b1,
      const iterator &b2,
      const iterator &e)
  {
    auto &&c1 = b1 == e ? '\0' : *b1;
    auto &&c2 = b2 == e ? '\0' : *b2;
    return b64table[((c1 & 0b00001111) << 2) +
                    ((c2 & 0b11000000) >> 6)];
  }

  template <typename iterator>
  const uchar_t encode3(
      const iterator &b2,
      const iterator &e)
  {
    auto &&c2 = b2 == e ? '\0' : *b2;
    return b64table[c2 & 0b00111111];
  }

  template <typename iterator>
  const base64::model::uchar4_t to_b64(iterator b, iterator e)
  {
    if (b != e)
    {
      auto b0 = b;
      auto b1 = lib::next_if_valid(b0, e);
      auto b2 = lib::next_if_valid(b1, e);

      return {
          (uchar_t)encode0(b0, e),
          (uchar_t)encode1(b0, b1, e),
          (uchar_t)(b1 == e ? '=' : encode2(b1, b2, e)),
          (uchar_t)(b2 == e ? '=' : encode3(b2, e))};
    }
    else
    {
      return {};
    }
  }

  template <
      typename iterator,
      base64::constraint::reservable_container container>
  void encode(iterator b, iterator e, container &encoded)
  {

    while (b != e)
    {
      auto &&b64 = to_b64(b, e);
      encoded.push_back(b64[0]);
      encoded.push_back(b64[1]);
      encoded.push_back(b64[2]);
      encoded.push_back(b64[3]);
      lib::advance_if_valid(b, e, 3);
    }
  }

  template <base64::constraint::reservable_container container>
  container reserve_from_source(lib::string_view bytes)
  {
    container encoded;
    encoded.reserve((bytes.size() + 2) / 3 * 4);
    return encoded;
  }

  template <base64::constraint::reservable_container container>
  container encode(lib::string_view bytes)
  {
    auto &&encoded = reserve_from_source<container>(bytes);
    encode(bytes.begin(), bytes.end(), encoded);
    return encoded;
  }
}

namespace base64::decoding
{
  using uchar_t = base64::model::uchar_t;
  using uint_t = base64::model::uint_t;

  const bool between(
      const uchar_t mn,
      const uchar_t c,
      const uchar_t mx)
  {
    return mn <= c and c <= mx;
  }

  const uint_t shift_upper(const uchar_t &c)
  {
    return c - 'A';
  }

  const uint_t shift_lower(const uchar_t &c)
  {
    return c - 'a' + ('Z' - 'A') + 1;
  }

  const uint_t shift_digit(const uchar_t &c)
  {
    return c - '0' + ('Z' - 'A') + ('z' - 'a') + 2;
  }

  const uint_t rtable(const uchar_t c)
  {
    if (between(u8'A', c, u8'Z'))
      return shift_upper(c);
    else if (between(u8'a', c, u8'z'))
      return shift_lower(c);
    else if (between(u8'0', c, u8'9'))
      return shift_digit(c);
    else if (c == '+')
      return 62;
    else if (c == '/')
      return 63;
    throw "If input is correct, this line should never be reached.";
  }

  template <typename iterator>
  const uchar_t decode0(
      iterator b0, iterator b1, iterator e)
  {
    auto &&c0 = b0 == e ? u8'\0' : *b0;
    auto &&c1 = b1 == e ? u8'\0' : *b1;
    return ((rtable(c0)) << 2) +
           ((rtable(c1) & 0x30) >> 4);
  }

  template <typename iterator>
  const base64::model::uchar_t decode1(
      iterator b1, iterator b2, iterator e)
  {
    auto &&c1 = b1 == e ? '\0' : *b1;
    auto &&c2 = b2 == e ? '\0' : *b2;
    return c2 != '='
               ? (((rtable(c1) & 0x0f) << 4) +
                  ((rtable(c2) & 0x3c) >> 2))
               : '\0';
  }

  template <typename iterator>
  const base64::model::uchar_t decode2(
      iterator b2, iterator b3, iterator e)
  {
    auto &&c2 = b2 == e ? '\0' : *b2;
    auto &&c3 = b3 == e ? '\0' : *b3;
    return c3 != '='
               ? (((rtable(c2) & 0x03) << 6) + rtable(c3))
               : '\0';
  }

  template <typename iterator>
  const base64::model::uchar3_t from_b64(iterator b, iterator e)
  {
    if (b != e)
    {
      auto b0 = b;
      auto b1 = lib::next_if_valid(b0, e);
      auto b2 = lib::next_if_valid(b1, e);
      auto b3 = lib::next_if_valid(b2, e);

      return {
          (uchar_t)decode0(b0, b1, e),
          (uchar_t)decode1(b1, b2, e),
          (uchar_t)decode2(b2, b3, e)};
    }
    else
    {
      return {};
    }
  }

  template <typename C>
  concept reservable_container = base64::constraint::reservable_container<C>;

  template <
      typename iterator,
      reservable_container container>
  void decode(
      iterator b, iterator e,
      container &decoded)
  {
    while (b != e)
    {
      auto &&bts = from_b64(b, e);

      if (bts[0] != '\0')
        decoded.push_back(bts[0]);
      if (bts[1] != '\0')
        decoded.push_back(bts[1]);
      if (bts[2] != '\0')
        decoded.push_back(bts[2]);

      lib::advance_if_valid(b, e, 4);
    }
  }

  template <reservable_container container>
  container reserve_from_source(lib::string_view enc)
  {
    container decoded;
    decoded.reserve(enc.size() / 4 * 3);
    return decoded;
  }

  template <reservable_container container>
  container decode(lib::string_view enc)
  {
    if (enc.empty())
      return {};

    container decoded = reserve_from_source<container>(enc);
    decode(enc.begin(), enc.end(), decoded);
    return decoded;
  }
}

namespace base64
{
  template <typename C>
  concept reservable_container = base64::constraint::reservable_container<C>;

  template <reservable_container container>
  container encode(lib::string_view s)
  {
    return base64::encoding::encode<container>(s);
  }

  template <reservable_container container>
  container decode(lib::string_view s)
  {
    return base64::decoding::decode<container>(s);
  }
}

#endif
