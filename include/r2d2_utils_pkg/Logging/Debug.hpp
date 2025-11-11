#ifndef INCLUDE_R2D2_UTILS_PKG_DEBUG_HPP_
#define INCLUDE_R2D2_UTILS_PKG_DEBUG_HPP_

#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include "Console.hpp"

/**
 * @brief   Parses a variable string into a vector of variable names.
 *
 * @param   var_str Comma-separated string of variable names
 * @return          Vector of variable name strings
 *
 * @details Replaces commas with spaces and splits the string into individual
 *          names.
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
 * @brief   Streams a single variable with its name and value to an
 *          output stream.
 *
 * @tparam  T     Variable type
 * @param   oss   The output string stream
 * @param   color ANSI color code for the variable name
 * @param   name  The variable name
 * @param   arg   The variable value
 * @param   idx   Current index in the variable list
 * @param   sz    Total number of variables
 * @return        Reference to the output stream
 *
 * @details If the variable name is empty, does not print anything. Otherwise,
 *          prints the variable name and value, followed by a comma if the name
 *          is not the last.
 */
template <typename T>
inline std::ostringstream& stream_var(std::ostringstream& oss,
                                      std::string_view color,
                                      std::string_view name, T&& arg,
                                      std::size_t idx, std::size_t sz) {
  if (name == "\"\"") return oss;
  oss << color << name << " = " << std::forward<T>(arg) << ANSI_RESET;
  if (idx + 1 < sz) oss << ", ";
  return oss;
};

/**
 * @brief   Streams multiple variables with their names and values.
 *
 * @tparam  Args  Variadic variable types
 * @param   color ANSI color code for variable names
 * @param   names Comma-separated string of variable names
 * @param   args  The variable values
 * @return        Formatted string with "name = value" pairs, separated by
 *                commas
 *
 * @details This is the runtime version of stream_args, used by ROS_DEBUG_STREAM
 *          macros. Formats all variables with their names and values.
 */
template <typename... Args>
inline std::string stream_args(std::string_view color, std::string_view names,
                               Args&&... args) {
  std::ostringstream oss;
  auto var_names_{parse_var_str(names)};
  std::size_t idx{0};
  constexpr std::size_t sz{sizeof...(args)};
  ((stream_var(oss, color, var_names_[idx], std::forward<Args>(args), idx, sz),
    idx++),
   ...);
  return oss.str();
};

#endif  // INCLUDE_R2D2_UTILS_PKG_DEBUG_HPP_
