#include "metaphysicl/metaphysicl_config.h"

#ifdef METAPHYSICL_KOKKOS_COMPILATION

#include "metaphysicl/dualnumberarray.h"

#include "kokkos_derivs_unit.h"

int run_kokkos_tests(
    const Kokkos::Random_XorShift64_Pool<Kokkos::DefaultExecutionSpace> &pool) {
  int returnval = 0;

  returnval = returnval || dispatch_kokkos(NumberArray<N, DualNumber<float>>(0), pool);
  returnval = returnval || dispatch_kokkos(NumberArray<N, DualNumber<double>>(0), pool);

  return returnval;
}

#else

int main() { return 0; }

#endif // METAPHYSICL_KOKKOS_COMPILATION
