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
#define ANSI_COLOR_RESET "\033[0m"

// Color stream
#define RED(x) ANSI_COLOR_RED << x << ANSI_COLOR_RESET
#define GREEN(x) ANSI_COLOR_GREEN << x << ANSI_COLOR_RESET
#define YELLOW(x) ANSI_COLOR_YELLOW << x << ANSI_COLOR_RESET
#define BLUE(x) ANSI_COLOR_BLUE << x << ANSI_COLOR_RESET
#define MAGENTA(x) ANSI_COLOR_MAGENTA << x << ANSI_COLOR_RESET
#define CYAN(x) ANSI_COLOR_CYAN << x << ANSI_COLOR_RESET
#define WHITE(x) ANSI_COLOR_WHITE << x << ANSI_COLOR_RESET

// inline std::string prettyName(const std::string& prettyFunction) {
//   return prettyFunction.substr();
// }
// #define __FUNC_NAME__ prettyName(__PRETTY_FUNCTION__)

// Вспомогательная печать одной пары имя-значение
// inline constexpr bool is_valid_str(std::string_view name) {
//   if (name.empty() || !std::isalpha(name[0]) || name[0] == '_') return false;
//   return std::all_of(std::next(name.begin()), name.end(),
//                      [](char c) { return std::isalnum(c) || c == '_'; });
// }

[[nodiscard]]
inline std::vector<std::string> parse_var_str(std::string_view var_str) {
  std::string tmp_{var_str};
  std::replace(tmp_.begin(), tmp_.end(), ',', ' ');
  std::istringstream iss(tmp_);
  return std::vector<std::string>(std::istream_iterator<std::string>(iss),
                                  std::istream_iterator<std::string>());
}

template <typename T>
inline void stream_var(std::ostringstream& oss, std::string_view var_name,
                       T&& var_arg) {
  oss << YELLOW(var_name << "=" << std::forward<T>(var_arg)) << " ";
}

template <typename... Args>
inline void stream_vars(std::ostringstream& oss, std::string_view var_str,
                        Args&&... var_args) {
  auto var_names_{parse_var_str(var_str)};
  std::size_t idx{0};
  (stream_var(oss, var_names_[idx++], std::forward<Args>(var_args)), ...);
}

template <typename... Args>
inline std::ostringstream stream_args(std::string_view label,
                                      std::string_view names, Args&&... args) {
  std::ostringstream oss;
  if (!label.empty()) oss << "[" << MAGENTA(label) << "] : ";
  stream_vars(oss, names, std::forward<Args>(args)...);
  return oss;
}

// // Non-void return type version
// template <typename Func, typename OutFunc, typename... Args>
// inline auto log_func(const std::string func_name, Func func, OutFunc outfunc,
//                      const std::string names, Args&&... args) ->
//     typename std::enable_if<
//         !std::is_void<typename std::result_of<Func(Args&&...)>::type>::value,
//         typename std::result_of<Func(Args&&...)>::type>::type {
//   std::ostringstream oss;
//   auto result_ = func(std::forward<Args>(args)...);
//
//   oss << "[" << MAGENTA(func_name) << "](";
//   debug_print_args(oss, names, args...);
//   oss << ") : " << WHITE(result_);
//   outfunc(oss.str());
//
//   return result_;
// }

// // Void return type version
// template <typename Func, typename OutFunc, typename... Args>
// inline typename std::enable_if<
//     std::is_void<typename std::result_of<Func(Args&&...)>::type>::value,
//     void>::type
// log_func(const std::string func_name, Func func, OutFunc outfunc,
//          const std::string names, Args&&... args) {
//   std::ostringstream oss;
//   oss << "[" << MAGENTA(func_name) << "](";
//   debug_print_args(oss, names, args...);
//   oss << ") : " << WHITE("(void)");
//   outfunc(oss.str());
// }

#define STREAM_VARS(label, ...) stream_args(label, #__VA_ARGS__, __VA_ARGS__)

// #define LOG_FUNC_(func, outfunc, ...) \
//   log_func( \
//       #func, \
//       [&](auto &&...args) -> decltype(auto) { \
//         return func(std::forward<decltype(args)>(args)...); \
//       }, \ outfunc, #__VA_ARGS__, __VA_ARGS__)
// #define LOG_CLASS_FUNC_(func, outfunc, ...) \
//   log_func( \
//       __FUNC_NAME__ + "->" #func, \
//       [&](auto &&...args) -> decltype(auto) { \
//         return func(std::forward<decltype(args)>(args)...); \
//       }, \ outfunc, #__VA_ARGS__, __VA_ARGS__)
// #define LOG_FUNC(func, ...)                                                \
//   LOG_FUNC_(                                                               \
//       func, [](const std::string& msg) { std::cout << msg << std::endl; }, \
//       __VA_ARGS__)
// #define LOG_CLASS_FUNC(func, ...)                                          \
//   LOG_CLASS_FUNC_(                                                         \
//       func, [](const std::string& msg) { std::cout << msg << std::endl; }, \
//       __VA_ARGS__)

#endif  // INCLUDE_R2D2_UTILS_PKG_DEBUG_HPP_
