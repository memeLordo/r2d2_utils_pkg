#ifndef INCLUDE_R2D2_UTILS_PKG_JSON_HPP_
#define INCLUDE_R2D2_UTILS_PKG_JSON_HPP_

#include <fstream>
#include <nlohmann/json.hpp>
#include <string_view>
#include <unordered_map>

#include "Exceptions.hpp"

namespace r2d2_json {
std::string getFilePath(std::string_view fileName) noexcept;
}  // namespace r2d2_json

template <bool isSafe = false>
class IJsonConfig {
 protected:
  nlohmann::json m_json;

 public:
  explicit IJsonConfig(std::string_view fileName) {
    std::ifstream file{r2d2_json::getFilePath(fileName)};
    if (!file) throw r2d2_errors::json::FileNotFoundError{fileName};
    file >> m_json;
  };

 public:
  template <typename T = double>
  [[nodiscard]] T getParam(std::string_view key) const {
    if (!m_json.contains(key)) throw r2d2_errors::json::ParameterError{key};
    return m_json.at(std::string(key)).template get<T>();
  }
};

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

template <template <typename> class Type, typename T = double>
class IJsonConfigMap : public IJsonConfig<> {
 private:
  std::unordered_map<std::string_view, Type<T>> m_paramsMap;

 public:
  IJsonConfigMap(std::string_view fileName);

 public:
  [[nodiscard]] Type<T> getParams(std::string_view key) const {
    if (auto it = m_paramsMap.find(key); it != m_paramsMap.end())
      return it->second;
    throw r2d2_errors::json::ObjectParseError{key};
  };
};
#endif  // INCLUDE_R2D2_UTILS_PKG_JSON_HPP_
