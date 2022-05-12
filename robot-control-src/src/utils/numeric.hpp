#ifndef UTILS_NUMERIC_HPP_
#define UTILS_NUMERIC_HPP_

#include "numbers.hpp"
#include <numeric>
#include <iterator>

#if __cplusplus >= 201703L
template<class CONTAINER>
constexpr auto mean(const CONTAINER& values)
{
  return std::reduce(std::cbegin(values), std::cend(values), 0.0) / std::size(values);
}
#endif

template<typename T>
constexpr T radToDeg(const T& rad)
{
  return rad * static_cast<T>(180.f / numbers::pi);
}


#endif /* UTILS_NUMERIC_HPP_ */
