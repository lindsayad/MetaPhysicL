#include "metaphysicl/metaphysicl_algorithm.h"
#include "metaphysicl/metaphysicl_asserts.h"

#include <array>
#include <vector>

#define METAPHYSICL_UNIT_ASSERT(expr)                                                              \
  if (!(expr))                                                                                     \
  metaphysicl_error()

using namespace MetaPhysicL;

int main()
{
  // Sign-aware comparison
  METAPHYSICL_UNIT_ASSERT(algorithm::detail::less(-1, 0u));
  METAPHYSICL_UNIT_ASSERT(!algorithm::detail::less(0u, -1));
  METAPHYSICL_UNIT_ASSERT(algorithm::detail::less(1u, 2));
  METAPHYSICL_UNIT_ASSERT(!algorithm::detail::less(2, 1u));
  METAPHYSICL_UNIT_ASSERT(algorithm::detail::less(1.5, 2.0));

  const std::vector<unsigned int> sorted = {1, 3, 3, 7, 9};
  const std::vector<unsigned int> unsorted = {1, 7, 3, 9};

  METAPHYSICL_UNIT_ASSERT(algorithm::lower_bound(sorted.begin(), sorted.end(), 3u) ==
                          sorted.begin() + 1);
  METAPHYSICL_UNIT_ASSERT(algorithm::lower_bound(sorted.begin(), sorted.end(), 8u) ==
                          sorted.begin() + 4);
  METAPHYSICL_UNIT_ASSERT(algorithm::lower_bound(sorted.begin(), sorted.end(), 10u) ==
                          sorted.end());

  METAPHYSICL_UNIT_ASSERT(algorithm::adjacent_find(sorted.begin(), sorted.end()) ==
                          sorted.begin() + 1);
  METAPHYSICL_UNIT_ASSERT(algorithm::adjacent_find(unsorted.begin(), unsorted.end()) ==
                          unsorted.end());

  METAPHYSICL_UNIT_ASSERT(algorithm::is_sorted(sorted.begin(), sorted.end()));
  METAPHYSICL_UNIT_ASSERT(!algorithm::is_sorted(unsorted.begin(), unsorted.end()));

  std::array<double, 4> a;
  algorithm::fill(a.begin(), a.end(), 2.0);
  METAPHYSICL_UNIT_ASSERT(a == (std::array<double, 4>{2.0, 2.0, 2.0, 2.0}));

  const std::array<double, 4> src = {1.0, 2.0, 3.0, 4.0};
  METAPHYSICL_UNIT_ASSERT(algorithm::copy(src.begin(), src.end(), a.begin()) == a.end());
  METAPHYSICL_UNIT_ASSERT(a == src);

  // Overlapping shift right by one, as used when inserting into sparse
  // index arrays
  std::vector<unsigned int> shifted = {1, 2, 3, 0};
  METAPHYSICL_UNIT_ASSERT(algorithm::copy_backward(shifted.begin(), shifted.begin() + 3,
                                                   shifted.end()) == shifted.begin() + 1);
  METAPHYSICL_UNIT_ASSERT(shifted == (std::vector<unsigned int>{1, 1, 2, 3}));

#ifdef METAPHYSICL_HAVE_EXECUTION_POLICIES
  // The parallel policies are left out so that this test does not need
  // to link against a parallel backend such as TBB.
  std::vector<double> v(100);
  algorithm::fill(std::execution::seq, v.begin(), v.end(), 3.0);
  for (const auto val : v)
    METAPHYSICL_UNIT_ASSERT(val == 3.0);

  const std::vector<double> v_src(v.size(), 5.0);
  METAPHYSICL_UNIT_ASSERT(algorithm::copy(std::execution::seq, v_src.begin(), v_src.end(),
                                          v.begin()) == v.end());
  METAPHYSICL_UNIT_ASSERT(v == v_src);

#if defined(__cpp_lib_execution) && __cpp_lib_execution >= 201902L
  algorithm::fill(std::execution::unseq, v.begin(), v.end(), 4.0);
  for (const auto val : v)
    METAPHYSICL_UNIT_ASSERT(val == 4.0);
#endif
#endif

  return 0;
}
