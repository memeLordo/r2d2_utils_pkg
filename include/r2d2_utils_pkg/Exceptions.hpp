#ifndef R2D2_EXCEPTIONS_HPP
#define R2D2_EXCEPTIONS_HPP

#include <exception>
#include <stack>

namespace r2d2_exceptions {
class ExceptionStack : std::runtime_error {
 public:
  explicit ExceptionStack()
      : std::runtime_error("ExceptionStack has errors!") {};
};

class ExceptionHandler {
 private:
  static inline std::stack<std::exception_ptr> s_exceptionStack{};

 public:
  static void check() {
    if (!s_exceptionStack.empty()) throw ExceptionStack{};
  };
  template <typename Exception>
  static void record(Exception&& e) {
    s_exceptionStack.emplace(std::forward<Exception>(e));
  };
  static void process_stack() {

    while (!temp.empty()) {
      auto eptr = temp.top();
      temp.pop();

      try {
        std::rethrow_exception(s_exceptionStack.top());
      } catch (const std::exception& e) {
        std::cerr << "Level " << level++ << ": " << typeid(e).name() << " - "
                  << e.what() << '\n';
      } catch (...) {
        std::cerr << "Level " << level++ << ": Unknown exception\n";
      }
    }
  }
};

}  // namespace r2d2_exceptions
#endif  // R2D2_EXCEPTIONS_HPP
