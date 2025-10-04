#ifndef R2D2_EXCEPTIONS_HPP
#define R2D2_EXCEPTIONS_HPP

#include <exception>
#include <iostream>
#include <queue>

#include "Debug.hpp"

#define CHECK_FOR_ERROR_RECORD() r2d2_errors::collector::check()
#define RECORD_ERROR(error) r2d2_errors::collector::record(error)
#define PRINT_ERROR(msg) r2d2_errors::collector::print_error(msg)
#define PROCESS_ERROR_RECORD() r2d2_errors::collector::process_errors()

namespace r2d2_errors::collector {
struct RuntimeErrorRecord : public std::runtime_error {
  explicit RuntimeErrorRecord()
      : std::runtime_error("ErrorRecord has errors!") {};
};

namespace etc {
inline std::queue<std::string> errorQueue{};
}
inline bool has_errors() noexcept { return !etc::errorQueue.empty(); };
inline void check() {
  if (has_errors()) throw RuntimeErrorRecord{};
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
}  // namespace r2d2_errors::collector

namespace r2d2_errors::json {
struct FileNotFoundError : public std::runtime_error {
  explicit FileNotFoundError(std::string_view fileName)
      : std::runtime_error("File \"" + std::string{fileName} +
                           ".json\" not "
                           "found!") {};
};
struct ParseError : public std::runtime_error {
  explicit ParseError(std::string_view key)
      : std::runtime_error("Parameter \"" + std::string{key} +
                           "\" not found!") {};
};
struct ObjectParseError : public std::runtime_error {
  explicit ObjectParseError(std::string_view key)
      : std::runtime_error("Object \"" + std::string{key} + "\" not found!") {};
};
}  // namespace r2d2_errors::json
#endif  // R2D2_EXCEPTIONS_HPP
