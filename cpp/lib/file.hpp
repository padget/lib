#ifndef __lib_file_hpp__
#define __lib_file_hpp__

#include <lib/exception.hpp>
#include <lib/meta.hpp>
#include <cstdio>
#include <lib/span.hpp>
#include <lib/string_view.hpp>

namespace lib
{
  BASIC_EXCEPTION(ios_exception)
  INHERITED_EXCEPTION(file_opening_failed, ios_exception)
  INHERITED_EXCEPTION(null_file_descriptor, ios_exception)
  INHERITED_EXCEPTION(opened_file_flushing_failed, ios_exception)
  INHERITED_EXCEPTION(opened_file_closing_failed, ios_exception)
  INHERITED_EXCEPTION(opened_file_writing_failed, ios_exception)

  struct file
  {
    std::FILE *fd = nullptr;

    file(std::FILE *f) : fd(f) {}

    inline bool has_error() const
    {
      return std::ferror(fd) != 0;
    }

    inline bool eof() const
    {
      return std::feof(fd) != 0;
    }

    template <character char_t>
    inline void putc(const char_t &c)
    {
      if (fd == nullptr)
        throw null_file_descriptor();

      if (std::fputc(c, fd) != c)
        throw opened_file_writing_failed();
    }

    template <typename type_t, typename limit_t>
    inline void write(span<type_t, limit_t> s)
    {
      if (fd == nullptr)
        throw null_file_descriptor();

      std::size_t res = std::fwrite(
          s.data(), sizeof(type_t), s.size(), fd);

      if (res != s.size() and s.size() != 0)
        throw opened_file_writing_failed();
    }
  };

  inline file fopen(string_view filename, string_view mode)
  {
    file f(std::fopen(filename.data(), mode.data()));

    if (f.fd == nullptr)
      throw file_opening_failed();

    return f;
  }

  inline void fclose(file &f)
  {
    if (f.fd == nullptr)
      throw null_file_descriptor();

    if (std::fclose(f.fd) == EOF)
      throw opened_file_closing_failed();
  }

  inline void fflush(file &f)
  {
    if (std::fflush(f.fd))
      throw opened_file_flushing_failed();
  }

  inline file cout(stdout);
  inline file cin(stdin);
  inline file cerr(stderr);
}

#endif