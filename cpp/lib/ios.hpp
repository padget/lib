#ifndef __lib_ios_hpp__
#define __lib_ios_hpp__

#include <cstdio>
#include "string.hpp"
#include "format.hpp"
#include "format-types.hpp"

namespace lib
{
  inline std::size_t fprint(FILE *out, lib::basic_string_view<char> s)
  {
    for (const char &c : s)
      std::fputc(c, out);

    return s.size();
  }

  template <typename... args_t>
  inline std::size_t fprintf(FILE *out, lib::basic_string_view<char> f, const args_t &...args)
  {
    return fprint(out, lib::format(f, args...));
  }

  inline std::size_t fprintln(FILE *out, lib::basic_string_view<char> s)
  {
    std::size_t &&res = fprint(out, s);
    std::fputc('\n', out);
    return res + 1;
  }

  template <typename... args_t>
  inline std::size_t fprintfln(FILE *out, lib::basic_string_view<char> f, const args_t &...args)
  {
    return fprintln(out, lib::format(f, args...));
  }

  inline std::size_t print(lib::basic_string_view<char> s)
  {
    return fprint(stdout, s);
  }

  template <typename... args_t>
  inline std::size_t printf(lib::basic_string_view<char> f, const args_t &...args)
  {
    return fprintf(stdout, f, args...);
  }

  inline std::size_t println(lib::basic_string_view<char> s)
  {
    return fprintln(stdout, s);
  }

  template <typename... args_t>
  inline std::size_t printfln(lib::basic_string_view<char> f, const args_t &...args)
  {
    return fprintfln(stdout, f, args...);
  }
}

namespace lib
{
  inline std::size_t fprint(FILE *out, lib::basic_string_view<wchar_t> s)
  {
    for (const wchar_t &c : s)
      std::fputc(c, out);

    return s.size();
  }

  template <typename... args_t>
  inline std::size_t fprintf(FILE *out, lib::basic_string_view<wchar_t> f, const args_t &...args)
  {
    return fprint(out, lib::format(f, args...));
  }

  inline std::size_t fprintln(FILE *out, lib::basic_string_view<wchar_t> s)
  {
    std::size_t &&res = fprint(out, s);
    std::fputc('\n', out);
    return res + 1;
  }

  template <typename... args_t>
  inline std::size_t fprintfln(FILE *out, lib::basic_string_view<wchar_t> f, const args_t &...args)
  {
    return fprintln(out, lib::format(f, args...));
  }

  inline std::size_t print(lib::basic_string_view<wchar_t> s)
  {
    return fprint(stdout, s);
  }

  template <typename... args_t>
  inline std::size_t printf(lib::basic_string_view<wchar_t> f, const args_t &...args)
  {
    return fprintf(stdout, f, args...);
  }

  inline std::size_t println(lib::basic_string_view<wchar_t> s)
  {
    return fprintln(stdout, s);
  }

  template <typename... args_t>
  inline std::size_t printfln(lib::basic_string_view<wchar_t> f, const args_t &...args)
  {
    return fprintfln(stdout, f, args...);
  }
}

#endif