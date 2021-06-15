#ifndef __md5_md5_hpp__
#define __md5_md5_hpp__

#include <array>
#include <iterator>
#include <cstdint>
#include <string_view>
#include <algorithm>

namespace md5
{
  constexpr std::array<std::uint32_t, 64> k_array_ =
      {
          0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
          0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
          0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
          0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
          0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
          0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
          0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
          0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
          0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
          0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
          0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
          0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
          0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
          0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
          0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
          0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

  constexpr std::array<std::uint32_t, 64> s_array_ =
      {
          7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
          5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
          4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
          6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

  const std::string_view hex_chars = "0123456789abcdef";

  struct md5
  {
    std::uint32_t a0_ = 0x67452301;
    std::uint32_t b0_ = 0xefcdab89;
    std::uint32_t c0_ = 0x98badcfe;
    std::uint32_t d0_ = 0x10325476;

    std::array<std::uint32_t, 16> m_array_;
    std::array<std::uint32_t, 16>::iterator m_array_first_;
  };

  void reset_m_array(md5 &m)
  {
    m.m_array_first_ = m.m_array_.begin();
  }

  template <class InputIterator>
  static std::uint8_t input_u8(const InputIterator &it)
  {
    return *it;
  }

  template <class InputIterator>
  void bytes_to_m_array(md5 &m, InputIterator &first,
                        std::array<std::uint32_t, 16>::iterator m_array_last)
  {
    for (; m.m_array_first_ != m_array_last; ++m.m_array_first_)
    {
      *m.m_array_first_ = input_u8(first++);
      *m.m_array_first_ |= input_u8(first++) << 8;
      *m.m_array_first_ |= input_u8(first++) << 16;
      *m.m_array_first_ |= input_u8(first++) << 24;
    }
  }

  template <class InputIterator>
  void true_bit_to_m_array(md5 &m, InputIterator &first, std::ptrdiff_t chunk_length)
  {
    switch (chunk_length % 4)
    {
    case 0:
      *m.m_array_first_ = 0x00000080;
      break;
    case 1:
      *m.m_array_first_ = input_u8(first++);
      *m.m_array_first_ |= 0x00008000;
      break;
    case 2:
      *m.m_array_first_ = input_u8(first++);
      *m.m_array_first_ |= input_u8(first++) << 8;
      *m.m_array_first_ |= 0x00800000;
      break;
    case 3:
      *m.m_array_first_ = input_u8(first++);
      *m.m_array_first_ |= input_u8(first++) << 8;
      *m.m_array_first_ |= input_u8(first++) << 16;
      *m.m_array_first_ |= 0x80000000;
      break;
    }

    ++m.m_array_first_;
  }

  void zeros_to_m_array(md5 &m, std::array<std::uint32_t, 16>::iterator m_array_last)
  {
    std::fill(m.m_array_first_, m_array_last, 0);
  }

  void original_length_bits_to_m_array(md5 &m, std::uint64_t original_length_bits)
  {
    *m.m_array_first_++ = original_length_bits;
    *m.m_array_first_++ = original_length_bits >> 32;
  }

  std::uint32_t left_rotate(std::uint32_t x, std::uint32_t c)
  {
    return (x << c) | (x >> (32 - c));
  }

  template <class OutputIterator>
  void uint32_to_byte(std::uint32_t n, OutputIterator &first)
  {
    *first++ = n & 0xff;
    *first++ = (n >> 8) & 0xff;
    *first++ = (n >> 16) & 0xff;
    *first++ = (n >> 24) & 0xff;
  }

  void hash_chunk(md5 &m)
  {
    std::uint32_t A = m.a0_;
    std::uint32_t B = m.b0_;
    std::uint32_t C = m.c0_;
    std::uint32_t D = m.d0_;

    std::uint32_t F;
    unsigned int g;

    for (unsigned int i = 0; i < 64; ++i)
    {
      if (i < 16)
      {
        F = (B & C) | ((~B) & D);
        g = i;
      }
      else if (i < 32)
      {
        F = (D & B) | ((~D) & C);
        g = (5 * i + 1) & 0xf;
      }
      else if (i < 48)
      {
        F = B ^ C ^ D;
        g = (3 * i + 5) & 0xf;
      }
      else
      {
        F = C ^ (B | (~D));
        g = (7 * i) & 0xf;
      }

      std::uint32_t D_temp = D;
      D = C;
      C = B;
      B += left_rotate(A + F + k_array_[i] + m.m_array_[g], s_array_[i]);
      A = D_temp;
    }

    m.a0_ += A;
    m.b0_ += B;
    m.c0_ += C;
    m.d0_ += D;
  }

  template <class InputIterator>
  void update(InputIterator first, InputIterator last)
  {

    std::uint64_t original_length_bits = std::distance(first, last) * 8;
    std::ptrdiff_t chunk_length;

    while ((chunk_length = std::distance(first, last)) >= 64)
    {
      reset_m_array();
      bytes_to_m_array(first, m_array_.end());
      hash_chunk();
    }

    reset_m_array();
    bytes_to_m_array(first, m_array_.begin() + chunk_length / 4);
    true_bit_to_m_array(first, chunk_length);

    if (chunk_length >= 56)
    {
      zeros_to_m_array(m_array_.end());
      hash_chunk();

      reset_m_array();
      zeros_to_m_array(m_array_.end() - 2);
      original_length_bits_to_m_array(original_length_bits);
      hash_chunk();
    }
    else
    {
      zeros_to_m_array(m_array_.end() - 2);
      original_length_bits_to_m_array(original_length_bits);
      hash_chunk();
    }
  }

  template <class Container>
  void digest(Container &container)
  {
    container.resize(16);
    auto it = container.begin();

    uint32_to_byte(a0_, it);
    uint32_to_byte(b0_, it);
    uint32_to_byte(c0_, it);
    uint32_to_byte(d0_, it);
  }

  template <class Container>
  void hex_digest(Container &container)
  {
    container.resize(32);
    auto it = container.begin();

    uint32_to_hex(a0_, it);
    uint32_to_hex(b0_, it);
    uint32_to_hex(c0_, it);
    uint32_to_hex(d0_, it);
  }

  template <class OutputIterator>
  void uint32_to_hex(std::uint32_t n, OutputIterator &out)
  {
    for (auto i = 0u; i < 32; i += 4)
      *out++ = hex_chars[(n >> (i ^ 4)) & 0xf];
  }
}

int main()
{
  std::string data = "Hello World";
  std::string data_hex_digest;

  md5 hash;
  hash.update(data.begin(), data.end());
  hash.hex_digest(data_hex_digest);

  std::cout << data_hex_digest << std::endl;
}

#endif
