// Shared driver for the Kokkos derivative unit tests. Each including
// translation unit first includes the header for the derivative type it
// tests, then defines run_kokkos_tests() to dispatch every instantiation
// of that type through dispatch_kokkos().

#ifndef METAPHYSICL_KOKKOS_DERIVS_UNIT_H
#define METAPHYSICL_KOKKOS_DERIVS_UNIT_H

#include <cstdlib> // rand()
#include <iostream>
#include <limits>

#include "metaphysicl/metaphysicl_exceptions.h"
#include "metaphysicl/metaphysicl_math.h"

#include "metaphysicl/ignore_warnings.h"
#include <Kokkos_Core.hpp>
#include <Kokkos_Random.hpp>
#include "metaphysicl/restore_warnings.h"

using namespace MetaPhysicL;

// Change the instantiations in each driver iff changing this
static const unsigned int N = 4; // test pts.

#define one_test(test_func)                                                    \
  error_vec = raw_value(test_func);                                            \
  {                                                                            \
    int new_returnval = test_error_vec(random_vec, error_vec);                 \
    returnval = returnval || new_returnval;                                    \
  }

template <typename Vector, typename DualVector>
METAPHYSICL_INLINE int test_error_vec(const DualVector &random_vec,
                                      const Vector &error_vec) {
  typedef typename ValueType<Vector>::type Scalar;

  static const Scalar tol = MetaPhysicL::numeric_limits<Scalar>::epsilon() * 10;

  Scalar max_abs_error = 0;

  for (unsigned int i = 0; i != error_vec.size(); ++i) {
    max_abs_error = math::max(max_abs_error, math::fabs(error_vec.raw_at(i)));

    // Handle NaNs properly.  Testing max_abs_error for NaN is
    // impossible because IEEE sucks:
    // https://en.wikipedia.org/wiki/IEEE_754_revision#min_and_max
    if (max_abs_error > tol || error_vec[i] != error_vec[i])
      return 1;
  }

  return 0;
}

template <typename Vector>
METAPHYSICL_INLINE int
vectester(Vector zerovec,
          Kokkos::Random_XorShift64_Pool<Kokkos::DefaultExecutionSpace> pool) {
  typedef typename ValueType<Vector>::type DualScalar;
  typedef typename DualScalar::value_type Scalar;

  auto gen = pool.get_state();

  Vector random_vec = zerovec;

  typename DerivativeType<Vector>::type error_vec = 0;

  // Avoid divide by zero errors or acos(x>1) NaNs later
  for (unsigned int i = 0; i != N; ++i) {
    random_vec.raw_at(i) = .25 + (static_cast<Scalar>(gen.drand()) /
                                  static_cast<Scalar>(RAND_MAX) / 2);
    random_vec.raw_at(i).derivatives() = 1;
  }

  int returnval = 0;

  one_test(2 * random_vec - random_vec - random_vec);
  one_test(3 * random_vec - random_vec * 3);
  one_test((random_vec + random_vec) / 2 - random_vec);
  one_test(2. * random_vec - random_vec - 1.f * random_vec);
  one_test(math::sqrt(random_vec) * math::sqrt(random_vec) - random_vec);
  one_test(random_vec - math::sin(math::asin(random_vec)));
  one_test(random_vec - math::tan(math::atan(random_vec)));
  one_test(math::floor(random_vec / 2));
  one_test(math::abs(random_vec) - random_vec);
#if __cplusplus >= 201103L
  one_test(math::cbrt(random_vec) - math::pow(random_vec, Scalar(1) / 3));
  one_test(math::asinh(math::sinh(random_vec)) - random_vec);
  one_test(math::atanh(math::tanh(random_vec)) - random_vec);
  one_test(math::trunc(random_vec));
  one_test(math::round(random_vec / 2));
  one_test(derivatives(math::trunc(random_vec * 10)));
  one_test(derivatives(math::round(random_vec * 10)));
#endif // __cplusplus >= 201103L

  return returnval;
}

template <typename T> struct LaunchTester {
  Kokkos::View<int> ival;
  T tested_type;
  Kokkos::Random_XorShift64_Pool<Kokkos::DefaultExecutionSpace> pool;

  KOKKOS_INLINE_FUNCTION
  void operator()(int i) const { ival() = vectester(tested_type, pool); }
};

template <typename T>
int dispatch_kokkos(
    const T &tested_type,
    const Kokkos::Random_XorShift64_Pool<Kokkos::DefaultExecutionSpace> &pool) {
  Kokkos::View<int> ival("ival");
  Kokkos::parallel_for("call_tester_once",
                       Kokkos::RangePolicy<Kokkos::DefaultExecutionSpace>(0, 1),
                       LaunchTester<T>{ival, tested_type, pool});
  Kokkos::fence();
  int returnval;
  Kokkos::deep_copy(returnval, ival);
  return returnval;
}

// Defined by each driver: dispatch every tested instantiation and
// return nonzero on any failure.
int run_kokkos_tests(
    const Kokkos::Random_XorShift64_Pool<Kokkos::DefaultExecutionSpace> &pool);

int main(int argc, char *argv[]) {
  Kokkos::initialize(argc, argv);

  int returnval = 0;

  {
    Kokkos::Random_XorShift64_Pool<Kokkos::DefaultExecutionSpace> pool(
        /*seed*/ 12345);

    returnval = run_kokkos_tests(pool);
  }

  Kokkos::finalize();

  return returnval;
}

#endif // METAPHYSICL_KOKKOS_DERIVS_UNIT_H
