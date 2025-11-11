#ifndef INCLUDE_R2D2_UTILS_PKG_POLYNOME_HPP_
#define INCLUDE_R2D2_UTILS_PKG_POLYNOME_HPP_

#include <type_traits>

namespace horner {
template <template <typename> class Vector, typename T>
[[nodiscard]] constexpr T polynome(const Vector<T>& coeffs, const T& x) {
  static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type!");
  if (coeffs.empty()) return T{};
  T acc{coeffs[0]};
  for (std::size_t i = 1; i < coeffs.size(); i++) acc = acc * x + coeffs[i];
  return acc;
};
}  // namespace horner
#endif  // INCLUDE_R2D2_UTILS_PKG_POLYNOME_HPP_
