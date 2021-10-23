#ifndef UTILS_TYPES_HPP_
#define UTILS_TYPES_HPP_

#include <cstdint>

template<unsigned long long V> struct IntegerMinType
{
  using type = typename IntegerMinType<(V & (V - 1)) ? (V & (V - 1)) : (V >> 1)>::type;
};
template<> struct IntegerMinType<(1ull << 0)>
{
  using type = uint8_t;
};
template<> struct IntegerMinType<(1ull << 8)>
{
  using type = uint16_t;
};
template<> struct IntegerMinType<(1ull << 16)>
{
  using type = uint32_t;
};
template<> struct IntegerMinType<(1ull << 32)>
{
  using type = uint64_t;
};

template<unsigned long long V>
using IntegerMinType_t = typename IntegerMinType<V>::type;

#endif /* UTILS_TYPES_HPP_ */
