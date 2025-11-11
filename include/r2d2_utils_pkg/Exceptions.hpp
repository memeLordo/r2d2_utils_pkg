#ifndef INCLUDE_R2D2_UTILS_PKG_EXCEPTIONS_HPP_
#define INCLUDE_R2D2_UTILS_PKG_EXCEPTIONS_HPP_

#include <exception>
#include <iostream>
#include <queue>

#include "Logging/Console.hpp"

#define CHECK_FOR_ERROR_RECORD() r2d2_errors::agent::check()
#define RECORD_ERROR(error) r2d2_errors::agent::record(error)
#define PRINT_ERROR(msg) r2d2_errors::agent::print_error(msg)
#define PROCESS_ERROR_RECORD() r2d2_errors::agent::process_errors()

namespace r2d2_errors {
/**
 * @brief Base class for error types that concatenate multiple strings into a message.
 * @tparam Error The underlying error type to inherit from
 * @details Provides functionality to create error messages by concatenating multiple string arguments.
 */
template <typename Error>
class BaseError : public Error {
 protected:
  /**
   * @brief Constructs a BaseError with a concatenated message from multiple strings.
   * @tparam String Variadic string types
   * @param str Variadic list of strings to concatenate into the error message
   */
  template <typename... String>
  explicit BaseError(String&&... str)
      : Error{createMessage(std::forward<String>(str)...)} {};

 private:
  template <typename... String>
  static std::string createMessage(String&&... str) noexcept {
    std::string result_;
    result_.reserve((string_size(str) + ...));
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
/**
 * @brief Exception thrown when error record contains errors.
 */
struct RecordNotEmptyError final : public std::runtime_error {
  /**
   * @brief Constructs a RecordNotEmptyError.
   */
  explicit RecordNotEmptyError()
      : std::runtime_error("ErrorRecord has errors!") {};
};

namespace etc {
inline std::queue<std::string> errorQueue{};
}
/**
 * @brief Checks if there are any errors in the error queue.
 * @return True if errors exist, false otherwise
 */
inline bool has_errors() noexcept { return !etc::errorQueue.empty(); };
/**
 * @brief Checks for errors and throws RecordNotEmptyError if any exist.
 * @throws RecordNotEmptyError if the error queue is not empty
 */
inline void check() {
  if (has_errors()) throw RecordNotEmptyError{};
};
/**
 * @brief Records an exception in the error queue.
 * @param e The exception to record
 */
inline void record(const std::exception& e) noexcept {
  etc::errorQueue.emplace(e.what());
};
/**
 * @brief Prints an error message to stderr.
 * @param err_msg The error message to print
 */
inline void print_error(std::string_view err_msg) noexcept {
  std::cerr << RED("Got exception: " << err_msg) << "\n";
};
/**
 * @brief Processes all errors in the queue by printing and removing them.
 * @details Prints each error message and removes it from the queue until empty.
 */
inline void process_errors() noexcept {
  while (has_errors()) {
    print_error(etc::errorQueue.front());
    etc::errorQueue.pop();
  }
};
}  // namespace r2d2_errors::agent

namespace r2d2_errors::collections {
/**
 * @brief Exception thrown when a name is not found in a collection.
 */
struct NameError final : public BaseError<std::out_of_range> {
  /**
   * @brief Constructs a NameError for the specified name.
   * @param name The name that was not found
   */
  explicit NameError(std::string_view name)
      : BaseError("Name \"", name, "\" is not found!") {};
};
}  // namespace r2d2_errors::collections

namespace r2d2_errors::json {
/**
 * @brief Exception thrown when a JSON file is not found.
 */
struct FileNotFoundError final : public BaseError<std::runtime_error> {
  /**
   * @brief Constructs a FileNotFoundError for the specified file name.
   * @param fileName The name of the file that was not found
   */
  explicit FileNotFoundError(std::string_view fileName)
      : BaseError("File \"", fileName, ".json\"is not found!") {};
};
/**
 * @brief Exception thrown when a JSON parameter is not found.
 */
struct ParameterError final : public BaseError<std::runtime_error> {
  /**
   * @brief Constructs a ParameterError for the specified parameter key.
   * @param key The parameter key that was not found
   */
  explicit ParameterError(std::string_view key)
      : BaseError("Parameter \"", key, "\" is not found!") {};
};
/**
 * @brief Exception thrown when a JSON object is not found during parsing.
 */
struct ObjectParseError final : public BaseError<std::runtime_error> {
  /**
   * @brief Constructs an ObjectParseError for the specified object key.
   * @param key The object key that was not found
   */
  explicit ObjectParseError(std::string_view key)
      : BaseError("Object \"", key, "\" is not found!") {};
};
}  // namespace r2d2_errors::json
#endif  // INCLUDE_R2D2_UTILS_PKG_EXCEPTIONS_HPP_
