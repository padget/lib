#ifndef __clon_clon_wrapped_hpp__
#define __clon_clon_wrapped_hpp__

#define CLON_VERSION 1.0.0

#include <lib/meta.hpp>
#include <lib/string_view.hpp>
#include <lib/string.hpp>
#include <lib/scanner.hpp>
#include <lib/tree.hpp>
#include <lib/exception.hpp>

namespace lib
{
  enum class clon_type : unsigned char
  {
    none = 0,
    boolean = 1,
    number = 2,
    string = 3,
    list = 4
  };

  template <charable char_t>
  struct clon_value
  {
    clon_type type;
    basic_string_view<char_t> name;
    basic_string_view<char_t> val;
  };

  BASIC_EXCEPTION(clon_parsing_failed_empty_sview)
  BASIC_EXCEPTION(clon_parsing_failed_lbracket_expected)
  BASIC_EXCEPTION(clon_parsing_failed_rbracket_expected)
  BASIC_EXCEPTION(clon_parsing_failed_lower_expected)
  BASIC_EXCEPTION(clon_parsing_failed_boolean_expected)
  BASIC_EXCEPTION(clon_parsing_failed_quote_expected)
  BASIC_EXCEPTION(clon_parsing_failed_digit_expected)

  namespace __clon
  {
    template <typename char_t>
    using clon_storage = tree<clon_value<char_t>>;

    template <charable char_t>
    class clon_scanner : public basic_scanner<char_t>
    {
      using base = basic_scanner<char_t>;

    public:
      explicit clon_scanner(basic_string_view<char_t> s) : basic_scanner<char_t>(s) {}

      inline void ignore_blanks()
      {
        while (this->in(' ', '\n', '\t', '\r'))
          this->advance();
        this->ignore();
      }

      inline basic_string_view<char_t> scan_name()
      {
        if (not this->between('a', 'z'))
          throw clon_parsing_failed_lower_expected();

        while (this->between('a', 'z'))
          this->advance();

        return this->extract();
      }

      inline basic_string_view<char_t> scan_boolean()
      {
        if (this->starts_with("true"))
          this->advance(4);
        else if (this->starts_with("false"))
          this->advance(5);
        else
          throw clon_parsing_failed_boolean_expected();

        return this->extract();
      }

      inline basic_string_view<char_t> scan_string()
      {
        if (not this->is('"'))
          throw clon_parsing_failed_quote_expected();

        this->advance();
        this->ignore();

        while (not this->is('"') and not this->finished())
          this->advance();

        if (not this->is('"'))
          throw clon_parsing_failed_quote_expected();

        auto str = this->extract();
        this->advance();
        this->ignore();
        return str;
      }

      inline basic_string_view<char_t> scan_number()
      {
        if (this->between('0', '9'))
          throw clon_parsing_failed_digit_expected();

        while (this->between('0', '9'))
          this->advance();

        return this->extract();
      }
    };

    enum class push_type
    {
      root,
      next_of,
      child_of
    };

    template <charable char_t>
    void parse_node(
        clon_scanner<char_t> &scan,
        clon_storage<char_t> &nodes,
        push_type ntype,
        std::size_t index);

    template <charable char_t>
    inline void parse_list(
        std::size_t parent,
        clon_scanner<char_t> &scan,
        clon_storage<char_t> &nodes)
    {
      std::size_t nb_childs = 0;

      while (scan.is('('))
      {
        auto index = nb_childs == 0 ? parent : nodes.size() - 1;
        auto type = nb_childs == 0 ? push_type::child_of : push_type::next_of;
        parse_node(scan, nodes, index, type);
        scan.ignore_blanks();
        ++nb_childs;
      }
    }

