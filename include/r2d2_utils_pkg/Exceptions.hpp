#ifndef R2D2_EXCEPTIONS_HPP
#define R2D2_EXCEPTIONS_HPP

#include <stdexcept>

namespace r2d2_exceptions {

class SafeExceptionConfig {
  static const std::exception_ptr s_exceptionPtr;
};

class Exception : public std::runtime_error {
 public:
  explicit Exception(std::string message) : std::runtime_error(message) {}
};
}  // namespace r2d2_exceptions

#endif  // R2D2_EXCEPTIONS_HPP
