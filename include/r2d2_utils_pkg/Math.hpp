#ifndef R2D2_MATH_HPP
#define R2D2_MATH_HPP

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <type_traits>

namespace r2d2_math {
using std::is_arithmetic_v;

template <typename T>
[[nodiscard]] constexpr T min(const T a, const T b) {
  return std::min(a, b);
};
template <typename T>
[[nodiscard]] constexpr T max(const T a, const T b) {
  return std::max(a, b);
};
template <typename T>
[[nodiscard]] constexpr T abs(const T a) {
  static_assert(is_arithmetic_v<T>, "abs: T must be an arithmetic type!");
  return std::abs(a);
};
template <typename T>
[[nodiscard]] constexpr T deg2rad(const T a) {
  static_assert(is_arithmetic_v<T>, "deg2rad: T must be an arithmetic type!");
  return a * T{M_PI} / T{180};
};
template <typename T>
[[nodiscard]] constexpr T sin(const T theta) {
  static_assert(is_arithmetic_v<T>, "sin: T must be an arithmetic type!");
  return std::sin(deg2rad(theta));
};
template <typename T>
[[nodiscard]] constexpr T sqr(const T a) {
  static_assert(is_arithmetic_v<T>, "sqr: T must be an arithmetic type!");
  return a * a;
};
template <typename T>
[[nodiscard]] constexpr int8_t sign(const T a) {
  static_assert(is_arithmetic_v<T>, "sign: T must be an arithmetic type!");
  return (a > T{0}) - (a < T{0});
};
}  // namespace r2d2_math

namespace r2d2_process {
template <const double& ratio>
struct Wrapper {

  template <typename T, typename T2>
  [[nodiscard]] static constexpr T wrap(const T2 value) {
    return static_cast<T>(value / ratio);
  };
  template <typename T, typename T2>
  [[nodiscard]] static constexpr T unwrap(const T2 rawValue) {
    return static_cast<T>(rawValue * ratio);
  };
};

namespace config {
extern const double g_angleRatio;
extern const double g_forceRatio;
}  // namespace config

using Angle = Wrapper<config::g_angleRatio>;
using Force = Wrapper<config::g_forceRatio>;
}  // namespace r2d2_process

#endif  // R2D2_MATH_HPP
