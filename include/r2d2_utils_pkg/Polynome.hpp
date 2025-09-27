#ifndef R2D2_POLYNOME_HPP
#define R2D2_POLYNOME_HPP

#include <type_traits>

namespace horner {
template <template <typename> class Container, typename T>
[[nodiscard]] constexpr T polynome(const Container<T>& coeffs, const T x) {
  static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type");
  if (coeffs.empty()) return T{};

  auto it{coeffs.begin()};
  T result_{*it++};
  for (; it != coeffs.end(); ++it) result_ = result_ * x + *it;
  return result_;
};
}  // namespace horner
#endif  // R2D2_POLYNOME_HPP
