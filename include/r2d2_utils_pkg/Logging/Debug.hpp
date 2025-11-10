#ifndef INCLUDE_R2D2_UTILS_PKG_DEBUG_HPP_
#define INCLUDE_R2D2_UTILS_PKG_DEBUG_HPP_

// ANSI color definitions
#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include "Console.hpp"

[[nodiscard]]
inline auto parse_var_str(std::string_view var_str) {
  std::string tmp_{var_str};
  std::replace(tmp_.begin(), tmp_.end(), ',', ' ');
  std::istringstream iss(tmp_);
  return std::vector<std::string>(std::istream_iterator<std::string>(iss),
                                  std::istream_iterator<std::string>());
};
template <typename T>
inline std::ostringstream& stream_var(std::ostringstream& oss,
                                      std::string_view var_name, T&& var_arg,
                                      std::size_t idx, std::size_t sz) {
  if (var_name == "\"\"") return oss;
  oss << YELLOW(var_name << "=" << std::forward<T>(var_arg));
  if (idx + 1 < sz) oss << ", ";
  return oss;
};
template <typename... Args>
inline std::ostringstream& stream_vars(std::ostringstream& oss,
                                       std::string_view var_str,
                                       Args&&... var_args) {
  auto var_names_{parse_var_str(var_str)};
  std::size_t idx{0};
  constexpr std::size_t sz{sizeof...(var_args)};
  ((stream_var(oss, var_names_[idx], std::forward<Args>(var_args), idx, sz),
    idx++),
   ...);
  return oss;
};
template <typename... Args>
inline auto stream_args(std::string_view label, std::string_view names,
                        Args&&... args) {
  std::ostringstream oss;
  if (!label.empty()) oss << "[" << MAGENTA(label) << "] : ";
  return stream_vars(oss, names, std::forward<Args>(args)...).str();
};

#endif  // INCLUDE_R2D2_UTILS_PKG_DEBUG_HPP_
