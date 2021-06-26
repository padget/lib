#ifndef __lib_array_hpp__
#define __lib_array_hpp__



namespace lib
{
  template <typename type_t, unsigned n>
  requires(n > 0) struct array
  {
    type_t _data[n];

    constexpr unsigned size() const { return n; }
    constexpr bool empty() const { return size() == 0; }
    constexpr type_t *data() { return _data; }
    constexpr const type_t *data() const { return _data; }
    constexpr type_t *begin() { return _data; }
    constexpr type_t *end() { return _data + n; }
    constexpr const type_t *begin() const { return _data; }
    constexpr const type_t *end() const { return _data + n; }
    constexpr type_t &front() { return *begin(); }
    constexpr const type_t &front() const { return *begin(); }
    constexpr type_t &back() { return *(end() - 1); }
    constexpr const type_t &back() const { return *(end() - 1); }
    constexpr type_t &operator[](unsigned i) { return _data[i]; }
    constexpr const type_t &operator[](unsigned i) const { return _data[i]; }
  };
}

#endif