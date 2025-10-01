#ifndef R2D2_EXCEPTIONS_HPP
#define R2D2_EXCEPTIONS_HPP

namespace r2d2_exceptions {

class Exception : public std::runtime_error {
 public:
  explicit Exception(std::string message) : std::runtime_error(message) {}
};
}  // namespace r2d2_exceptions

#endif  // R2D2_EXCEPTIONS_HPP
