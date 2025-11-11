#ifndef INCLUDE_R2D2_UTILS_PKG_POLYNOME_HPP_
#define INCLUDE_R2D2_UTILS_PKG_POLYNOME_HPP_

#include <type_traits>

namespace horner {
/**
 * @brief   Evaluates a polynomial using Horner's method.
 *
 * @tparam  Vector The vector/container type for coefficients
 * @tparam  T      Numeric type
 * @param   coeffs Vector of polynomial coefficients (highest degree first)
 * @param   x      The value to evaluate the polynomial at
 * @return         The result of the polynomial evaluation
 *
 * @details Uses Horner's method for efficient polynomial evaluation.
 *          Returns 0 if the coefficients vector is empty.
 */
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
