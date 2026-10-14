#ifndef METAPHYSICL_ALGORITHM_H
#define METAPHYSICL_ALGORITHM_H

#include "metaphysicl/metaphysicl_device.h"

#include <algorithm>
#include <type_traits>
#include <utility>

#if __has_include(<version>)
#include <version>
#endif

#if defined(__cpp_lib_execution) && __cpp_lib_execution >= 201603L
#include <execution>
#define METAPHYSICL_HAVE_EXECUTION_POLICIES
#endif

namespace MetaPhysicL::detail {

#ifdef METAPHYSICL_KOKKOS_COMPILATION

template <typename T, typename U>
METAPHYSICL_INLINE constexpr bool less(const T & lhs, const U & rhs)
{
  typedef typename std::remove_const<typename std::remove_reference<T>::type>::type lhs_type;
  typedef typename std::remove_const<typename std::remove_reference<U>::type>::type rhs_type;

  if constexpr (std::is_integral<lhs_type>::value && std::is_integral<rhs_type>::value)
  {
    if constexpr (std::is_signed<lhs_type>::value == std::is_signed<rhs_type>::value)
      return lhs < rhs;
    else if constexpr (std::is_signed<lhs_type>::value)
    {
      if (lhs < 0)
        return true;
      return static_cast<typename std::make_unsigned<lhs_type>::type>(lhs) < rhs;
    }
    else
    {
      if (rhs < 0)
        return false;
      return lhs < static_cast<typename std::make_unsigned<rhs_type>::type>(rhs);
    }
  }
  else
    return lhs < rhs;
}

template <typename ForwardIt, typename T>
METAPHYSICL_INLINE ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T & value)
{
  auto count = last - first;
  while (count > 0)
  {
    const auto step = count / 2;
    auto it = first + step;
    if (less(*it, value))
    {
      first = ++it;
      count -= step + 1;
    }
    else
      count = step;
  }

  return first;
}

template <typename BidirectionalIt1, typename BidirectionalIt2>
METAPHYSICL_INLINE BidirectionalIt2 copy_backward(BidirectionalIt1 first,
                                                  BidirectionalIt1 last,
                                                  BidirectionalIt2 result)
{
  while (first != last)
    *(--result) = *(--last);
  return result;
}

template <typename ForwardIt>
METAPHYSICL_INLINE ForwardIt adjacent_find(ForwardIt first, ForwardIt last)
{
  if (first == last)
    return last;

  auto next = first;
  ++next;
  for (; next != last; ++first, ++next)
    if (*first == *next)
      return first;

  return last;
}

template <typename ForwardIt>
METAPHYSICL_INLINE bool is_sorted(ForwardIt first, ForwardIt last)
{
  if (first != last)
  {
    auto next = first;
    while (++next != last)
    {
      if (less(*next, *first))
        return false;
      first = next;
    }
  }

  return true;
}

#else

template <typename ForwardIt, typename T>
METAPHYSICL_INLINE ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T & value)
{
  return std::lower_bound(first, last, value);
}

template <typename BidirectionalIt1, typename BidirectionalIt2>
METAPHYSICL_INLINE BidirectionalIt2 copy_backward(BidirectionalIt1 first,
                                                  BidirectionalIt1 last,
                                                  BidirectionalIt2 result)
{
  return std::copy_backward(first, last, result);
}

template <typename ForwardIt>
METAPHYSICL_INLINE ForwardIt adjacent_find(ForwardIt first, ForwardIt last)
{
  return std::adjacent_find(first, last);
}

template <typename ForwardIt>
METAPHYSICL_INLINE bool is_sorted(ForwardIt first, ForwardIt last)
{
  return std::is_sorted(first, last);
}

#endif

// fill and copy forward to the std:: algorithms wherever those are
// callable, i.e. everywhere except in device code. Device code falls
// back to equivalent hand-written loops.

template <typename ForwardIt, typename T>
METAPHYSICL_INLINE void fill(ForwardIt first, ForwardIt last, const T & value)
{
#ifdef METAPHYSICL_KOKKOS_COMPILATION
  KOKKOS_IF_ON_HOST((std::fill(first, last, value);))
  KOKKOS_IF_ON_DEVICE((for (; first != last; ++first) *first = value;))
#else
  std::fill(first, last, value);
#endif
}

template <typename InputIt, typename OutputIt>
METAPHYSICL_INLINE OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
{
#ifdef METAPHYSICL_KOKKOS_COMPILATION
  KOKKOS_IF_ON_HOST((d_first = std::copy(first, last, d_first);))
  KOKKOS_IF_ON_DEVICE((for (; first != last; ++first, ++d_first) *d_first = *first;))
  return d_first;
#else
  return std::copy(first, last, d_first);
#endif
}

#ifdef METAPHYSICL_HAVE_EXECUTION_POLICIES

// Overloads taking a std:: execution policy. The policy is passed
// along to std:: on the host and ignored in device code, where the
// work is already being done by a single device thread.

template <typename ExecutionPolicy>
using enable_if_execution_policy_t =
  std::enable_if_t<std::is_execution_policy_v<std::decay_t<ExecutionPolicy>>, int>;

template <typename ExecutionPolicy, typename ForwardIt, typename T,
          enable_if_execution_policy_t<ExecutionPolicy> = 0>
METAPHYSICL_INLINE void fill(ExecutionPolicy && policy,
                             ForwardIt first, ForwardIt last, const T & value)
{
#ifdef METAPHYSICL_KOKKOS_COMPILATION
  KOKKOS_IF_ON_HOST((std::fill(std::forward<ExecutionPolicy>(policy), first, last, value);))
  KOKKOS_IF_ON_DEVICE(((void)policy; fill(first, last, value);))
#else
  std::fill(std::forward<ExecutionPolicy>(policy), first, last, value);
#endif
}

template <typename ExecutionPolicy, typename ForwardIt1, typename ForwardIt2,
          enable_if_execution_policy_t<ExecutionPolicy> = 0>
METAPHYSICL_INLINE ForwardIt2 copy(ExecutionPolicy && policy,
                                   ForwardIt1 first, ForwardIt1 last, ForwardIt2 d_first)
{
#ifdef METAPHYSICL_KOKKOS_COMPILATION
  KOKKOS_IF_ON_HOST((d_first = std::copy(std::forward<ExecutionPolicy>(policy), first, last, d_first);))
  KOKKOS_IF_ON_DEVICE(((void)policy; d_first = copy(first, last, d_first);))
  return d_first;
#else
  return std::copy(std::forward<ExecutionPolicy>(policy), first, last, d_first);
#endif
}

#endif // METAPHYSICL_HAVE_EXECUTION_POLICIES

} // namespace MetaPhysicL::detail

#endif // METAPHYSICL_ALGORITHM_H
