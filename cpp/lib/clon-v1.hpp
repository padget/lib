#ifndef __clon_clon_wrapped_hpp__
#define __clon_clon_wrapped_hpp__

#define CLON_VERSION 1.0.0

#include <variant>
#include <limits>

#include "vector.hpp"
#include "array.hpp"
#include "format.hpp"
#include "meta.hpp"
#include "string_view.hpp"
#include "string.hpp"

namespace lib
{
  namespace detail
  {
    enum struct clon_type : unsigned char
    {
      none = 0,
      boolean = 1,
      number = 2,
      string = 3,
      list = 4
    };

    template <charable char_t>
    struct value
    {
      basic_string_view<char_t> val;
      clon_type type;
    };

    using clon_number = long long;

    template <charable char_t>
    using clon_string = basic_string<char_t>;

    using clon_boolean = bool;
    using clon_list = struct clon_list_tag
    {
    };
    using clon_none = void;

    constexpr std::size_t no_next = static_cast<std::size_t>(-1);
    constexpr std::size_t no_child = static_cast<std::size_t>(-1);
    constexpr std::size_t no_root = static_cast<std::size_t>(-1);

    template <charable char_t>
    clon_number to_number(basic_string_view<char_t> v)
    {
      clon_number n = 0;

      for (const char_t &c : v)
        n = n * 10 + (c - '0');

      return n;
    }

    template <charable char_t>
    std::size_t to_integer(basic_string_view<char_t> v)
    {
      std::size_t n = 0;

      for (const char_t &c : v)
        n = n * 10 + (c - '0');

      return n;
    }

    template <charable char_t>
    struct node
    {
      basic_string_view<char_t> name;
      basic_string_view<char_t> valv;
      clon_type type;
      std::size_t next = no_next;
      std::size_t child = no_child;
    };

    template <charable char_t>
    node<char_t> make_node(
        const clon_type &type,
        const basic_string_view<char_t> &name,
        const basic_string_view<char_t> &valv)
    {
      return node<char_t>{name, valv, type};
    }

    template <charable char_t>
    struct root_node
    {
      vector<char_t> buff;
      vector<node<char_t>> nodes;
      vector<clon_string<char_t>> updt;
    };

    template <charable char_t>
    root_node<char_t> make_root(
        const basic_string_view<char_t> &data)
    {
      root_node<char_t> root;
      root.buff = vector<char_t>(data.begin(), data.end());
      root.nodes.reserve(data.count('('));
      return root;
    }

    template <charable char_t>
    struct root_view
    {
      root_node<char_t> *root;
      std::size_t index;

      lib::basic_string<char_t> to_string() const
      {
        return format("{}", *this);
      }

      using view = basic_string_view<char_t>;

      const view &name() const { return root->nodes[index].name; }
      const std::size_t &child() const { return root->nodes[index].child; }
      const std::size_t &next() const { return root->nodes[index].next; }
      const view &valv() const { return root->nodes[index].valv; }
      const clon_type type() const { return root->nodes[index].type; }

      // template <typename type_t>
      // const type_t &as_() const
      // {
      //   if constexpr (same_v<type_t, clon_boolean>)
      //     if (type() == clon_type::no_boolean)
      //       return root->nodes[index].valv == "true";

      //   if constexpr (same_v<type_t, clon_string<char_t>>)
      //     if (type() == clon_type::no_string)
      //       root->nodes[index].val = root->nodes[index].valv;

      //   if constexpr (same_v<type_t, clon_number>)
      //     if (type() == clon_type::no_number)
      //       root->nodes[index].val = to_number(root->nodes[index].valv);

      //   return std::get<type_t>(root->nodes[index].val);
      // }

      template <typename type_t>
      bool is_() const
      {
        if constexpr (same_v<type_t, clon_boolean>)
          return type() == clon_type::boolean;
        if constexpr (same_v<type_t, clon_list>)
          return type() == clon_type::list;
        if constexpr (same_v<type_t, clon_string<char_t>>)
          return type() == clon_type::string;
        if constexpr (same_v<type_t, clon_number>)
          return type() == clon_type::number;
        if constexpr (same_v<type_t, clon_none>)
          return type() == clon_type::none;
      }
    };

    template <charable char_t>
    root_view<char_t> make_rview(root_node<char_t> &r, const std::size_t &index = 0)
    {
      return {&r, index};
    }

