#ifndef INCLUDE_R2D2_UTILS_PKG_POLYNOME_HPP_
#define INCLUDE_R2D2_UTILS_PKG_POLYNOME_HPP_

#include <iterator>
#include <numeric>
#include <type_traits>

namespace horner {
template <template <typename> class Container, typename T>
[[nodiscard]] constexpr T polynome(const Container<T>& coeffs, const T& x) {
  static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type!");
  if (coeffs.empty()) return T{};
  return std::accumulate(
      std::next(coeffs.cbegin()), coeffs.cend(), coeffs[0],
      [&](const T& acc, const T& coeff) { return acc * x + coeff; });
};
}  // namespace horner
#endif  // INCLUDE_R2D2_UTILS_PKG_POLYNOME_HPP_
