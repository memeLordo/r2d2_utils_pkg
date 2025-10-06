#ifndef R2D2_EXCEPTIONS_HPP
#define R2D2_EXCEPTIONS_HPP

#include <exception>
#include <iostream>
#include <queue>

#include "Debug.hpp"

#define CHECK_FOR_ERROR_RECORD() r2d2_errors::agent::check()
#define RECORD_ERROR(error) r2d2_errors::agent::record(error)
#define PRINT_ERROR(msg) r2d2_errors::agent::print_error(msg)
#define PROCESS_ERROR_RECORD() r2d2_errors::agent::process_errors()

namespace r2d2_errors {
template <typename Error>
class BaseError : public Error {
 protected:
  template <typename... String>
  explicit BaseError(String&&... str)
      : Error{createMessage(std::forward<String>(str)...)} {};

 private:
  template <typename... String>
  static std::string createMessage(String&&... str) {
    size_t totalSize_{(std::size(str) + ...)};

    std::string result_;
    result_.reserve(totalSize_);
    (result_.append(std::forward<String>(str)), ...);
    return result_;
  };
  template <typename T, size_t N>
  static constexpr size_t string_size(const T (&)[N]) noexcept {
    return N - 1;
  };
  template <typename T>
  static constexpr size_t string_size(T&& str) noexcept {
    return std::size(str);
  };
};
}  // namespace r2d2_errors

namespace r2d2_errors::agent {
struct RecordNotEmptyError : public std::runtime_error {
  explicit RecordNotEmptyError()
      : std::runtime_error("ErrorRecord has errors!") {};
};

namespace etc {
inline std::queue<std::string> errorQueue{};
}
inline bool has_errors() noexcept { return !etc::errorQueue.empty(); };
inline void check() {
  if (has_errors()) throw RecordNotEmptyError{};
};
inline void record(const std::exception& e) noexcept {
  etc::errorQueue.emplace(e.what());
};
inline void print_error(std::string_view err_msg) noexcept {
  std::cerr << RED("Got exception: " << err_msg) << "\n";
};
inline void process_errors() noexcept {
  while (has_errors()) {
    print_error(etc::errorQueue.front());
    etc::errorQueue.pop();
  }
};
}  // namespace r2d2_errors::agent

namespace r2d2_errors::json {
struct FileNotFoundError : public BaseError<std::runtime_error> {
  explicit FileNotFoundError(std::string_view fileName)
      : BaseError("File \"", fileName, ".json\" not found!") {};
};
struct ParameterError : public BaseError<std::runtime_error> {
  explicit ParameterError(std::string_view key)
      : BaseError("Parameter \"", key, "\" not found!") {};
};
struct ObjectParseError : public BaseError<std::runtime_error> {
  explicit ObjectParseError(std::string_view key)
      : BaseError("Object \"", key, "\" not found!") {};
};
}  // namespace r2d2_errors::json
#endif  // R2D2_EXCEPTIONS_HPP
