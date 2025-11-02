#ifndef INCLUDE_R2D2_UTILS_PKG_DEBUG_HPP_
#define INCLUDE_R2D2_UTILS_PKG_DEBUG_HPP_

// ANSI color definitions
#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#define ANSI_COLOR_RED "\033[0;31m"
#define ANSI_COLOR_GREEN "\033[0;32m"
#define ANSI_COLOR_YELLOW "\033[0;33m"
#define ANSI_COLOR_BLUE "\033[0;34m"
#define ANSI_COLOR_MAGENTA "\033[0;35m"
#define ANSI_COLOR_CYAN "\033[0;36m"
#define ANSI_COLOR_WHITE "\033[0;37m"
#define ANSI_COLOR_NONE "\033[0m"

#define ANSI_COLOR_RESET ANSI_COLOR_GREEN

// Color stream
#define RED(x) ANSI_COLOR_RED << x << ANSI_COLOR_RESET
#define GREEN(x) ANSI_COLOR_GREEN << x << ANSI_COLOR_RESET
#define YELLOW(x) ANSI_COLOR_YELLOW << x << ANSI_COLOR_RESET
#define BLUE(x) ANSI_COLOR_BLUE << x << ANSI_COLOR_RESET
#define MAGENTA(x) ANSI_COLOR_MAGENTA << x << ANSI_COLOR_RESET
#define CYAN(x) ANSI_COLOR_CYAN << x << ANSI_COLOR_RESET
#define WHITE(x) ANSI_COLOR_WHITE << x << ANSI_COLOR_RESET

[[nodiscard]]
inline auto parse_var_str(std::string_view var_str) {
  std::string tmp_{var_str};
  std::replace(tmp_.begin(), tmp_.end(), ',', ' ');
  std::istringstream iss(tmp_);
  return std::vector<std::string>(std::istream_iterator<std::string>(iss),
                                  std::istream_iterator<std::string>());
}

template <typename T>
inline std::ostringstream& stream_var(std::ostringstream& oss,
                                      std::string_view var_name, T&& var_arg,
                                      std::size_t idx, std::size_t sz) {
  oss << YELLOW(var_name << "=" << std::forward<T>(var_arg));
  if (idx + 1 < sz) oss << ", ";
  return oss;
}

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
}

template <typename... Args>
inline auto stream_args(std::string_view label, std::string_view names,
                        Args&&... args) {
  std::ostringstream oss;
  if (!label.empty()) oss << "[" << MAGENTA(label) << "] : ";
  return stream_vars(oss, names, std::forward<Args>(args)...).str();
}

#define STREAM_VARS(label, ...) stream_args(label, #__VA_ARGS__, __VA_ARGS__)

#endif  // INCLUDE_R2D2_UTILS_PKG_DEBUG_HPP_