    template <charable char_t>
    root_view<char_t> make_rview(const root_view<char_t> &view, const std::size_t &index = 0)
    {
      return make_rview(*view.root, index);
    }

    template <charable char_t>
    struct childs_iterator
    {
      root_view<char_t> view;

      childs_iterator &operator++()
      {
        view.index = view.next();
        return *this;
      }

      childs_iterator operator++(int)
      {
        childs_iterator tmp(*this);
        ++(*this);
        return tmp;
      }

      root_view<char_t> operator*() const { return make_rview(view, view.index); }

      friend bool operator==(
          const childs_iterator &a,
          const childs_iterator &b)
      {
        return a.view.root == b.view.root and
               a.view.index == b.view.index;
      }

      friend bool operator!=(
          const childs_iterator &a,
          const childs_iterator &b)
      {
        return not(a == b);
      }
    };

    template <charable char_t>
    struct childs_list
    {
      root_view<char_t> view;
      childs_iterator<char_t> begin() const { return {view}; }
      childs_iterator<char_t> end() const { return {make_rview(view, no_next)}; }
    };

    template <charable char_t>
    childs_list<char_t> childs(const root_view<char_t> &view)
    {
      return {make_rview(view, view.child())};
    }

    template <charable char_t>
    std::size_t length_of(const root_view<char_t> &view)
    {
      return view.root->buff.size();
    }

    template <charable char_t>
    void format_of(
        formatter_context<char_t> &ctx,
        const root_view<char_t> &view)
    {
      switch (view.type())
      {
      case clon_type::no_boolean:
      case clon_type::no_number:
      case clon_type::boolean:
      case clon_type::number:
        format_into(ctx, "({} {})", view.name(), view.valv());
        break;
      case clon_type::no_string:
      case clon_type::string:
        format_into(ctx, "({} \"{}\")", view.name(), view.valv());
        break;
      case clon_type::list:
        format_into(ctx, "({} ", view.name());
        for (auto &&child : childs(view))
          format_of(ctx, child);
        format_into(ctx, ")");
        break;
      case clon_type::none:
        break;
      }
    }

    enum class ascii_type : int
    {
      eos,
      blank,
      letter_f,
      letter_t,
      lower,
      dquote,
      digit,
      lpar,
      rpar,
      other
    };

    constexpr array<ascii_type, 128> ascii_lookup()
    {
      array<ascii_type, 128> table;

      for (ascii_type &sb : table)
        sb = ascii_type::other;

      for (char l = 'a'; l <= 'z'; ++l)
        table[l] = ascii_type::lower;

      for (char l = '0'; l <= '9'; ++l)
        table[l] = ascii_type::digit;

      table['f'] = ascii_type::letter_f;
      table['t'] = ascii_type::letter_t;
      table['"'] = ascii_type::dquote;
      table['('] = ascii_type::lpar;
      table[')'] = ascii_type::rpar;
      table[' '] = ascii_type::blank;
      table['\n'] = ascii_type::blank;
      table['\r'] = ascii_type::blank;
      table['\t'] = ascii_type::blank;
      table['\0'] = ascii_type::eos;

      return table;
    }



    constexpr array<ascii_type, 128> ascii_to_sb = ascii_build();

    template <charable char_t>
    struct clon_scanner : public basic_scanner<char_t>
    {
      ascii_type symbol() const
      {
        if (index < data.size())
          return data[index] > 127 ? ascii_type::other : ascii_to_sb[data[index]];
        else
          return ascii_type::eos;
      }

      
    };

    template <charable char_t, std::size_t n>
    void handle_error_expecting(const char_t (&s)[n])
    {
      throw format("expected character {}", s);
    }

    template <charable char_t>
    void ignore_blanks(scanner<char_t> &scan)
    {
      while (scan.symbol() == ascii_type::blank)
        scan.advance();

      scan.ignore();
    }

    template <charable char_t>
    basic_string_view<char_t> scan_name(
        scanner<char_t> &scan)
    {
      ignore_blanks(scan);

      ascii_type sb = scan.symbol();

      if (sb != ascii_type::lower and
          sb != ascii_type::letter_f and
          sb != ascii_type::letter_t)
        handle_error_expecting("[a-z]");

      while (sb == ascii_type::lower or
             sb == ascii_type::letter_f or
             sb == ascii_type::letter_t)
      {
        scan.advance();
        sb = scan.symbol();
      }

      return scan.extract();
    }

