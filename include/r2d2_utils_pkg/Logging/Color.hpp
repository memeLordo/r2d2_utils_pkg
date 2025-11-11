#ifndef INCLUDE_R2D2_UTILS_PKG_COLOR_HPP_
#define INCLUDE_R2D2_UTILS_PKG_COLOR_HPP_

#include <array>
#include <string_view>

#include "Console.hpp"

namespace r2d2_console {

enum class ColorPreset { DEFAULT = 0, BOOL, ENUM, RESULT };

struct ColorArg {
  std::string_view name;
  std::string_view value;
};
struct ColorType {
  ColorArg arg;
  std::string_view name;
  std::string_view function;
  std::string_view result;
};

// Nested aggregate initialization in constexpr array
constexpr std::array<ColorType, 4> COLOR_PRESETS{
    {{{ANSI_WHITE, ANSI_WHITE}, ANSI_RESET, ANSI_RESET, ANSI_WHITE},
     {{ANSI_BLUE, ANSI_YELLOW}, ANSI_BLUE, ANSI_BLUE, ANSI_YELLOW},
     {{ANSI_CYAN, ANSI_CYAN}, ANSI_CYAN, ANSI_CYAN, ANSI_CYAN},
     {{ANSI_YELLOW, ANSI_RED}, ANSI_MAGENTA, ANSI_BLUE, ANSI_YELLOW}}};

/**
 * @brief   Factory function to get a color preset configuration.
 *
 * @param   preset The color preset to retrieve
 * @return         The ColorType configuration for the specified preset
 */
constexpr ColorType paint(ColorPreset preset) noexcept {
  return COLOR_PRESETS[static_cast<std::size_t>(preset)];
};

}  // namespace r2d2_console

#endif  // INCLUDE_R2D2_UTILS_PKG_COLOR_HPP_
