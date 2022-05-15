#pragma once

namespace utils
{
template<class T>
constexpr const T& max(const T &a, const T &b)
{
  return (a < b) ? b : a;
}
}