    template <charable char_t>
    inline void parse_node(
        clon_scanner<char_t> &scan,
        clon_storage<char_t> &nodes,
        push_type ptype,
        std::size_t index = no_root)
    {
      scan.ignore_blanks();

      if (scan.is('('))
      {
        scan.advance();
        scan.ignore();
      }
      else
        throw clon_parsing_failed_lbracket_expected();

      auto name = scan.scan_name();
      scan.ignore_blanks();

      basic_string_view<char_t> scanr;

      clon_type ctype = clon_type::none;

      if (scan.in('f', 't'))
      {
        scanr = scan.scan_boolean();
        ctype = clon_type::boolean;
      }
      else if (scan.between('0', '9'))
      {
        scanr = scan.scan_number();
        ctype = clon_type::number;
      }
      else if (scan.is('"'))
      {
        scanr = scan.scan_string();
        ctype = clon_type::string;
      }
      else if (scan.is('('))
        ctype = clon_type::list;

      clon_value<char_t> cval{ctype, name, scanr};

      switch (ptype)
      {
      case push_type::root:
        nodes.push_root(cval);
        break;
      case push_type::child_of:
        nodes.push_child(cval, index);
        break;
      case push_type::next_of:
        nodes.push_next(cval, index);
        break;
      }

      if (ctype == clon_type::list)
        parse_list(nodes.size() - 1, scan, nodes);

      scan.ignore_blanks();

      if (scan.is(')'))
      {
        scan.advance();
        scan.ignore();
      }
      else
        throw clon_parsing_failed_rbracket_expected();
    }

    template <charable char_t>
    clon_storage<char_t> inline parse_clon(basic_string_view<char_t> s)
    {
      if (s.empty())
        throw clon_parsing_failed_empty_sview();

      clon_scanner<char_t> scan(s);
      clon_storage<char_t> nodes(s.count('('));
      parse_node(scan, nodes, push_type::root);
      return nodes;
    }
  }

  template <charable char_t>
  class basic_clon_view;

  template <charable char_t>
  class basic_clon
  {
    basic_string<char_t> buff;
    __clon::clon_storage<char_t> nodes;

  public:
    explicit basic_clon(basic_string_view<char_t> s)
        : buff(s.begin(), s.end()),
          nodes(__clon::parse_clon(s)) {}

    inline std::size_t buffsize() const { return buff.size(); }
    inline std::size_t size() const { return nodes.size(); }

    inline basic_clon_view<char_t> view() const
    {
      return (*this, 0);
    }

    inline clon_type type(std::size_t index) const
    {
      return nodes[index].value.type;
    }

    inline basic_string_view<char_t> name(std::size_t index) const
    {
      return nodes[index].value.name;
    }

    inline basic_string_view<char_t> value(std::size_t index) const
    {
      return nodes[index].value.val;
    }
  };

  using clon = basic_clon<char>;
  using wclon = basic_clon<wchar_t>;

  template <charable char_t>
  class basic_clon_view
  {
    const basic_clon<char_t> &c;
    std::size_t index;

  public:
    explicit basic_clon_view(
        const basic_clon<char_t> &_c,
        std::size_t _index)
        : c(_c), index(_index) {}

  public:
    inline clon_type type() const { return c.type(index); }
    inline basic_string_view<char_t> name() const { return c.name(index); }
    inline basic_string_view<char_t> value() const { return c.value(index); }
  };

  using clon_view = basic_clon_view<char>;
  using wclon_view = basic_clon_view<wchar_t>;

  template <charable char_t>
  std::size_t length_of(const basic_clon_view<char_t> &c) { return c.buffsize(); }

  template <charable char_t>
  inline void format_of(
      formatter_context<char_t> &ctx,
      const basic_clon_view<char_t> &view)
  {
    switch (view.type())
    {
    case clon_type::boolean:
    case clon_type::number:
      format_into(ctx, "({} {})", view.name(), view.value());
      break;
    case clon_type::no_string:
    case clon_type::string:
      format_into(ctx, "({} \"{}\")", view.name(), view.value());
      break;
    case clon_type::list:
      format_into(ctx, "({} ", view.name());
      for (auto &&child : ) // FIXME childs of a view
        format_of(ctx, child);
      format_into(ctx, ")");
      break;
    case clon_type::none:
      break;
    }
  }
}

#endif