    template <charable char_t>
    basic_string_view<char_t> scan_boolean(
        scanner<char_t> &scan)
    {
      ignore_blanks(scan);

      if (scan.starts_with("true"))
        scan.advance(4);
      else if (scan.starts_with("false"))
        scan.advance(5);
      else
        handle_error_expecting("'true' or 'false'");

      return scan.extract();
    }

    template <charable char_t>
    basic_string_view<char_t> scan_string(
        scanner<char_t> &scan)
    {
      ignore_blanks(scan);

      if (scan.symbol() == ascii_type::dquote)
      {
        scan.advance();
        scan.ignore();

        while (scan.symbol() != ascii_type::dquote)
          scan.advance();

        if (scan.symbol() == ascii_type::dquote)
          scan.advance();
        else
          handle_error_expecting("'\"'");
      }
      else
        handle_error_expecting("'\"'");

      scan.backward();
      auto &&str = scan.extract();
      scan.advance();
      scan.ignore();
      return str;
    }

    template <charable char_t>
    basic_string_view<char_t> scan_number(
        scanner<char_t> &scan)
    {
      ignore_blanks(scan);

      if (scan.symbol() != ascii_type::digit)
        handle_error_expecting("[0-9]");

      while (scan.symbol() == ascii_type::digit)
        scan.advance();

      return scan.extract();
    }

    template <charable char_t>
    basic_string_view<char_t> scan_list(
        scanner<char_t> &)
    {
      return {};
    }

    template <charable char_t>
    void open_node(scanner<char_t> &scan)
    {
      ignore_blanks(scan);

      if (scan.symbol() == ascii_type::lpar)
      {
        scan.advance();
        scan.ignore();
      }
      else
        handle_error_expecting("'('");
    }

    template <charable char_t>
    void close_node(scanner<char_t> &scan)
    {
      ignore_blanks(scan);

      if (scan.symbol() == ascii_type::rpar)
      {
        scan.advance();
        scan.ignore();
      }
      else
        handle_error_expecting("')'");
    }

    template <charable char_t>
    struct parser_context
    {
      vector<node<char_t>> *nodes;
      scanner<char_t> scan;
    };

    template <charable char_t>
    void parse_node(parser_context<char_t> &ctx);

    template <charable char_t>
    void parse_list(parser_context<char_t> &ctx)
    {
      ignore_blanks(ctx.scan);
      std::size_t index = ctx.nodes->size();
      std::size_t count = 0;

      while (ctx.scan.symbol() == ascii_type::lpar)
      {
        if (count > 0)
          index = ctx.nodes->operator[](index).next = ctx.nodes->size();

        parse_node(ctx);
        ignore_blanks(ctx.scan);
        count++;
      }
    }

    template <charable char_t>
    clon_type predict_clon_type(scanner<char_t> &scan)
    {
      switch (scan.symbol())
      {
      case ascii_type::letter_f:
      case ascii_type::letter_t:
        return clon_type::boolean;
      case ascii_type::dquote:
        return clon_type::string;
      case ascii_type::digit:
        return clon_type::number;
      case ascii_type::lpar:
        return clon_type::list;
      default:
        return clon_type::none;
      }
    }

    template <charable char_t>
    void parse_node(parser_context<char_t> &ctx)
    {
      open_node(ctx.scan);

      basic_string_view<char_t> &&name(scan_name(ctx.scan));
      ignore_blanks(ctx.scan);

      basic_string_view<char_t> scanr;
      clon_type type = predict_clon_type(ctx.scan);

      switch (type)
      {
      case clon_type::boolean:
        scanr = scan_boolean(ctx.scan);
        break;
      case clon_type::string:
        scanr = scan_string(ctx.scan);
        break;
      case clon_type::number:
        scanr = scan_number(ctx.scan);
        break;
      case clon_type::list:
        scanr = scan_list(ctx.scan);
        break;
      default:
        break;
      }

      ctx.nodes->emplace_back(make_node(type, name, scanr));

      if (type == clon_type::list)
      {
        ctx.nodes->back().child = ctx.nodes->size();
        parse_list(ctx);
      }

      close_node(ctx.scan);
    }

    template <charable char_t>
    const root_node<char_t> parse(
        const basic_string_view<char_t> &data)
    {
      root_node<char_t> root = make_root(data);
      parser_context<char_t> ctx{&root.nodes, {data}};
      parse_node(ctx);
      return root;
    }

