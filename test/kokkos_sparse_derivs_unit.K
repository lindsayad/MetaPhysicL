#include "metaphysicl/metaphysicl_config.h"

#ifdef METAPHYSICL_KOKKOS_COMPILATION

#include "metaphysicl/dualsemidynamicsparsenumberarray.h"

#include "kokkos_derivs_unit.h"

int run_kokkos_tests(
    const Kokkos::Random_XorShift64_Pool<Kokkos::DefaultExecutionSpace> &pool) {
  int returnval = 0;

  KokkosSemiDynamicSparseNumberArray<DualNumber<float>, unsigned int,
                                     NWrapper<4>>
      float_sdsna;
  float_sdsna.resize(4);
  float_sdsna.raw_index(0) = 0;
  float_sdsna.raw_index(1) = 1;
  float_sdsna.raw_index(2) = 2;
  float_sdsna.raw_index(3) = 3;
  returnval = returnval || dispatch_kokkos(float_sdsna, pool);

  KokkosSemiDynamicSparseNumberArray<DualNumber<double>, unsigned int,
                                     NWrapper<4>>
      double_sdsna;
  double_sdsna.resize(4);
  double_sdsna.raw_index(0) = 0;
  double_sdsna.raw_index(1) = 1;
  double_sdsna.raw_index(2) = 2;
  double_sdsna.raw_index(3) = 3;
  returnval = returnval || dispatch_kokkos(double_sdsna, pool);

  return returnval;
}

#else

int main() { return 0; }

#endif // METAPHYSICL_KOKKOS_COMPILATION
