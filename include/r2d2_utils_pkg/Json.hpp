#ifndef INCLUDE_R2D2_UTILS_PKG_JSON_HPP_
#define INCLUDE_R2D2_UTILS_PKG_JSON_HPP_

#include <fstream>
#include <nlohmann/json.hpp>
#include <string_view>
#include <unordered_map>

#include "Exceptions.hpp"

namespace r2d2_json {
/**
 * @brief Gets the full file path for a configuration file.
 * @param fileName The name of the configuration file (without extension)
 * @return The full path to the configuration file
 */
std::string getFilePath(std::string_view fileName) noexcept;
}  // namespace r2d2_json

/**
 * @brief Base class for JSON configuration loading.
 * @tparam isSafe If true, errors are recorded instead of thrown (default: false)
 * @details Loads a JSON file and provides access to its parameters.
 */
template <bool isSafe = false>
class IJsonConfig {
 protected:
  nlohmann::json m_json;

 public:
  /**
   * @brief Constructs an IJsonConfig and loads the specified JSON file.
   * @param fileName The name of the JSON configuration file
   * @throws r2d2_errors::json::FileNotFoundError if the file cannot be opened
   */
  explicit IJsonConfig(std::string_view fileName) {
    std::ifstream file{r2d2_json::getFilePath(fileName)};
    if (!file) throw r2d2_errors::json::FileNotFoundError{fileName};
    file >> m_json;
  };

 public:
  /**
   * @brief Gets a parameter value from the JSON configuration.
   * @tparam T The type to retrieve the parameter as (default: double)
   * @param key The parameter key
   * @return The parameter value
   * @throws r2d2_errors::json::ParameterError if the key is not found
   */
  template <typename T = double>
  [[nodiscard]] T getParam(std::string_view key) const {
    if (!m_json.contains(key)) throw r2d2_errors::json::ParameterError{key};
    return m_json.at(std::string(key)).template get<T>();
  }
};

/**
 * @brief Specialized constructor for safe mode that records errors instead of throwing.
 * @param fileName The name of the JSON configuration file
 * @details Errors are recorded in the error queue instead of being thrown.
 */
template <>
inline IJsonConfig<true>::IJsonConfig(std::string_view fileName) {
  try {
    std::ifstream file{r2d2_json::getFilePath(fileName)};
    if (!file) throw r2d2_errors::json::FileNotFoundError{fileName};
    file >> m_json;
  } catch (const std::exception& e) {
    RECORD_ERROR(e);
  }
};
/**
 * @brief Specialized getParam for safe mode that records errors instead of throwing.
 * @tparam T The type to retrieve the parameter as
 * @param key The parameter key
 * @return The parameter value, or default-constructed T if key is not found
 * @details Errors are recorded in the error queue instead of being thrown.
 */
template <>
template <typename T>
[[nodiscard]]
inline T IJsonConfig<true>::getParam(std::string_view key) const {
  try {
    if (!m_json.contains(key)) throw r2d2_errors::json::ParameterError{key};
    return m_json.at(std::string(key)).template get<T>();
  } catch (const std::exception& e) {
    RECORD_ERROR(e);
    return T{};
  }
};

/**
 * @brief Extended JSON configuration class that maps keys to typed objects.
 * @tparam Type The configuration type template
 * @tparam T Numeric type for the configuration values (default: double)
 * @details Loads JSON and deserializes entries into a map of configuration objects.
 */
template <template <typename> class Type, typename T = double>
class IJsonConfigMap : public IJsonConfig<> {
 private:
  std::unordered_map<std::string_view, Type<T>> m_paramsMap;

 public:
  /**
   * @brief Constructs an IJsonConfigMap and loads all parameters from JSON.
   * @param fileName The name of the JSON configuration file
   */
  IJsonConfigMap(std::string_view fileName);

 public:
  /**
   * @brief Gets a configuration object by key.
   * @param key The configuration key
   * @return The configuration object of type Type<T>
   * @throws r2d2_errors::json::ObjectParseError if the key is not found
   */
  [[nodiscard]] Type<T> getParams(std::string_view key) const {
    if (auto it = m_paramsMap.find(key); it != m_paramsMap.end())
      return it->second;
    throw r2d2_errors::json::ObjectParseError{key};
  };
};
#endif  // INCLUDE_R2D2_UTILS_PKG_JSON_HPP_