    constexpr std::size_t path_max = static_cast<std::size_t>(-1);

    template <charable char_t>
    struct path
    {
      basic_string_view<char_t> name;
      std::size_t min = path_max;
      std::size_t max = path_max;
    };

    template <charable char_t>
    bool scan_colon(scanner<char_t> &scan)
    {
      if (scan.data[scan.index] == ':')
      {
        scan.advance();
        scan.ignore();
        return true;
      }

      return false;
    }

    template <charable char_t>
    std::pair<std::size_t, std::size_t> scan_interval(scanner<char_t> &scan)
    {
      if (scan.data[scan.index] == '*')
      {
        scan.ignore();
        return {path_max, path_max};
      }
      else
      {
        std::size_t &&min = to_integer(scan_number(scan));
        return {min, min};
      }
    }

    template <charable char_t>
    path<char_t> parse_path(const basic_string_view<char_t> &view)
    {
      path<char_t> p;
      scanner<char_t> scan{view};
      p.name = scan_name(scan);

      if (scan_colon(scan))
      {
        auto &&[min, max] = scan_interval(scan);
        p.min = min;
        p.max = max;
      }
      else
        p.min = p.max = 0;

      return p;
    }

    template <charable char_t>
    struct splits_iterator
    {
      char_t del;
      basic_string_view<char_t> data;

      splits_iterator &operator++()
      {
        if (data.size() != 0)
        {
          auto found = data.find(del);
          data = basic_string_view<char_t>(
              found == data.end() ? found : found + 1, data.end());
        }

        return *this;
      }

      const basic_string_view<char_t> operator*()
      {
        return basic_string_view<char_t>(
            data.begin(), data.find(del));
      }

      splits_iterator operator++(int)
      {
        splits_iterator tmp(*this);
        ++(*this);
        return tmp;
      }

      friend bool operator==(
          const splits_iterator &a,
          const splits_iterator &b)
      {
        return a.data.data() == b.data.data();
      }

      friend bool operator!=(
          const splits_iterator &a,
          const splits_iterator &b)
      {
        return not(a == b);
      }
    };

    template <charable char_t>
    struct splits
    {
      char_t del;
      basic_string_view<char_t> data;

      splits_iterator<char_t> begin() const
      {
        return splits_iterator<char_t>{del, data};
      }

      splits_iterator<char_t> end() const
      {
        return splits_iterator<char_t>{del, basic_string_view<char_t>(data.end(), data.end())};
      }
    };

    template <charable char_t>
    splits<char_t> split(
        const basic_string_view<char_t> &data,
        const char_t &del)
    {
      return splits<char_t>{del, data};
    }

    template <charable char_t>
    struct paths_iterator
    {
      splits_iterator<char_t> sit;

      paths_iterator &operator++()
      {
        ++sit;
        return *this;
      }

      paths_iterator operator++(int)
      {
        paths_iterator tmp(*this);
        ++(*this);
        return tmp;
      }

      path<char_t> operator*()
      {
        return parse_path(*sit);
      }

      friend bool operator==(
          const paths_iterator &a,
          const paths_iterator &b)
      {
        return a.sit == b.sit;
      }

      friend bool operator!=(
          const paths_iterator &a,
          const paths_iterator &b)
      {
        return not(a == b);
      }
    };

    template <charable char_t>
    struct paths
    {
      splits<char_t> spl;

      paths_iterator<char_t> begin() const { return {spl.begin()}; }
      paths_iterator<char_t> end() const { return {spl.end()}; }
    };

    template <charable char_t>
    paths<char_t> split_paths(const basic_string_view<char_t> &pths)
    {
      return paths<char_t>{split(pths, '.')};
    }

    template <charable char_t>
    root_view<char_t> getone(
        const path<char_t> &pth,
        const root_view<char_t> &view)
    {
      std::size_t cnt = 0;

      if (view.type() == clon_type::list)
        for (root_view<char_t> &&child : childs(view))
          if (child.name() == pth.name)
          {
            if (cnt == pth.min)
              return child;
            else
              ++cnt;
          }

      return make_rview(view, no_root);
    }

    template <charable char_t>
    root_view<char_t> getone(
        const basic_string_view<char_t> &pth,
        const root_view<char_t> &view)
    {
      return getone(parse_path(pth), view);
    }

