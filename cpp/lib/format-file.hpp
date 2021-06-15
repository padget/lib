#ifndef __lib_format_file_hpp__
#define __lib_format_file_hpp__

#include <lib/file.hpp>
#include <lib/string_view.hpp>
#include <lib/format.hpp>

namespace lib::__fmtfile
{
  template <typename char_t, typename... args_t>
  void format(file &out, basic_string_view<char_t> fmt, const args_t &...args);
}

namespace lib
{
  template <typename... args_t>
  void format(file &out, string_view fmt, const args_t &...args)
  {
    __fmtfile::format(out, fmt, args...);
  }
}

#endif