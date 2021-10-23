#ifndef UTILS_RINGBUFFER_HPP_
#define UTILS_RINGBUFFER_HPP_

#include <cstddef>
#include <type_traits>

template<typename _Tp, std::size_t N>
class circular_buffer
{
public:

  using value_type = std::remove_const_t<_Tp>;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = std::size_t;
  static size_type next_index(const size_type current_index)
  {
    return (current_index + 1) % (N + 1);
  }

  template<typename T>
  class _iterator
  {
  public:
    using DataRef = T(&)[N];
    using _Self = _iterator<T>;
  private:
    DataRef data;
    size_type index; /* [0...N] */
    const size_type next;
  public:
    _iterator(DataRef _data, const size_type _index, const size_type _head) :
        data(_data), index(_index), next(_head)
    {

    }

    friend bool operator!=(const _iterator<T> &lhs, const _iterator<T> &rhs)
    {
      return (lhs.data != rhs.data) || (lhs.index != rhs.index) || (lhs.next != rhs.next);
    }
    T operator*() const
    {
      return data[index % N];
    }
    _Self& operator++()
    {
      index = next_index(index % N);
      return *this;
    }

  };

  using iterator = _iterator<value_type>;
  using const_iterator = _iterator<const value_type>;
  using difference_type = std::ptrdiff_t;
private:
  value_type values[N]
  { };
  size_type next = 0;
  size_type last = 0;
public:
  size_type size() const
  {
    return (next - last + N + 1) % (N + 1);
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
    const auto current = next % N;
    const auto oldLast = last % N;
    values[current] = value;
    if (!empty() && current == oldLast)
    {
      last = next_index(oldLast);
    }
    next = next_index(current);
  }

  const_iterator begin() const
  {
    return const_iterator(values, last, next);
  }

  const_iterator end() const
  {
    return const_iterator(values, next, next);
  }

};

#endif /* UTILS_RINGBUFFER_HPP_ */
