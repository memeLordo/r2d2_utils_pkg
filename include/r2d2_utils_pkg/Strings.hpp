#ifndef R2D2_STRING_HPP
#define R2D2_STRING_HPP

#include <algorithm>
#include <cassert>
#include <string>
#include <string_view>

namespace r2d2_string {
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
[[nodiscard]]
inline std::string lower(std::string_view sv) {
  std::string str(sv);
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  return str;
};
[[nodiscard]]
inline std::string lower(std::string_view sv, size_t from) {
  assert(from <= sv.length());
  std::string str(sv);
  auto begin_ = str.begin() + from;
  auto end_ = str.end();
  std::transform(begin_, end_, begin_, ::tolower);
  return str;
};
[[nodiscard]]
inline std::string lower(std::string_view sv, size_t from, size_t to) {
  assert(from <= sv.length());
  assert(to >= from && to <= sv.length());
  std::string str(sv);
  auto begin_ = str.begin() + from;
  auto end_ = str.begin() + to;
  std::transform(begin_, end_, begin_, ::tolower);
  return str;
};

[[nodiscard]]
inline std::string upper(std::string_view sv) {
  std::string str(sv);
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
  return str;
};
[[nodiscard]]
inline std::string upper(std::string_view sv, size_t from) {
  assert(from <= sv.length());
  std::string str(sv);
  auto begin_ = str.begin() + from;
  auto end_ = str.end();
  std::transform(begin_, end_, begin_, ::toupper);
  return str;
};
[[nodiscard]]
inline std::string upper(std::string_view sv, size_t from, size_t to) {
  assert(from <= sv.length());
  assert(to >= from && to <= sv.length());
  std::string str(sv);
  auto begin_ = str.begin() + from;
  auto end_ = str.begin() + to;
  std::transform(begin_, end_, begin_, ::toupper);
  return str;
};
}  // namespace r2d2_string

#endif  // R2D2_STRING_HPP
