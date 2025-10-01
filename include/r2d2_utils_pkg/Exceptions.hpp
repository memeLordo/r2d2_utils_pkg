#ifndef R2D2_EXCEPTIONS_HPP
#define R2D2_EXCEPTIONS_HPP

#include <exception>
#include <iostream>
#include <stack>

#include "Debug.hpp"

namespace r2d2_errors {
class RuntimeErrorStack : public std::runtime_error {
 public:
  explicit RuntimeErrorStack()
      : std::runtime_error("ExceptionStack has errors!") {};
};

class ExceptionHandler {
 private:
  static inline std::stack<std::string> s_exceptionStack{};

 public:
  static void check() {
    if (has_exceptions()) throw RuntimeErrorStack{};
  };
  static void record(const std::exception& e) {
    s_exceptionStack.emplace(e.what());
  };
  static void process_stack() {
    while (has_exceptions()) {
      print_exception(s_exceptionStack.top());
      s_exceptionStack.pop();
    }
  };
  static void print_exception(std::string_view err_msg) {
    std::cerr << RED("Got exception: " << err_msg) << "\n";
  };
  static bool has_exceptions() { return !s_exceptionStack.empty(); };
};
}  // namespace r2d2_errors

#define CHECK_FOR_STACK_ERRORS() r2d2_errors::ExceptionHandler::check()
#define RECORD_ERROR(exception) r2d2_errors::ExceptionHandler::record(exception)
#define PROCESS_ERROR_STACK() r2d2_errors::ExceptionHandler::process_stack()

#endif  // R2D2_EXCEPTIONS_HPP
