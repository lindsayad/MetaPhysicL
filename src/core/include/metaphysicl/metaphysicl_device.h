#ifndef METAPHYSICL_DEVICE_H
#define METAPHYSICL_DEVICE_H

#ifdef ACSM_KOKKOS_COMPILATION
#define METAPHYSICL_KOKKOS_COMPILATION
#endif

#ifdef METAPHYSICL_KOKKOS_COMPILATION

#include "metaphysicl/ignore_warnings.h"
#include <Kokkos_Array.hpp>
#include <Kokkos_Macros.hpp>
#include "metaphysicl/restore_warnings.h"

#define METAPHYSICL_INLINE KOKKOS_INLINE_FUNCTION
#define METAPHYSICL_IF_ON_HOST(...)                                            \
  do {                                                                         \
    KOKKOS_IF_ON_HOST(__VA_ARGS__)                                             \
  } while (0)
#define METAPHYSICL_IF_ON_DEVICE(...)                                          \
  do {                                                                         \
    KOKKOS_IF_ON_DEVICE(__VA_ARGS__)                                           \
  } while (0)

namespace MetaPhysicL::detail {
template <typename T> METAPHYSICL_INLINE auto begin(T &t) {
  return Kokkos::begin(t);
}
} // namespace MetaPhysicL

#else

#include <array>

#define METAPHYSICL_INLINE inline
// Helper to remove one level of parentheses
#define METAPHYSICL_UNPAREN(...) __VA_ARGS__

// Make it a standalone statement (no trailing ';' required), and
// unparen so ( ... ; ) becomes ... ;
#define METAPHYSICL_IF_ON_HOST(CODE)                                           \
  do {                                                                         \
    METAPHYSICL_UNPAREN CODE                                                   \
  } while (0)

// Without Kokkos there is no device code, so CODE is dropped.
#define METAPHYSICL_IF_ON_DEVICE(CODE)                                         \
  do {                                                                         \
  } while (0)

namespace MetaPhysicL::detail {
template <typename T> auto begin(T &t) { return std::begin(t); }
} // namespace MetaPhysicL
#endif

#endif // METAPHYSICL_DEVICE_H
