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

constexpr void copy_str(const char* src, char* dst, std::size_t& pos,
                        const std::size_t max_len) {
  for (std::size_t i = 0; src[i] && pos < max_len - 1; ++i) dst[pos++] = src[i];
}
template <std::size_t SrcSize>
constexpr void concat_buffer(const std::array<char, SrcSize>& src, char* dst,
                             std::size_t& pos, const std::size_t max_len) {
  for (std::size_t i = 0; src[i] && pos < max_len - 1; ++i) dst[pos++] = src[i];
}

[[nodiscard]]
constexpr std::size_t num_to_str(std::int64_t value, char* buffer,
                                 std::size_t max_len) {
  const bool negative{value < 0};
  if (negative) value = -value;

  std::size_t idx{max_len - 1};
  do {
    buffer[idx--] = '0' + (value % 10);
    value /= 10;
  } while (value != 0 && idx > 0);

  if (negative && idx > 0) buffer[idx--] = '-';

  // Copy to the beginning of the buffer
  std::size_t pos{0};
  for (std::size_t i = idx + 1; i < max_len; ++i) buffer[pos++] = buffer[i];
  buffer[pos] = '\0';
  return pos;
}

template <std::size_t MaxNames>
[[nodiscard]] constexpr std::array<char[MAX_NAME_LEN], MaxNames> parse_var_str(
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
      result[count][len] = '\0';
      ++count;
    }
    start = i + 1;
  }

  return result;
}

template <typename T>
[[nodiscard]] constexpr std::array<char, MAX_VALUE_LEN> stream_var(
    const char* var_name, const T& var_arg, std::size_t idx, std::size_t sz) {
  std::array<char, MAX_VALUE_LEN> result{};
  std::size_t pos{0};

  copy_str(ANSI_COLOR_YELLOW, result.data(), pos, MAX_VALUE_LEN);
  copy_str(var_name, result.data(), pos, MAX_VALUE_LEN);
  result[pos++] = '=';

  if constexpr (std::is_integral_v<T>) {
    char value_buf[MAX_VALUE_LEN]{};
    const std::size_t value_len{num_to_str(static_cast<std::int64_t>(var_arg),
                                           value_buf, MAX_VALUE_LEN)};

    for (std::size_t i = 0; i < value_len && pos < MAX_VALUE_LEN - 1; ++i)
      result[pos++] = value_buf[i];
  } else if constexpr (std::is_same_v<std::decay_t<T>, const char*>) {
    copy_str(var_arg, result.data(), pos, MAX_VALUE_LEN);
  } else {
    copy_str("<unknown>", result.data(), pos, MAX_VALUE_LEN);
  }

  copy_str(ANSI_COLOR_RESET, result.data(), pos, MAX_VALUE_LEN);
  if (idx + 1 < sz) {
    result[pos++] = ',';
    result[pos++] = ' ';
  }
  result[pos] = '\0';
  return result;
}

template <typename... Args>
[[nodiscard]] constexpr std::array<char, MAX_RESULT_LEN> stream_vars(
    const char* var_str, const Args&... var_args) {
  constexpr std::size_t sz{sizeof...(Args)};
  auto var_names{parse_var_str<sz>(var_str)};

  std::array<char, MAX_RESULT_LEN> result{};
  std::size_t pos{0}, idx{0};

  auto append_var = [&](const auto& arg) {
    concat_buffer(stream_var(var_names[idx], arg, idx, sz), result.data(), pos,
                  MAX_RESULT_LEN);
    ++idx;
  };
  (append_var(var_args), ...);
  result[pos] = '\0';
  return result;
}

template <typename... Args>
constexpr auto stream_args_c(const char* label, const char* names,
                             const Args&... args) {
  std::array<char, MAX_RESULT_LEN> result{};
  std::size_t pos{0};

  if (label && label[0]) {
    result[pos++] = '[';
    copy_str(ANSI_COLOR_MAGENTA, result.data(), pos, MAX_RESULT_LEN);
    copy_str(label, result.data(), pos, MAX_RESULT_LEN);
    copy_str(ANSI_COLOR_RESET, result.data(), pos, MAX_RESULT_LEN);
    result[pos++] = ']';
    result[pos++] = ' ';
    result[pos++] = ':';
    result[pos++] = ' ';
  }
  auto vars{stream_vars(names, args...)};
  concat_buffer(vars, result.data(), pos, MAX_RESULT_LEN);
  result[pos] = '\0';
  auto res{result};
  return res.data();
}

#endif  // INCLUDE_R2D2_UTILS_PKG_DEBUGC_HPP_
