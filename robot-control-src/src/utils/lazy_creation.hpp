/** \file
 * \see https://codereview.stackexchange.com/q/127353/
 */

#pragma once

#include <utility>
#include <functional>

namespace lazy_creation_details
{
/**
 * Returns a pointer to a new object of type `T` constructed from `args`.
 */
template<typename T, typename ... Args>
T* make_new(Args &&... args)
{
  new_s_pre( T(std::forward<Args>(args)...) );
  T* const p = new T(std::forward<Args>(args)...);
  assert(p != nullptr);
  return p;
}

/**
 * Wrap a value of type `T` and perfect-forward it when accessed.
 */
template<typename T>
class Forward
{
public:
  Forward(T &&value) noexcept :
      value_(std::forward<T>(value))
  {
  }

  operator T()
  {
    return std::forward<T>(value_);
  }

private:
  T value_;
};

} /* end of namespace lazy_creation_details */

/**
 * Generates a creator object which creates a new object when that is called.
 *
 * The object is not created when this function is called.
 * Instead the object is created when the return value of this function is called in turn.
 *
 * @tparam T type of object to be created
 * @tparam Args types of arguments to the constructor
 * @param args arguments to the constructor
 * @returns a callable object which, when called, returns a pointer to a *new* object
 *          of type `T` constructed from `args`.
 */
template<typename T, typename ... Args>
auto createCreatorForNewObject(Args &&... args) -> decltype(
    std::bind(
        lazy_creation_details::make_new<T, Args...>,
        lazy_creation_details::Forward<Args>(std::forward<Args>(args))...))
{
  return std::bind(
                   lazy_creation_details::make_new<T, Args...>,
                   lazy_creation_details::Forward<Args>(std::forward<Args>(args))...);
}
