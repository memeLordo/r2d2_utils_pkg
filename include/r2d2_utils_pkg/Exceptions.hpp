#ifndef R2D2_EXCEPTIONS_HPP
#define R2D2_EXCEPTIONS_HPP

#include <exception>
#include <iostream>
#include <stack>

#include "Debug.hpp"

#define CHECK_FOR_STACK_ERRORS() r2d2_errors::stack::check()
#define RECORD_ERROR(error) r2d2_errors::stack::record(error)
#define PRINT_ERROR(msg) r2d2_errors::stack::print_exception(msg)
#define PROCESS_ERROR_STACK() r2d2_errors::stack::process_stack()

namespace r2d2_errors {
class RuntimeErrorStack : public std::runtime_error {
 public:
  explicit RuntimeErrorStack()
      : std::runtime_error("ErrorStack has errors!") {};
};

namespace stack {
namespace etc {
inline std::stack<std::string> exceptionStack{};
}
inline bool has_exceptions() { return !etc::exceptionStack.empty(); };
inline void record(const std::exception& e) noexcept {
  etc::exceptionStack.emplace(e.what());
};
inline void check() {
  if (has_exceptions()) throw RuntimeErrorStack{};
};
inline void print_exception(std::string_view err_msg) noexcept {
  std::cerr << RED("Got exception: " << err_msg) << "\n";
};
inline void process_stack() noexcept {
  while (has_exceptions()) {
    print_exception(etc::exceptionStack.top());
    etc::exceptionStack.pop();
  }
};
}  // namespace stack

}  // namespace r2d2_errors
#endif  // R2D2_EXCEPTIONS_HPP
