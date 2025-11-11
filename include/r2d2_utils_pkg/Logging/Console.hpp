#ifndef INCLUDE_R2D2_UTILS_PKG_CONSOLE_HPP_
#define INCLUDE_R2D2_UTILS_PKG_CONSOLE_HPP_

#define ANSI_RED "\033[0;31m"
#define ANSI_GREEN "\033[0;32m"
#define ANSI_YELLOW "\033[0;33m"
#define ANSI_BLUE "\033[0;34m"
#define ANSI_MAGENTA "\033[0;35m"
#define ANSI_CYAN "\033[0;36m"
#define ANSI_WHITE "\033[0;37m"
#define ANSI_NONE "\033[0m"

#define ANSI_RESET ANSI_GREEN

// Color stream
#define RED(x) ANSI_RED << x << ANSI_RESET
#define GREEN(x) ANSI_GREEN << x << ANSI_RESET
#define YELLOW(x) ANSI_YELLOW << x << ANSI_RESET
#define BLUE(x) ANSI_BLUE << x << ANSI_RESET
#define MAGENTA(x) ANSI_MAGENTA << x << ANSI_RESET
#define CYAN(x) ANSI_CYAN << x << ANSI_RESET
#define WHITE(x) ANSI_WHITE << x << ANSI_RESET

#endif  // INCLUDE_R2D2_UTILS_PKG_CONSOLE_HPP_
