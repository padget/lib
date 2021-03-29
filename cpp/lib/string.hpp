#ifndef __lib_string_hpp__
#define __lib_string_hpp__

#include "vector.hpp"
#include "string_view.hpp"

namespace lib
{
  template <charable char_t>
  class basic_string
  {
    vector<char_t> data;

  public:
    basic_string() = default;
    explicit basic_string(std::size_t n) : data(n) {}
    basic_string(const basic_string &) = default;
    basic_string(basic_string &&) = default;
    ~basic_string() = default;

    template <std::size_t n>
    basic_string(const char_t (&s)[n])
        : data(n - 1)
    {
      for (auto &&c : s)
        if (c != '\0')
          data.push_back(c);
    }

    basic_string &operator=(const basic_string &) = default;
    basic_string &operator=(basic_string &&) = default;

  public:
    bool operator==(basic_string_view<char_t> s) const
    {
      return basic_string_view<char_t>(*this) == s;
    }

    operator basic_string_view<char_t>() const
    {
      return basic_string_view<char_t>(begin(), end());
    }

  public:
    const std::size_t &size() const { return data.size(); }
    char_t &operator[](std::size_t i) { return data[i]; }
    const char_t &operator[](std::size_t i) const { return data[i]; }
    void push_back(char_t c) { data.push_back(c); }
    void emplace_back(char_t c) { data.emplace_back(c); }
    char_t *begin() { return data.begin(); }
    char_t *end() { return data.end(); }
    const char_t *begin() const { return data.begin(); }
    const char_t *end() const { return data.end(); }
  };

  using string = basic_string<char>;
  using wstring = basic_string<wchar_t>;
}

#endif