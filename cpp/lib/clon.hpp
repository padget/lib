#ifndef __clon_clon_wrapped_hpp__
#define __clon_clon_wrapped_hpp__

#define CLON_VERSION 1.0.0

#include <lib/meta.hpp>
#include <lib/array.hpp>
#include <lib/string_view.hpp>
#include <lib/string.hpp>
#include <lib/scanner.hpp>
#include <lib/tree.hpp>
#include <lib/exception.hpp>
#include <lib/algorithm.hpp>

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

  template <character char_t>
  struct clon_value
  {
    clon_type type;
    basic_string_view<char_t> name;
    basic_string_view<char_t> val;
  };

  template <character char_t>
  using clon_node = tree_node<clon_value<char_t>>;

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

    template <character char_t>
    class clon_scanner
        : public basic_scanner<char_t>
    {
    public:
      explicit clon_scanner(
          basic_string_view<char_t> s)
          : basic_scanner<char_t>(s) {}

      inline void
      ignore_blanks()
      {
        while (this->in(' ', '\n', '\t', '\r'))
          this->advance();
        this->ignore();
      }

      inline basic_string_view<char_t>
      scan_name()
      {
        if (not this->between('a', 'z'))
          throw clon_parsing_failed_lower_expected();

        while (this->between('a', 'z'))
          this->advance();

        return this->extract();
      }

      inline basic_string_view<char_t>
      scan_boolean()
      {
        if (this->starts_with("true"))
          this->advance(4);
        else if (this->starts_with("false"))
          this->advance(5);
        else
          throw clon_parsing_failed_boolean_expected();

        return this->extract();
      }

      inline basic_string_view<char_t>
      scan_string()
      {
        if (not this->is('"'))
          throw clon_parsing_failed_quote_expected();

        this->advance();
        this->ignore();

        while (not this->is('"') and
               not this->finished())
          this->advance();

        if (not this->is('"'))
          throw clon_parsing_failed_quote_expected();

        auto str = this->extract();
        this->advance();
        this->ignore();
        return str;
      }

      inline basic_string_view<char_t>
      scan_number()
      {
        if (this->between('0', '9'))
          throw clon_parsing_failed_digit_expected();

        while (this->between('0', '9'))
          this->advance();

        return this->extract();
      }
    };

    template <character char_t>
    void parse_node(
        clon_scanner<char_t> &scan,
        clon_storage<char_t> &nodes,
        bool is_root,
        std::size_t parent_id);

    template <character char_t>
    inline void parse_list(
        std::size_t parent_id,
        clon_scanner<char_t> &scan,
        clon_storage<char_t> &nodes)
    {
      while (scan.is('('))
      {
        parse_node(scan, nodes, false, parent_id);
        scan.ignore_blanks();
      }
    }

    template <character char_t>
    inline void parse_node(
        clon_scanner<char_t> &scan,
        clon_storage<char_t> &nodes,
        bool is_root,
        std::size_t parent_id)
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

      if (is_root)
        nodes.push_root(cval);
      else
        nodes.push_back_child(cval, parent_id);

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

    template <character char_t>
    inline clon_storage<char_t>
    parse_clon(
        basic_string_view<char_t> s)
    {
      if (s.empty())
        throw clon_parsing_failed_empty_sview();

      clon_scanner<char_t> scan(s);
      clon_storage<char_t> nodes(count(s.begin(), s.end(), '('));
      parse_node(scan, nodes, true, no_root);
      return nodes;
    }

    template <character char_t>
    struct search_path_step
    {
      basic_string_view<char_t> name;
      std::size_t min = 0;
      std::size_t max = 0;
    };

    template <std::size_t n, character char_t>
    struct search_path;

    template <std::size_t n, character char_t>
    const clon_node<char_t> *
    search(
        const search_path<n, char_t> &pth)
    {
      const clon_node<char_t> *root = nullptr;

      for (auto &&step : pth.steps)
      {
        if (root == nullptr)
          root = &(*pth.nodes)[0];

        auto cs = root->childs();

        auto found = lib::find_nth_if(
            step.min, cs.begin(), cs.end(),
            [&name = step.name](auto &&c) {
              return lib::equals(
                  c.value.name.begin(),
                  c.value.name.end(),
                  name.begin(), name.end());
            });

        if (found == cs.end())
          return nullptr;
        else
          root = &*found;
      }

      return root;
    }

    template <std::size_t n, character char_t>
    struct search_path
    {
      array<search_path_step<char_t>, n> steps;

      const search_path<n + 1, char_t>
      operator[](
          basic_string_view<char_t> name) const
      {
        search_path<n + 1, char_t> nsp{nodes};
        copy(steps.begin(), steps.end(), nsp.steps.begin());
        nsp.steps[n].name = name;
        return nsp;
      }

      const search_path
      operator()(std::size_t num) const
      {
        search_path tmp(*this);
        tmp.steps[n - 1].min = num;
        tmp.steps[n - 1].max = num;
        return tmp;
      }
    };

    template <std::size_t n, character char_t>
    struct search_path
    {
      array<search_path_step<char_t>, n> steps;

      const search_path<n + 1, char_t>
      operator[](
          basic_string_view<char_t> name) const
      {
        search_path<n + 1, char_t> nsp{nodes};
        copy(steps.begin(), steps.end(), nsp.steps.begin());
        nsp.steps[n].name = name;
        return nsp;
      }

      const search_path
      operator()(std::size_t num) const
      {
        search_path tmp(*this);
        tmp.steps[n - 1].min = num;
        tmp.steps[n - 1].max = num;
        return tmp;
      }
    };
  }

  template <character char_t>
  class basic_clon
  {
    basic_string<char_t> buff;
    __clon::clon_storage<char_t> nodes;

  public:
    explicit basic_clon(
        basic_string_view<char_t> s)
        : buff(s.begin(), s.end()),
          nodes(__clon::parse_clon(s)) {}

    inline std::size_t
    buffsize() const
    {
      return buff.size();
    }

    inline std::size_t
    size() const
    {
      return nodes.size();
    }

    inline clon_type
    type(std::size_t index) const
    {
      return nodes[index].value.type;
    }

    template <clon_type ctype>
    inline bool
    is_(std::size_t id) const
    {
      return type(id) == ctype;
    }

    inline basic_string_view<char_t>
    name(std::size_t index) const
    {
      return nodes[index].value.name;
    }

    inline basic_string_view<char_t>
    value(std::size_t index) const
    {
      return nodes[index].value.val;
    }

    inline const clon_node<char_t> &
    root() const
    {
      return nodes[0];
    }

    inline clon_node<char_t> &
    root()
    {
      return nodes[0];
    }

    const __clon::search_path<1, char_t>
    operator[](basic_string_view<char_t> name) const
    {
      return __clon::search_path<1, char_t>{&nodes, {__clon::search_path_step<char_t>{name}}};
    }
  };



  struct path_builder
  {
    template <character char_t>
    __clon::search_path<1, char_t>
    operator[](basic_string_view<char_t> name)
    {
      return __clon::search_path<1, char_t>{
          &nodes, {__clon::search_path_step<char_t>{name}}};
    }
  };

  inline path_builder pth;

  using clon = basic_clon<char>;
  using wclon = basic_clon<wchar_t>;

  template <character char_t>
  std::size_t length_of(
      const basic_clon<char_t> &c)
  {
    return c.buffsize();
  }

  template <character char_t>
  inline void format_of(
      formatter_context<char_t> &ctx,
      const basic_clon<char_t> &c)
  {
    format_of(ctx, c.root());
  }

  template <character char_t>
  inline void format_of(
      formatter_context<char_t> &ctx,
      const clon_node<char_t> &nc)
  {
    auto &&name = nc.value.name;
    auto &&val = nc.value.val;
    auto &&type = nc.value.type;

    switch (type)
    {
    case clon_type::boolean:
    case clon_type::number:
      format_into(ctx, "(# #)", name, val);
      break;
    case clon_type::string:
      format_into(ctx, "(# \"#\")", name, val);
      break;
    case clon_type::list:
      format_into(ctx, "(# ", name);
      for (const auto &c : nc.childs())
        format_of(ctx, c);
      format_into(ctx, ")");
      break;
    case clon_type::none:
      break;
    }
  }
}

#endif