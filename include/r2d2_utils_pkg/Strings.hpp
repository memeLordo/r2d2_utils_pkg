#ifndef INCLUDE_R2D2_UTILS_PKG_STRINGS_HPP_
#define INCLUDE_R2D2_UTILS_PKG_STRINGS_HPP_

#include <algorithm>
#include <cassert>
#include <string>
#include <string_view>

namespace r2d2_string {
/**
 * @brief Transforms a substring of a string view using a transformation function.
 * @tparam TransformFunc The transformation function type
 * @param sv The string view to transform
 * @param func The transformation function to apply to each character
 * @param from Start index (default: 0)
 * @param to End index (default: npos, meaning end of string)
 * @return A new string with the transformed substring
 * @details Creates a copy of the string and applies the transformation function
 *          to characters in the specified range.
 */
template <typename TransformFunc>
[[nodiscard]]
inline std::string transform_string(std::string_view sv, TransformFunc func,
                                    size_t from = 0,
                                    size_t to = std::string_view::npos) {
  if (to == std::string_view::npos) to = sv.length();
  assert(from <= sv.length());
  assert(to >= from && to <= sv.length());

  std::string str{sv};
  auto begin_{str.begin() + from};
  auto end_{str.begin() + to};
  std::transform(begin_, end_, begin_, func);
  return str;
};

/**
 * @brief Converts a substring to uppercase.
 * @param sv The string view to convert
 * @param from Start index (default: 0)
 * @param to End index (default: npos, meaning end of string)
 * @return A new string with the specified substring in uppercase
 */
[[nodiscard]]
inline std::string upper(std::string_view sv, size_t from = 0,
                         size_t to = std::string_view::npos) {
  return transform_string(sv, ::toupper, from, to);
};
/**
 * @brief Converts a substring to lowercase.
 * @param sv The string view to convert
 * @param from Start index (default: 0)
 * @param to End index (default: npos, meaning end of string)
 * @return A new string with the specified substring in lowercase
 */
[[nodiscard]]
inline std::string lower(std::string_view sv, size_t from = 0,
                         size_t to = std::string_view::npos) {
  return transform_string(sv, ::tolower, from, to);
};
}  // namespace r2d2_string

#endif  // INCLUDE_R2D2_UTILS_PKG_STRINGS_HPP_
