#ifndef INCLUDE_R2D2_UTILS_PKG_DEBUG_HPP_
#define INCLUDE_R2D2_UTILS_PKG_DEBUG_HPP_

#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include "Console.hpp"

/**
 * @brief Parses a variable string into a vector of variable names.
 * @param var_str Comma-separated string of variable names
 * @return Vector of variable name strings
 * @details Replaces commas with spaces and splits the string into individual names.
 */
[[nodiscard]]
inline auto parse_var_str(std::string_view var_str) {
  std::string tmp_{var_str};
  std::replace(tmp_.begin(), tmp_.end(), ',', ' ');
  std::istringstream iss(tmp_);
  return std::vector<std::string>(std::istream_iterator<std::string>(iss),
                                  std::istream_iterator<std::string>());
};
/**
 * @brief Streams a single variable with its name and value to an ostringstream.
 * @tparam T Variable type
 * @param oss The output string stream
 * @param color ANSI color code for the variable name
 * @param var_name The variable name
 * @param var_arg The variable value
 * @param idx Current index in the variable list
 * @param sz Total number of variables
 * @return Reference to the output stream
 */
template <typename T>
inline std::ostringstream& stream_var(std::ostringstream& oss,
                                      std::string_view color,
                                      std::string_view var_name, T&& var_arg,
                                      std::size_t idx, std::size_t sz) {
  if (var_name == "\"\"") return oss;
  oss << color << var_name << " = " << std::forward<T>(var_arg) << ANSI_RESET;
  if (idx + 1 < sz) oss << ", ";
  return oss;
};
/**
 * @brief Streams multiple variables with their names and values.
 * @tparam Args Variadic variable types
 * @param oss The output string stream
 * @param color ANSI color code for variable names
 * @param var_str Comma-separated string of variable names
 * @param var_args The variable values
 * @return Reference to the output stream
 */
template <typename... Args>
inline std::ostringstream& stream_vars(std::ostringstream& oss,
                                       std::string_view color,
                                       std::string_view var_str,
                                       Args&&... var_args) {
  auto var_names_{parse_var_str(var_str)};
  std::size_t idx{0};
  constexpr std::size_t sz{sizeof...(var_args)};
  ((stream_var(oss, color, var_names_[idx], std::forward<Args>(var_args), idx,
               sz),
    idx++),
   ...);
  return oss;
};
/**
 * @brief Creates a formatted string with variable names and values.
 * @tparam Args Variadic variable types
 * @param color ANSI color code for variable names
 * @param names Comma-separated string of variable names
 * @param args The variable values
 * @return Formatted string with "name = value" pairs
 */
template <typename... Args>
inline auto stream_args(std::string_view color, std::string_view names,
                        Args&&... args) {
  std::ostringstream oss;
  stream_vars(oss, color, names, std::forward<Args>(args)...);
  return oss.str();
};

#endif  // INCLUDE_R2D2_UTILS_PKG_DEBUG_HPP_
