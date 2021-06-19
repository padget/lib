#ifndef __lib_ios_hpp__
#define __lib_ios_hpp__

#include <lib/file.hpp>
#include <lib/string_view.hpp>

namespace lib
{
  inline void fprint(file &out, lib::string_view s)
  {
    out.write(s);
  }

  inline void fprintln(file &out, lib::string_view s)
  {
    fprint(out, s);
    out.push_back('\n');
  }

  template <typename... args_t>
  inline void fprintf(file &out, lib::string_view f, const args_t &...args)
  {
    lib::format(out, f, args...);
  }

  template <typename... args_t>
  inline void fprintfln(file &out, lib::string_view f, const args_t &...args)
  {
    fprintf(out, f, args...);
    fprintln(out, "");
  }

  inline void print(lib::string_view s)
  {
    fprint(cout, s);
  }

  template <typename... args_t>
  inline void printf(lib::string_view f, const args_t &...args)
  {
    fprintf(cout, f, args...);
  }

  inline void println(lib::string_view s)
  {
    fprintln(cout, s);
  }

  template <typename... args_t>
  inline void printfln(lib::string_view f, const args_t &...args)
  {
    fprintfln(cout, f, args...);
  }
}

namespace lib
{
  inline void fprint(file &out, lib::wstring_view s)
  {
    out.write(s);
  }

  inline void fprintln(file &out, lib::wstring_view s)
  {
    fprint(out, s);
    out.push_back('\n');
  }

  template <typename... args_t>
  inline void fprintf(file &out, lib::wstring_view f, const args_t &...args)
  {
    lib::format(out, f, args...);
  }

  template <typename... args_t>
  inline void fprintfln(file &out, lib::wstring_view f, const args_t &...args)
  {
    fprintf(out, f, args...);
    fprintln(out, "");
  }

  inline void print(lib::wstring_view s)
  {
    fprint(cout, s);
  }

  template <typename... args_t>
  inline void printf(lib::wstring_view f, const args_t &...args)
  {
    fprintf(cout, f, args...);
  }

  inline void println(lib::wstring_view s)
  {
    fprintln(cout, s);
  }

  template <typename... args_t>
  inline void printfln(lib::wstring_view f, const args_t &...args)
  {
    fprintfln(cout, f, args...);
  }
}

#endif