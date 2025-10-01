#ifndef R2D2_EXCEPTIONS_HPP
#define R2D2_EXCEPTIONS_HPP

#include <exception>

namespace r2d2_exceptions {
class ExceptionStack : std::runtime_error {
 public:
  explicit ExceptionStack()
      : std::runtime_error("ExceptionStack has errors!") {};
};

class ExceptionHandler {
 private:
  static inline std::exception_ptr s_exceptionPtr{};

 public:
  static void check() {
    if (s_exceptionPtr) std::rethrow_exception(s_exceptionPtr);
  };
  template <typename Exception>
  static void record(Exception&& e) {
    s_exceptionStack.emplace(std::forward<Exception>(e));
  };
};

}  // namespace r2d2_exceptions
#endif  // R2D2_EXCEPTIONS_HPP
