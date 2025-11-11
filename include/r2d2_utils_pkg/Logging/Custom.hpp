#ifndef INCLUDE_R2D2_UTILS_PKG_CUSTOM_HPP_
#define INCLUDE_R2D2_UTILS_PKG_CUSTOM_HPP_

#include <ros/console.h>

#include "Color.hpp"   // IWYU pragma: keep
#include "Debug.hpp"   // IWYU pragma: export
#include "DebugC.hpp"  // IWYU pragma: export

template <typename T>
std::string paint_value(std::string color, T value) {
  return color + std::to_string(value);
}

#define ROS_DEBUG_VARS(...) \
  ROS_DEBUG("%s", stream_args(ANSI_WHITE, #__VA_ARGS__, __VA_ARGS__).c_str())
#define ROS_DEBUG_VOID(...)     \
  ROS_DEBUG("%s(%s)", __func__, \
            stream_args(ANSI_WHITE, #__VA_ARGS__, __VA_ARGS__).c_str())
#define ROS_DEBUG_FUNC(output, ...)                                     \
  ROS_DEBUG("%s(%s) : %s", __func__,                                    \
            stream_args(ANSI_WHITE, #__VA_ARGS__, __VA_ARGS__).c_str(), \
            paint_value(ANSI_MAGENTA, output).c_str())

#define ROS_DEBUG_VARS_C(...) \
  ROS_DEBUG("%s", stream_args_c(ANSI_WHITE, #__VA_ARGS__, __VA_ARGS__).data())
#define ROS_DEBUG_VOID_C(...)   \
  ROS_DEBUG("%s(%s)", __func__, \
            stream_args_c(ANSI_WHITE, #__VA_ARGS__, __VA_ARGS__).data())
#define ROS_DEBUG_FUNC_C(output, ...)                                    \
  ROS_DEBUG("%s(%s) : %s", __func__,                                     \
            stream_args_c(ANSI_WHITE, #__VA_ARGS__, __VA_ARGS__).data(), \
            paint_value(ANSI_MAGENTA, output).c_str())

#define ROS_DEBUG_NAMED_VARS(name, ...) \
  ROS_DEBUG("[%s] %s", name.c_str(),    \
            stream_args(ANSI_WHITE, #__VA_ARGS__, __VA_ARGS__).c_str())
#define ROS_DEBUG_NAMED_VOID(name, ...)            \
  ROS_DEBUG("[%s] %s(%s)", name.c_str(), __func__, \
            stream_args(ANSI_WHITE, #__VA_ARGS__, __VA_ARGS__).c_str())
#define ROS_DEBUG_NAMED_FUNC(name, output, ...)                         \
  ROS_DEBUG("[%s] %s(%s) : %s", name.c_str(), __func__,                 \
            stream_args(ANSI_WHITE, #__VA_ARGS__, __VA_ARGS__).c_str(), \
            paint_value(ANSI_MAGENTA, output).c_str())

#define ROS_DEBUG_NAMED_VARS_C(name, ...) \
  ROS_DEBUG("[%s] %s", name.c_str(),      \
            stream_args_c(ANSI_WHITE, #__VA_ARGS__, __VA_ARGS__).data())
#define ROS_DEBUG_NAMED_VOID_C(name, ...)          \
  ROS_DEBUG("[%s] %s(%s)", name.c_str(), __func__, \
            stream_args_c(ANSI_WHITE, #__VA_ARGS__, __VA_ARGS__).data())
#define ROS_DEBUG_NAMED_FUNC_C(name, output, ...)                        \
  ROS_DEBUG("[%s] %s(%s) : %s", name.c_str(), __func__,                  \
            stream_args_c(ANSI_WHITE, #__VA_ARGS__, __VA_ARGS__).data(), \
            paint_value(ANSI_MAGENTA, output).c_str())

#define ROS_DEBUG_COLORED_VARS(color, ...) \
  ROS_DEBUG("%s", stream_args(color, #__VA_ARGS__, __VA_ARGS__).c_str())
#define ROS_DEBUG_COLORED_VOID(color, ...) \
  ROS_DEBUG("%s(%s)", __func__,            \
            stream_args(color, #__VA_ARGS__, __VA_ARGS__).c_str())
#define ROS_DEBUG_COLORED_FUNC(color, output, ...)                 \
  ROS_DEBUG("%s(%s) : %s", __func__,                               \
            stream_args(color, #__VA_ARGS__, __VA_ARGS__).c_str(), \
            paint_value(ANSI_MAGENTA, output).c_str())

#define ROS_DEBUG_COLORED_VARS_C(color, ...) \
  ROS_DEBUG("%s", stream_args_c(color, #__VA_ARGS__, __VA_ARGS__).data())
#define ROS_DEBUG_COLORED_VOID_C(color, ...) \
  ROS_DEBUG("%s(%s)", __func__,              \
            stream_args_c(color, #__VA_ARGS__, __VA_ARGS__).data())
#define ROS_DEBUG_COLORED_FUNC_C(color, output, ...)                \
  ROS_DEBUG("%s(%s) : %s", __func__,                                \
            stream_args_c(color, #__VA_ARGS__, __VA_ARGS__).data(), \
            paint_value(ANSI_MAGENTA, output).c_str())

#define ROS_DEBUG_NAMED_COLORED_VARS(name, color, ...) \
  ROS_DEBUG("[%s] %s", name.c_str(),                   \
            stream_args(color, #__VA_ARGS__, __VA_ARGS__).c_str())
#define ROS_DEBUG_NAMED_COLORED_VOID(name, color, ...) \
  ROS_DEBUG("[%s] %s(%s)", name.c_str(), __func__,     \
            stream_args(color, #__VA_ARGS__, __VA_ARGS__).c_str())
#define ROS_DEBUG_NAMED_COLORED_FUNC(name, color, output, ...)     \
  ROS_DEBUG("[%s] %s(%s) : %s", name.c_str(), __func__,            \
            stream_args(color, #__VA_ARGS__, __VA_ARGS__).c_str(), \
            paint_value(ANSI_MAGENTA, output).c_str())

#define ROS_DEBUG_NAMED_COLORED_VARS_C(name, color, ...) \
  ROS_DEBUG("[%s] %s", name.c_str(),                     \
            stream_args_c(color, #__VA_ARGS__, __VA_ARGS__).data())
#define ROS_DEBUG_NAMED_COLORED_VOID_C(name, color, ...) \
  ROS_DEBUG("[%s] %s(%s)", name.c_str(), __func__,       \
            stream_args_c(color, #__VA_ARGS__, __VA_ARGS__).data())
#define ROS_DEBUG_NAMED_COLORED_FUNC_C(name, color, output, ...)    \
  ROS_DEBUG("[%s] %s(%s) : %s", name.c_str(), __func__,             \
            stream_args_c(color, #__VA_ARGS__, __VA_ARGS__).data(), \
            paint_value(ANSI_MAGENTA, output).c_str())

#endif  // INCLUDE_R2D2_UTILS_PKG_CUSTOM_HPP_
