#ifndef __clog_hpp__
#define __clog_hpp__

#include <lib/string_view.hpp>
#include <lib/ios.hpp>

namespace lib
{
  struct configuration
  {
    file &out;
    file &err;
  };

  configuration &config()
  {
    static configuration c{cout, cerr};
    return c;
  }

  template <typename... args_t>
  void debug(
      string_view fmt,
      const args_t &...args)
  {
    config().out.write(format(fmt, args...));
  }

  template <typename... args_t>
  void info(
      string_view fmt,
      const args_t &...args)
  {
    config().out.write(format(fmt, args...));
  }

  template <typename... args_t>
  void warn(
      string_view fmt,
      const args_t &...args)
  {
    config().out.write(format(fmt, args...));
  }

  template <typename... args_t>
  void error(
      string_view fmt,
      const args_t &...args)
  {
    config().err.write(format(fmt, args...));
  }

  template <typename... args_t>
  void fatal(
      string_view fmt,
      const args_t &...args)
  {
    config().err.write(format(fmt, args...));
  }

}

#endif