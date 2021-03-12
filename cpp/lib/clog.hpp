#ifndef __clog_hpp__
#define __clog_hpp__

//#include <clon/format.hpp>
#include <string_view>
#include <iostream>

namespace log
{
  struct configuration
  {
    std::ostream &out;
    std::ostream &err;
  };

  configuration &config()
  {
    static configuration c{std::cout, std::cerr};
    return c;
  }

  template <typename... args_t>
  bool debug(std::string_view fmt, const args_t &...args)
  {
    config().out << clon::fmt::format(fmt, args...);
  }

  template <typename... args_t>
  bool info(std::string_view fmt, const args_t &...args)
  {
    config().out << clon::fmt::format(fmt, args...);
  }

  template <typename... args_t>
  bool warn(std::string_view fmt, const args_t &...args)
  {
    config().out << clon::fmt::format(fmt, args...);
  }

  template <typename... args_t>
  bool error(std::string_view fmt, const args_t &...args)
  {
    config().err << clon::fmt::format(fmt, args...);
  }

  template <typename... args_t>
  bool fatal(std::string_view fmt, const args_t &...args)
  {
    config().err << clon::fmt::format(fmt, args...);
  }

}

#endif