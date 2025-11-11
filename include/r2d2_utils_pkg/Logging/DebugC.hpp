#ifndef INCLUDE_R2D2_UTILS_PKG_DEBUGC_HPP_
#define INCLUDE_R2D2_UTILS_PKG_DEBUGC_HPP_

#include <array>
#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "Console.hpp"

constexpr std::size_t MAX_NAME_LEN{64};
constexpr std::size_t MAX_VALUE_LEN{256};
constexpr std::size_t MAX_RESULT_LEN{1024};

/**
 * @brief Appends a character to a buffer at the current position.
 * @param chr The character to append (ignored if null terminator)
 * @param dst The destination buffer
 * @param pos Current position in the buffer (incremented after append)
 * @details Skips null terminators to avoid corrupting the string.
 */
constexpr void append_chr(char chr, char* dst, std::size_t& pos) {
  if (chr == '\0') return;
  dst[pos++] = chr;
}
/**
 * @brief Pushes a character backwards into a buffer (for reverse string
 * building).
 * @param chr The character to push
 * @param dst The destination buffer
 * @param pos Current position in the buffer (decremented after push)
 */
constexpr void push_chr(char chr, char* dst, std::size_t& pos) {
  dst[pos--] = chr;
}
/**
 * @brief Null-terminates a string at the current position.
 * @param dst The destination buffer
 * @param pos Current position in the buffer (where null terminator is placed)
 */
constexpr void end_str(char* dst, std::size_t& pos) { dst[pos] = '\0'; }

/**
 * @brief Converts a number to a null-terminated string in a buffer.
 * @param value The integer value to convert
 * @param buffer The buffer to write the string to
 * @return The length of the resulting string (excluding null terminator)
 * @details Handles negative numbers and builds the string in reverse before
 * copying forward.
 */
[[nodiscard]]
constexpr std::size_t num_to_str(std::int64_t value, char* buffer) {
  const bool negative{value < 0};
  if (negative) value = -value;

  std::size_t idx{MAX_VALUE_LEN - 1};
  do {
    push_chr('0' + (value % 10), buffer, idx);
    value /= 10;
  } while (value != 0 && idx > 0);

  if (negative && idx > 0) push_chr('-', buffer, idx);

  // Copy to the beginning of the buffer
  std::size_t pos{0};
  for (std::size_t i = idx + 1; i < MAX_VALUE_LEN; ++i)
    append_chr(buffer[i], buffer, pos);
  end_str(buffer, pos);
  return pos;
}

/**
 * @brief Copies a null-terminated string to a buffer at the current position.
 * @param src The source null-terminated string
 * @param dst The destination buffer
 * @param pos Current position in the destination buffer (updated after copy)
 * @details Copies characters until null terminator or buffer limit is reached.
 */
constexpr void copy_str(const char* src, char* dst, std::size_t& pos) {
  for (std::size_t i = 0; src[i] && pos < MAX_VALUE_LEN - 1; ++i)
    append_chr(src[i], dst, pos);
}
/**
 * @brief Copies an argument value to a buffer as a string representation.
 * @tparam T The argument type
 * @param var_arg The argument value to convert
 * @param dst The destination buffer
 * @param pos Current position in the destination buffer (updated after copy)
 * @details Converts numeric/enum types to string, handles other types as
 * "<unknown>".
 */
template <typename T>
constexpr void copy_arg(T&& var_arg, char* dst, std::size_t& pos) {
  char value_buf[MAX_VALUE_LEN]{};
  const std::size_t value_len{
      num_to_str(static_cast<std::int64_t>(var_arg), value_buf)};
  for (std::size_t i = 0; i < value_len && pos < MAX_VALUE_LEN - 1; ++i)
    append_chr(value_buf[i], dst, pos);
}
/**
 * @brief Concatenates a source buffer array to a destination buffer.
 * @tparam SrcSize The size of the source array
 * @param src The source array to copy from
 * @param dst The destination buffer
 * @param pos Current position in the destination buffer (updated after copy)
 * @details Copies characters until null terminator or buffer limit is reached.
 */
template <std::size_t SrcSize>
constexpr void concat_buff(const std::array<char, SrcSize>& src, char* dst,
                           std::size_t& pos) {
  for (std::size_t i = 0; src[i] && pos < MAX_VALUE_LEN - 1; ++i)
    append_chr(src[i], dst, pos);
}

/**
 * @brief Parses a variable string into an array of variable names at compile
 * time.
 * @tparam MaxNames Maximum number of variable names to parse
 * @param var_str Comma-separated null-terminated string of variable names
 * @return Array of null-terminated variable name strings
 * @details Splits the input string by commas and spaces, extracting individual
 * variable names. Returns empty strings for empty variable names (indicated by
 * "").
 */
