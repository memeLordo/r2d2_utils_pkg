#ifndef R2D2_EXCEPTIONS_HPP
#define R2D2_EXCEPTIONS_HPP

#include <exception>

namespace r2d2_exceptions {

class ExceptionHandler {
  static inline std::exception_ptr s_exceptionPtr{};
  static void check() {
    if (s_exceptionPtr) std::rethrow_exception(s_exceptionPtr);
  };
  template <typename Exception>
  static void record(Exception&& e) {
    if (s_exceptionPtr) {
      try {
        std::rethrow_exception(s_exceptionPtr);
      } catch (...) {
        try {
          std::throw_with_nested(std::forward<Exception>(e));
        } catch (...) {
          s_exceptionPtr = std::current_exception();
        }
      }
    }
    try {
      throw std::forward<Exception>(e);
    } catch (...) {
      s_exceptionPtr = std::current_exception();
    }
  }
};

class Exception : public std::runtime_error {
 public:
  explicit Exception(std::string message) : std::runtime_error(message) {}
};
}  // namespace r2d2_exceptions
#endif  // R2D2_EXCEPTIONS_HPP
