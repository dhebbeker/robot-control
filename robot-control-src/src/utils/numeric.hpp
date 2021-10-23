#ifndef UTILS_NUMERIC_HPP_
#define UTILS_NUMERIC_HPP_

#include <numeric>
#include <iterator>

template<class CONTAINER>
constexpr auto mean(const CONTAINER& values)
{
  return std::reduce(std::cbegin(values), std::cend(values), 0.0) / std::size(values);
}


#endif /* UTILS_NUMERIC_HPP_ */
