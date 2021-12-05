#ifndef UTILS_CIRCULAR_BUFFER_HPP_
#define UTILS_CIRCULAR_BUFFER_HPP_

#include "types.hpp"
#include <cstddef>
#include <type_traits>
#include <iterator>

template<typename _Tp, std::size_t N_>
class circular_buffer
{
public:
  using value_type = typename std::remove_const<_Tp>::type;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = IntegerMinType_t<N_+1>;
private:
  static constexpr size_type N = N_;
public:
  static size_type next_index(const size_type current_index)
  {
    return static_cast<size_type>((current_index + 1) % (N + 1));
  }

  template<typename T>
  class _iterator
  {
  public:
    using iterator_category = std::input_iterator_tag;
    using difference_type = typename std::make_signed<size_type>::type;
    using value_type = T;
    using reference = const T&;
    using pointer = const T*;
    using DataRef = T(&)[N];
    using _Self = _iterator<T>;
  private:
    DataRef data;
    size_type index; /* [0...N] */
    size_type distance_to_end;
  public:
    _iterator(DataRef _data, const size_type _index, const size_type _distance) :
        data(_data), index(_index), distance_to_end(_distance)
    {

    }

    friend bool operator!=(const _iterator<T> &lhs, const _iterator<T> &rhs)
    {
      return (lhs.data != rhs.data) || (lhs.index != rhs.index) || (lhs.distance_to_end != rhs.distance_to_end);
    }
    T operator*() const
    {
      return data[index % N];
    }
    _Self& operator++()
    {
      index = next_index(index % N);
      --distance_to_end;
      return *this;
    }

  };

  using iterator = _iterator<value_type>;
  using const_iterator = _iterator<const value_type>;
  using difference_type = typename std::iterator_traits<iterator>::difference_type;
private:
  value_type values[N]
  { };
  size_type next = 0;
  size_type last = 0;
public:
  size_type size() const
  {
    if (empty())
    {
      return 0;
    }
    else if (next == last)
    {
      return max_size();
    }
    else
    {
      return static_cast<size_type>((next - last + N + 1) % (N + 1));
    }
  }
  constexpr size_type max_size() const
  {
    return N;
  }
  bool empty() const
  {
    return next == 0;
  }

  /**
   * Returns value at index.
   *
   * The index with the lowest value points to the oldest entry.
   * The index with the highest values points to the newest entry.
   * @param index [0...N]
   * @return copy of value at index
   */
  value_type at(const size_type index) const
  {
    return values[(last + index) % N];
  }

  void push_back(const_reference value)
  {
    const size_type current = next % N;
    const size_type oldLast = last % N;
    values[current] = value;
    if (!empty() && current == oldLast)
    {
      last = next_index(oldLast);
    }
    next = next_index(current);
  }

  const_iterator begin() const
  {
    return const_iterator(values, last, size());
  }

  const_iterator end() const
  {
    return const_iterator(values, next, 0);
  }

  void clear()
  {
    next = 0;
    last = 0;
  }

};

#endif /* UTILS_CIRCULAR_BUFFER_HPP_ */