    template <charable char_t>
    root_view<char_t> get(
        const basic_string_view<char_t> &pths,
        const root_view<char_t> &view)
    {
      bool should_return = false;

      root_view<char_t> vfound = view;

      for (const path<char_t> &pth : split_paths(pths))
      {
        bool found = false;

        if (should_return)
          return make_rview(view, no_root);

        if (vfound.template is_<clon_list>())
        {
          std::size_t cnt = 0;

          for (root_view<char_t> &&child : childs(vfound))
            if (child.name() == pth.name)
            {
              if (cnt == pth.min)
              {
                found = true;
                vfound = child;
                break;
              }
              else
                ++cnt;
            }
        }
        else
          should_return = true;

        if (not found)
          return make_rview(view, no_root);
      }

      return vfound;
    }
  }
}

namespace lib
{
  using clon_type = detail::clon_type;
  using clon_number = detail::clon_number;
  using clon_list = detail::clon_list;
  using clon_boolean = detail::clon_boolean;
  template <charable char_t>
  using clon_string = detail::clon_string<char_t>;

  template <charable char_t>
  class basic_clon_view
  {
    detail::root_view<char_t> view;

  public:
    explicit basic_clon_view(
        const detail::root_view<char_t> &_v)
        : view(_v) {}

  public:
    basic_clon_view<char_t> operator[](
        const basic_string_view<char_t> &pth) const
    {
      return basic_clon_view<char_t>(detail::get(pth, view));
    }

    std::size_t total_length() const
    {
      return view.root->nodes.size();
    }

    clon_type type() const
    {
      switch (view.type())
      {
      case clon_type::boolean:
      case clon_type::no_boolean:
        return clon_type::boolean;
      case clon_type::number:
      case clon_type::no_number:
        return clon_type::number;
      case clon_type::string:
      case clon_type::no_string:
        return clon_type::string;
      case clon_type::list:
        return clon_type::list;
      case clon_type::none:
      default:
        return clon_type::none;
      }
    }

    const basic_string_view<char_t> &name() const
    {
      return view.name();
    }

    template <typename type_t>
    const type_t &as_() const
    {
      return view.template as_<type_t>();
    }

    template <typename type_t>
    const type_t &get_(
        const basic_string_view<char_t> &pth) const
    {
      return (*this)[pth].template as_<type_t>();
    }

    const clon_string<char_t> &string(
        const basic_string_view<char_t> &pth)
    {
      return get_<clon_string<char_t>>(pth);
    }

    const clon_number &number(
        const basic_string_view<char_t> &pth)
    {
      return get_<clon_number>(pth);
    }

    const clon_boolean &boolean(const basic_string_view<char_t> &pth)
    {
      return get_<clon_boolean>(pth);
    }

    const basic_string<char_t> to_string()
    {
      return view.to_string();
    }

    const basic_string_view<char_t> &value()
    {
      return view.valv();
    }

    template <typename type_t>
    void update(const basic_string_view<char_t> &valv)
    {
      if (view.index != detail::no_root)
      {
        view.root->updt.push_back(
            basic_string<char_t>(valv.begin(), valv.end()));

        detail::node<char> &node = view.root->nodes[view.index];

        node.valv = basic_string_view<char_t>(
            view.root->updt.back().begin(),
            view.root->updt.back().end());

        if constexpr (same_v<type_t, clon_boolean>)
          node.val = detail::clon_no_boolean{};

        if constexpr (same_v<type_t, clon_string<char_t>>)
          node.val = detail::clon_no_string{};

        if constexpr (same_v<type_t, clon_number>)
          node.val = detail::clon_no_number{};
      }
    }

    friend std::size_t length_of(
        const basic_clon_view<char_t> &a)
    {
      return detail::length_of(a.view);
    }

    friend void format_of(
        formatter_context<char_t> &ctx,
        const basic_clon_view<char_t> &a)
    {
      detail::format_of(ctx, a.view);
    }
  };

  template <charable char_t>
  class basic_clon
      : public basic_clon_view<char_t>
  {
    detail::root_node<char_t> node;

  public:
    explicit basic_clon(const basic_string_view<char_t> &_v)
        : basic_clon_view<char_t>(detail::make_rview(node)), node(detail::parse(_v)) {}
  };

  using clon = basic_clon<char>;
  using wclon = basic_clon<wchar_t>;
}

#endif