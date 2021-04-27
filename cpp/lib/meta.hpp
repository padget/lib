#ifndef __lib_meta_hpp__
#define __lib_meta_hpp__

namespace lib
{
  template <typename first_t, typename second_t>
  constexpr bool same_v = false;

  template <typename first_t>
  constexpr bool same_v<first_t, first_t> = true;

  template <typename first_t, typename second_t>
  concept same_as = same_v<first_t, second_t>;

  template <typename char_t>
  concept charable =
      same_as<char_t, char> or
      same_as<char_t, wchar_t>;

  template <typename type_t>
  concept integer =
      same_as<type_t, short> or same_as<type_t, unsigned short> or
      same_as<type_t, const short> or same_as<type_t, const unsigned short> or
      same_as<type_t, int> or same_as<type_t, unsigned int> or
      same_as<type_t, const int> or same_as<type_t, const unsigned int> or
      same_as<type_t, long> or same_as<type_t, unsigned long> or
      same_as<type_t, const long> or same_as<type_t, const unsigned long> or
      same_as<type_t, long long> or same_as<type_t, unsigned long long> or
      same_as<type_t, const long long> or same_as<type_t, const unsigned long long>;

  namespace __meta
  {
    template <typename type_t>
    struct remove_reference
    {
      using type = type_t;
    };

    template <typename type_t>
    struct remove_reference<type_t &>
    {
      using type = type_t;
    };

    template <typename type_t>
    struct remove_reference<type_t &&>
    {
      using type = type_t;
    };
  }

  template <typename type_t>
  using remove_reference = typename __meta::remove_reference<type_t>::type;

}

#endif