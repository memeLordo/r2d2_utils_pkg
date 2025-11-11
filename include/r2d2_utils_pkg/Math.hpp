#ifndef INCLUDE_R2D2_UTILS_PKG_MATH_HPP_
#define INCLUDE_R2D2_UTILS_PKG_MATH_HPP_

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <type_traits>

namespace r2d2_math {
using std::is_arithmetic_v;

/**
 * @brief Returns the minimum of two values.
 * @tparam T Numeric type
 * @param a First value
 * @param b Second value
 * @return The minimum of a and b
 */
template <typename T>
[[nodiscard]] constexpr T min(const T a, const T b) {
  return std::min(a, b);
};
/**
 * @brief Returns the maximum of two values.
 * @tparam T Numeric type
 * @param a First value
 * @param b Second value
 * @return The maximum of a and b
 */
template <typename T>
[[nodiscard]] constexpr T max(const T a, const T b) {
  return std::max(a, b);
};
/**
 * @brief Returns the absolute value of a number.
 * @tparam T Arithmetic type
 * @param a The value
 * @return The absolute value of a
 */
template <typename T>
[[nodiscard]] constexpr T abs(const T a) {
  static_assert(is_arithmetic_v<T>, "abs: T must be an arithmetic type!");
  return std::abs(a);
};
/**
 * @brief Converts degrees to radians.
 * @tparam T Arithmetic type
 * @param a Angle in degrees
 * @return Angle in radians
 */
template <typename T>
[[nodiscard]] constexpr T deg2rad(const T a) {
  static_assert(is_arithmetic_v<T>, "deg2rad: T must be an arithmetic type!");
  return a * T{M_PI} / T{180};
};
/**
 * @brief Calculates sine of an angle in degrees.
 * @tparam T Arithmetic type
 * @param theta Angle in degrees
 * @return Sine of the angle
 */
template <typename T>
[[nodiscard]] constexpr T sin(const T theta) {
  static_assert(is_arithmetic_v<T>, "sin: T must be an arithmetic type!");
  return std::sin(deg2rad(theta));
};
/**
 * @brief Calculates the square of a value.
 * @tparam T Arithmetic type
 * @param a The value
 * @return The square of a (a * a)
 */
template <typename T>
[[nodiscard]] constexpr T sqr(const T a) {
  static_assert(is_arithmetic_v<T>, "sqr: T must be an arithmetic type!");
  return a * a;
};
/**
 * @brief Returns the sign of a value.
 * @tparam T Arithmetic type
 * @param a The value
 * @return 1 if positive, -1 if negative, 0 if zero
 */
template <typename T>
[[nodiscard]] constexpr int8_t sign(const T a) {
  static_assert(is_arithmetic_v<T>, "sign: T must be an arithmetic type!");
  return (a > T{0}) - (a < T{0});
};
}  // namespace r2d2_math

namespace r2d2_process {
/**
 * @brief Template struct for wrapping/unwrapping values using a conversion ratio.
 * @tparam ratio Reference to the conversion ratio constant
 * @details Provides static methods to convert between raw and scaled values.
 */
template <const double& ratio>
struct Wrapper final {
  Wrapper() = delete;
  Wrapper(const Wrapper&) = delete;
  Wrapper(Wrapper&&) = delete;
  Wrapper& operator=(const Wrapper&) = delete;
  Wrapper& operator=(Wrapper&&) = delete;

  /**
   * @brief Wraps a value by dividing by the ratio.
   * @tparam T Output type
   * @tparam T2 Input type
   * @param value The value to wrap
   * @return The wrapped value (value / ratio)
   */
  template <typename T, typename T2>
  [[nodiscard]] static constexpr T wrap(const T2 value) {
    return static_cast<T>(value / ratio);
  };
  /**
   * @brief Unwraps a value by multiplying by the ratio.
   * @tparam T Output type
   * @tparam T2 Input type
   * @param rawValue The raw value to unwrap
   * @return The unwrapped value (rawValue * ratio)
   */
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

#endif  // INCLUDE_R2D2_UTILS_PKG_MATH_HPP_
