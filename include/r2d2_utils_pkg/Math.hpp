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
template <class Derived>
class Wrapper {
 protected:
  template <typename T = double>
  [[nodiscard]] static constexpr T getRatio() {
    assert(Derived::s_convertRatio != 0);
    return static_cast<T>(Derived::s_convertRatio);
  };

 public:
  template <typename T>
  [[nodiscard]] static constexpr T wrap(const T value) {
    return value / getRatio<T>();
  };
  template <typename T>
  [[nodiscard]] static constexpr T unwrap(const T rawValue) {
    return rawValue * getRatio<T>();
  };
  template <typename T, typename T2>
  [[nodiscard]] static constexpr T wrap(const T2 value) {
    return static_cast<T>(value / getRatio());
  };
  template <typename T, typename T2>
  [[nodiscard]] static constexpr T unwrap(const T2 rawValue) {
    return static_cast<T>(rawValue * getRatio());
  };
};
class Angle : public Wrapper<Angle> {
  friend class Wrapper<Angle>;
  static const double s_convertRatio;
};
class Force : public Wrapper<Force> {
  friend class Wrapper<Force>;
  static const double s_convertRatio;
};
}  // namespace r2d2_process

#endif  // R2D2_MATH_HPP
