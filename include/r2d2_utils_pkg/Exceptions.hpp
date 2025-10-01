#ifndef R2D2_EXCEPTIONS_HPP
#define R2D2_EXCEPTIONS_HPP

#include <exception>
#include <stack>

#define CHECK_FOR_ERRORS() r2d2_errors::ExceptionHandler::check()
#define RECORD_ERROR(exception) r2d2_errors::ExceptionHandler::record(exception)
#define PROCESS_ERROR_STACK() r2d2_errors::ExceptionHandler::process_stack()

namespace r2d2_errors {
class ExceptionStack : public std::runtime_error {
 public:
  explicit ExceptionStack()
      : std::runtime_error("ExceptionStack has errors!") {};
};

class ExceptionHandler {
 private:
  static inline std::stack<std::exception_ptr> s_exceptionStack{};

 public:
  static void check() {
    if (has_exceptions()) throw ExceptionStack{};
  };
  template <typename Exception>
  static void record(Exception&& e) {
    s_exceptionStack.emplace(std::forward<Exception>(e));
  };
  static void process_stack() {
    while (!s_exceptionStack.empty()) try {
        std::rethrow_exception(s_exceptionStack.top());
      } catch (const std::exception& e) {
        print_exception(e);
        s_exceptionStack.pop();
      }
  };
  static bool has_exceptions() { return !s_exceptionStack.empty(); };
  static void print_exception(const std::exception& e);
};
}  // namespace r2d2_errors
#endif  // R2D2_EXCEPTIONS_HPP
