#ifndef METAPHYSICL_ALGORITHM_H
#define METAPHYSICL_ALGORITHM_H

#include "metaphysicl/metaphysicl_device.h"

#include <algorithm>
#include <type_traits>
#include <utility>

#if __has_include(<version>)
#include <version>
#endif

// libstdc++ advertises __cpp_lib_execution before <execution> is usable
// everywhere. GCC 9 always selects the TBB parallel backend, so including
// <execution> fails unless TBB is installed. GCC 10 selects TBB whenever
// <tbb/tbb.h> exists but only supports the pre-oneTBB interface, so the
// include fails when oneTBB is installed. GCC 11 and later handle both cases.
#if defined(__cpp_lib_execution) && __cpp_lib_execution >= 201603L
#  if !defined(_GLIBCXX_RELEASE) || _GLIBCXX_RELEASE >= 11 || \
      (_GLIBCXX_RELEASE == 10 && !__has_include(<tbb/tbb.h>))
#    define METAPHYSICL_HAVE_EXECUTION_POLICIES
#  endif
#endif

#ifdef METAPHYSICL_HAVE_EXECUTION_POLICIES
#include <execution>
#endif

// Shims for <algorithm> functions that are callable from both host and
// device code. They live in their own namespace, parallel to
// MetaPhysicL::math, so that argument-dependent lookup of an unqualified
// call like `using std::fill; fill(...)` on MetaPhysicL types does not
// find them and become ambiguous.
namespace MetaPhysicL::algorithm {

namespace detail {

// Sign-aware comparison used by the device fallbacks below, so that
// comparisons between signed and unsigned integers give the
// mathematically correct result.
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

} // namespace detail

// Each shim below forwards to the std:: algorithm in host code and falls
// back to an equivalent hand-written loop in device code.

template <typename ForwardIt, typename T>
METAPHYSICL_INLINE ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T & value)
{
  METAPHYSICL_IF_ON_HOST((first = std::lower_bound(first, last, value);));
  METAPHYSICL_IF_ON_DEVICE((
    auto count = last - first;
    while (count > 0)
    {
      const auto step = count / 2;
      auto it = first + step;
      if (detail::less(*it, value))
      {
        first = ++it;
        count -= step + 1;
      }
      else
        count = step;
    }
  ));
  return first;
}

template <typename BidirectionalIt1, typename BidirectionalIt2>
METAPHYSICL_INLINE BidirectionalIt2 copy_backward(BidirectionalIt1 first,
                                                  BidirectionalIt1 last,
                                                  BidirectionalIt2 result)
{
  METAPHYSICL_IF_ON_HOST((result = std::copy_backward(first, last, result);));
  METAPHYSICL_IF_ON_DEVICE((
    while (first != last)
      *(--result) = *(--last);
  ));
  return result;
}

template <typename ForwardIt>
METAPHYSICL_INLINE ForwardIt adjacent_find(ForwardIt first, ForwardIt last)
{
  ForwardIt found = last;
  METAPHYSICL_IF_ON_HOST((found = std::adjacent_find(first, last);));
  METAPHYSICL_IF_ON_DEVICE((
    if (first != last)
    {
      auto next = first;
      ++next;
      for (; next != last; ++first, ++next)
        if (*first == *next)
        {
          found = first;
          break;
        }
    }
  ));
  return found;
}

template <typename ForwardIt>
METAPHYSICL_INLINE bool is_sorted(ForwardIt first, ForwardIt last)
{
  bool sorted = true;
  METAPHYSICL_IF_ON_HOST((sorted = std::is_sorted(first, last);));
  METAPHYSICL_IF_ON_DEVICE((
    if (first != last)
    {
      auto next = first;
      while (++next != last)
      {
        if (detail::less(*next, *first))
        {
          sorted = false;
          break;
        }
        first = next;
      }
    }
  ));
  return sorted;
}

template <typename ForwardIt, typename T>
METAPHYSICL_INLINE void fill(ForwardIt first, ForwardIt last, const T & value)
{
  METAPHYSICL_IF_ON_HOST((std::fill(first, last, value);));
  METAPHYSICL_IF_ON_DEVICE((for (; first != last; ++first) *first = value;));
}

template <typename InputIt, typename OutputIt>
METAPHYSICL_INLINE OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
{
  METAPHYSICL_IF_ON_HOST((d_first = std::copy(first, last, d_first);));
  METAPHYSICL_IF_ON_DEVICE((for (; first != last; ++first, ++d_first) *d_first = *first;));
  return d_first;
}

#ifdef METAPHYSICL_HAVE_EXECUTION_POLICIES

// Overloads taking a std:: execution policy. The policy is passed
// along to std:: on the host and ignored in device code, where the
// work is already being done by a single device thread.

namespace detail {
template <typename ExecutionPolicy>
using enable_if_execution_policy_t =
  std::enable_if_t<std::is_execution_policy_v<std::decay_t<ExecutionPolicy>>, int>;
} // namespace detail

template <typename ExecutionPolicy, typename ForwardIt, typename T,
          detail::enable_if_execution_policy_t<ExecutionPolicy> = 0>
METAPHYSICL_INLINE void fill(ExecutionPolicy && policy,
                             ForwardIt first, ForwardIt last, const T & value)
{
  METAPHYSICL_IF_ON_HOST((std::fill(std::forward<ExecutionPolicy>(policy), first, last, value);));
  METAPHYSICL_IF_ON_DEVICE(((void)policy; fill(first, last, value);));
}

template <typename ExecutionPolicy, typename ForwardIt1, typename ForwardIt2,
          detail::enable_if_execution_policy_t<ExecutionPolicy> = 0>
METAPHYSICL_INLINE ForwardIt2 copy(ExecutionPolicy && policy,
                                   ForwardIt1 first, ForwardIt1 last, ForwardIt2 d_first)
{
  METAPHYSICL_IF_ON_HOST((d_first = std::copy(std::forward<ExecutionPolicy>(policy), first, last, d_first);));
  METAPHYSICL_IF_ON_DEVICE(((void)policy; d_first = copy(first, last, d_first);));
  return d_first;
}

#endif // METAPHYSICL_HAVE_EXECUTION_POLICIES

} // namespace MetaPhysicL::algorithm

#endif // METAPHYSICL_ALGORITHM_H
