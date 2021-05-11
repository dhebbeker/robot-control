#ifndef ROBOT_CONTROL_SRC_LIBRARY_EXTENSION_HPP_
#define ROBOT_CONTROL_SRC_LIBRARY_EXTENSION_HPP_

#include <cstddef>

template <class T, std::size_t N>
constexpr std::size_t size(const T (&)[N]) noexcept
{
    return N;
}

#endif /* ROBOT_CONTROL_SRC_LIBRARY_EXTENSION_HPP_ */
