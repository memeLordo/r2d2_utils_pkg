#ifndef R2D2_MATH_HPP
#define R2D2_MATH_HPP

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace r2d2_math {
template <typename T>
constexpr T min(const T a, const T b) {
  return std::min<T>(a, b);
};
template <typename T>
constexpr T max(const T a, const T b) {
  return std::max<T>(a, b);
};
template <typename T>
constexpr T deg2rad(const T a) {
  return a * M_PI / 180.0;
};
template <typename T>
constexpr T sin(const T thetha) {
  return std::sin(deg2rad(thetha));
};
template <typename T>
constexpr T sqr(const T a) {
  return a * a;
};
template <typename T>
constexpr int8_t sign(const T a) {
  return a ? a > 0 ? 1 : -1 : 0;
};
}  // namespace r2d2_math

namespace r2d2_process {
template <class Derived>
class Wrapper {
 protected:
  static constexpr double getRatio() { return Derived::s_convertRatio; }

 public:
  template <typename T>
  static constexpr T wrap(const T a) {
    return a * getRatio();
  };
  template <typename T>
  static constexpr T unwrap(const T a) {
    return a / getRatio();
  };
  template <typename T, typename T2>
  static constexpr T wrap(const T2 a) {
    return static_cast<T>(a * getRatio());
  };
  template <typename T, typename T2>
  static constexpr T unwrap(const T2 a) {
    return static_cast<T>(a / getRatio());
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
