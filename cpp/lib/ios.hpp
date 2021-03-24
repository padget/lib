#ifndef __lib_ios_hpp__
#define __lib_ios_hpp__

#include <cstdio>
#include "string.hpp"
#include "format.hpp"
#include "format-types.hpp"

namespace lib
{
  struct file
  {
    std::FILE *fd = nullptr;

    file(std::FILE *f) : fd(f) {}
    ~file() = default;

    bool has_error() const
    {
      return std::ferror(fd) != 0;
    }

    bool eof() const
    {
      return std::feof(fd) != 0;
    }

    template <typename char_t>
    int putc(const char_t &c)
    {
      if (fd == nullptr)
        return EOF;

      return std::fputc(c, fd);
    }

    template <typename char_t>
    std::size_t write(basic_string_view<char_t> s)
    {
      if (fd == nullptr)
        return 0;

      return std::fwrite(s.data(), sizeof(char_t), s.size(), fd);
    }
  };

  inline file cout(stdout);
  inline file cin(stdin);
  inline file cerr(stderr);
}

namespace lib
{
  inline std::size_t fprint(file &out, lib::string_view s)
  {
    return out.write(s);
  }

  template <typename... args_t>
  inline std::size_t fprintf(file &out, lib::string_view f, const args_t &...args)
  {
    return fprint(out, lib::format(f, args...));
  }

  inline std::size_t fprintln(file &out, lib::string_view s)
  {
    std::size_t &&res = fprint(out, s);
    out.putc('\n');
    return res + 1;
  }

  template <typename... args_t>
  inline std::size_t fprintfln(file &out, lib::string_view f, const args_t &...args)
  {
    return fprintln(out, lib::format(f, args...));
  }

  inline std::size_t print(lib::string_view s)
  {
    return fprint(cout, s);
  }

  template <typename... args_t>
  inline std::size_t printf(lib::string_view f, const args_t &...args)
  {
    return fprintf(cout, f, args...);
  }

  inline std::size_t println(lib::string_view s)
  {
    return fprintln(cout, s);
  }

  template <typename... args_t>
  inline std::size_t printfln(lib::string_view f, const args_t &...args)
  {
    return fprintfln(cout, f, args...);
  }
}

namespace lib
{
  inline std::size_t fprint(file &out, lib::wstring_view s)
  {
    return out.write(s);
  }

  template <typename... args_t>
  inline std::size_t fprintf(file &out, lib::wstring_view f, const args_t &...args)
  {
    return fprint(out, lib::format(f, args...));
  }

  inline std::size_t fprintln(file &out, lib::wstring_view s)
  {
    std::size_t &&res = fprint(out, s);
    out.putc('\n');
    return res + 1;
  }

  template <typename... args_t>
  inline std::size_t fprintfln(file &out, lib::wstring_view f, const args_t &...args)
  {
    return fprintln(out, lib::format(f, args...));
  }

  inline std::size_t print(lib::wstring_view s)
  {
    return fprint(cout, s);
  }

  template <typename... args_t>
  inline std::size_t printf(lib::wstring_view f, const args_t &...args)
  {
    return fprintf(cout, f, args...);
  }

  inline std::size_t println(lib::wstring_view s)
  {
    return fprintln(cout, s);
  }

  template <typename... args_t>
  inline std::size_t printfln(lib::wstring_view f, const args_t &...args)
  {
    return fprintfln(cout, f, args...);
  }
}

namespace lib
{
  struct file_opening_failed
  {
  };

  file fopen(string_view filename, string_view mode)
  {
    file f(std::fopen(filename.data(), mode.data()));

    if (f.fd == nullptr)
      throw file_opening_failed();

    return f;
  }

  struct file_closing_failed
  {
  };

  void fclose(file &f)
  {
    if (std::fclose(f.fd) == EOF)
      throw file_closing_failed();
  }

  struct file_fluhshing_failed
  {
  };

  void fflush(file &f)
  {
    if (std::fflush(f.fd))
      throw file_fluhshing_failed();
  }

}

#endif