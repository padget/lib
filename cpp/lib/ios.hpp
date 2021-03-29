#ifndef __lib_ios_hpp__
#define __lib_ios_hpp__

#include <cstdio>
#include "string.hpp"
#include "format.hpp"
#include "format-types.hpp"
#include "meta.hpp"

#define BASIC_EXCEPTION(exception) \
  struct exception                 \
  {                                \
  };

#define INHERITED_EXCEPTION(exception, base) \
  struct exception : public base             \
  {                                          \
  };

namespace lib
{
  BASIC_EXCEPTION(ios_exception)

  INHERITED_EXCEPTION(file_opening_failed, ios_exception)
  INHERITED_EXCEPTION(file_closing_failed, ios_exception)
  INHERITED_EXCEPTION(file_fluhshing_failed, ios_exception)
  INHERITED_EXCEPTION(null_file_descriptor, ios_exception)
  INHERITED_EXCEPTION(file_writing_failed, ios_exception)

  struct file
  {
    std::FILE *fd = nullptr;

    file(std::FILE *f) : fd(f) {}

    bool has_error() const
    {
      return std::ferror(fd) != 0;
    }

    bool eof() const
    {
      return std::feof(fd) != 0;
    }

    template <charable char_t>
    void putc(const char_t &c)
    {
      if (fd == nullptr)
        throw null_file_descriptor();

      if (std::fputc(c, fd) != c)
        throw file_writing_failed();
    }

    template <typename type_t, typename limit_t>
    void write(span<type_t, limit_t> s)
    {
      if (fd == nullptr)
        throw null_file_descriptor();

      std::size_t res = std::fwrite(
          s.data(), sizeof(type_t), s.size(), fd);

      if (res != s.size() and s.size() != 0)
        throw file_writing_failed();
    }
  };

  file fopen(string_view filename, string_view mode)
  {
    file f(std::fopen(filename.data(), mode.data()));

    if (f.fd == nullptr)
      throw file_opening_failed();

    return f;
  }

  void fclose(file &f)
  {
    if (std::fclose(f.fd) == EOF)
      throw file_closing_failed();
  }

  void fflush(file &f)
  {
    if (std::fflush(f.fd))
      throw file_fluhshing_failed();
  }

  inline file cout(stdout);
  inline file cin(stdin);
  inline file cerr(stderr);
}

namespace lib
{
  inline void fprint(file &out, lib::string_view s)
  {
    out.write(s);
  }

  template <typename... args_t>
  inline void fprintf(file &out, lib::string_view f, const args_t &...args)
  {
    fprint(out, lib::format(f, args...));
  }

  inline void fprintln(file &out, lib::string_view s)
  {
    fprint(out, s);
    out.putc('\n');
  }

  template <typename... args_t>
  inline void fprintfln(file &out, lib::string_view f, const args_t &...args)
  {
    fprintln(out, lib::format(f, args...));
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

  template <typename... args_t>
  inline void fprintf(file &out, lib::wstring_view f, const args_t &...args)
  {
    fprint(out, lib::format(f, args...));
  }

  inline void fprintln(file &out, lib::wstring_view s)
  {
    fprint(out, s);
    out.putc('\n');
  }

  template <typename... args_t>
  inline void fprintfln(file &out, lib::wstring_view f, const args_t &...args)
  {
    fprintln(out, lib::format(f, args...));
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