template <std::size_t MaxNames>
[[nodiscard]]
constexpr std::array<char[MAX_NAME_LEN], MaxNames> parse_var_str(
    const char* var_str) {
  std::array<char[MAX_NAME_LEN], MaxNames> result{};
  std::size_t count{0}, start{0}, str_len{0};

  while (var_str[str_len] != '\0') ++str_len;

  for (std::size_t i = 0; i <= str_len && count < MaxNames; ++i) {
    if (i < str_len && var_str[i] != ',' && var_str[i] != ' ') continue;

    while (start < i && (var_str[start] == ' ' || var_str[start] == ','))
      ++start;

    if (start < i && (i - start) < MAX_NAME_LEN) {
      std::size_t len = i - start;
      for (std::size_t j = 0; j < len; ++j)
        result[count][j] = var_str[start + j];
      end_str(result[count], len);
      ++count;
    }
    start = i + 1;
  }

  return result;
}

/**
 * @brief Creates a formatted string for a single variable at compile time.
 * @tparam T The variable type
 * @param color ANSI color code for the variable name
 * @param var_name The variable name (null-terminated string)
 * @param var_arg The variable value
 * @param idx Current index in the variable list
 * @param sz Total number of variables
 * @return Array containing the formatted "name = value" string
 * @details Formats as "color name = value ANSI_GREEN" with comma separator if
 * not last. Returns empty string if var_name is "".
 */
template <typename T>
[[nodiscard]] constexpr std::array<char, MAX_VALUE_LEN> stream_var(
    const char* color, const char* var_name, const T& var_arg, std::size_t idx,
    std::size_t sz) {
  std::array<char, MAX_VALUE_LEN> result{};
  std::size_t pos{0};

  if (var_name[0] == '"' && var_name[1] == '"') {
    end_str(result.data(), pos);
    return result;
  }

  copy_str(color, result.data(), pos);
  copy_str(var_name, result.data(), pos);
  copy_str(" = ", result.data(), pos);

  if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>) {
    copy_arg(var_arg, result.data(), pos);
  } else if constexpr (std::is_same_v<std::decay_t<T>, const char*>) {
    copy_str(var_arg, result.data(), pos);
  } else {
    copy_str("<unknown>", result.data(), pos);
  }

  copy_str(ANSI_GREEN, result.data(), pos);
  if (idx + 1 < sz) {
    append_chr(',', result.data(), pos);
    append_chr(' ', result.data(), pos);
  }
  end_str(result.data(), pos);
  return result;
}

/**
 * @brief Creates a formatted string for multiple variables at compile time.
 * @tparam Args Variadic variable types
 * @param color ANSI color code for variable names
 * @param var_str Comma-separated null-terminated string of variable names
 * @param var_args The variable values
 * @return Array containing the formatted string with all "name = value" pairs
 * @details Concatenates formatted strings for all variables with proper
 * separators.
 */
template <typename... Args>
[[nodiscard]] constexpr std::array<char, MAX_RESULT_LEN> stream_vars(
    const char* color, const char* var_str, const Args&... var_args) {
  constexpr std::size_t sz{sizeof...(Args)};
  auto var_names{parse_var_str<sz>(var_str)};

  std::array<char, MAX_RESULT_LEN> result{};
  std::size_t pos{0}, idx{0};

  auto append_var = [&](const auto& arg) {
    concat_buff(stream_var(color, var_names[idx], arg, idx, sz), result.data(),
                pos);
    ++idx;
  };
  (append_var(var_args), ...);
  end_str(result.data(), pos);
  return result;
}

/**
 * @brief Main compile-time function to create a formatted string with variable
 * names and values.
 * @tparam Args Variadic variable types
 * @param color ANSI color code for variable names
 * @param names Comma-separated null-terminated string of variable names
 * @param args The variable values
 * @return Array containing the formatted string with "name = value" pairs
 * @details This is the compile-time (constexpr) version of stream_args, used by
 * ROS_DEBUG macros that require compile-time evaluation. Formats all variables
 * with their names and values.
 */
template <typename... Args>
constexpr auto stream_args_c(const char* color, const char* names,
                             const Args&... args) {
  std::array<char, MAX_RESULT_LEN> result{};
  std::size_t pos{0};

  auto vars{stream_vars(color, names, args...)};
  concat_buff(vars, result.data(), pos);
  end_str(result.data(), pos);
  return result;
}

#endif  // INCLUDE_R2D2_UTILS_PKG_DEBUGC_HPP_
