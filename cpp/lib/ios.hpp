#ifndef __lib_ios_hpp__
#define __lib_ios_hpp__

#include <lib/file.hpp>
#include <lib/string_view.hpp>
#include <lib/format.hpp>

namespace lib
{
  inline void fprint(file &out, string_view s)
  {
    out.write(s);
  }

  inline void fprintln(file &out, string_view s)
  {
    fprint(out, s);
    out.push_back('\n');
  }

  template <typename... args_t>
  inline void fprintf(file &out, string_view f, const args_t &...args)
  {
    fformat(out, f, args...);
  }

  template <typename... args_t>
  inline void fprintfln(file &out, string_view f, const args_t &...args)
  {
    fprintf(out, f, args...);
    fprintln(out, "");
  }

  inline void print(string_view s)
  {
    fprint(cout, s);
  }

  template <typename... args_t>
  inline void printf(string_view f, const args_t &...args)
  {
    fprintf(cout, f, args...);
  }

  inline void println(string_view s)
  {
    fprintln(cout, s);
  }

  template <typename... args_t>
  inline void printfln(string_view f, const args_t &...args)
  {
    fprintfln(cout, f, args...);
  }
}

namespace lib
{
  inline void fprint(file &out, wstring_view s)
  {
    out.write(s);
  }

  inline void fprintln(file &out, wstring_view s)
  {
    fprint(out, s);
    out.push_back('\n');
  }

  template <typename... args_t>
  inline void fprintf(file &out, wstring_view f, const args_t &...args)
  {
    fformat(out, f, args...);
  }

  template <typename... args_t>
  inline void fprintfln(file &out, wstring_view f, const args_t &...args)
  {
    fprintf(out, f, args...);
    fprintln(out, "\n");
  }

  inline void print(wstring_view s)
  {
    fprint(cout, s);
  }

  template <typename... args_t>
  inline void printf(wstring_view f, const args_t &...args)
  {
    fprintf(cout, f, args...);
  }

  inline void println(wstring_view s)
  {
    fprintln(cout, s);
  }

  template <typename... args_t>
  inline void printfln(wstring_view f, const args_t &...args)
  {
    fprintfln(cout, f, args...);
  }
}

#endif