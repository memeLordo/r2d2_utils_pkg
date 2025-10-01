#ifndef R2D2_EXCEPTIONS_HPP
#define R2D2_EXCEPTIONS_HPP

#include <exception>

namespace r2d2_exceptions {

class ExceptionHandler {
  static inline std::exception_ptr s_exceptionPtr{};
  static void check() {
    if (s_exceptionPtr) std::rethrow_exception(s_exceptionPtr);
  };
};

class Exception : public std::runtime_error {
 public:
  explicit Exception(std::string message) : std::runtime_error(message) {}
};
}  // namespace r2d2_exceptions
#endif  // R2D2_EXCEPTIONS_HPP
