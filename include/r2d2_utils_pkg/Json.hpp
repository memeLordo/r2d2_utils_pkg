#ifndef R2D2_CONFIG_JSON_HPP
#define R2D2_CONFIG_JSON_HPP

#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string_view>
#include <unordered_map>

#include "r2d2_utils_pkg/Exceptions.hpp"

namespace r2d2_json {
std::string getFilePath(std::string_view fileName);
}  // namespace r2d2_json

template <typename T = double, bool isSafe = false>
class IJsonConfig {
 protected:
  nlohmann::json m_json;

 public:
  explicit IJsonConfig(std::string_view fileName) {
    try {
      std::ifstream file{r2d2_json::getFilePath(fileName)};
      if (!file)
        RECORD_ERROR(std::runtime_error(
            {"File \"" + std::string{fileName} + ".json\" not found!"}));
      file >> m_json;
    } catch (const std::exception& e) {
      RECORD_ERROR(e);
    }
  };

 public:
  template <typename U = T>
  [[nodiscard]] U getParam(std::string_view key) const {
    try {
      if (!m_json.contains(key))
        throw std::runtime_error(
            {"Parameter \"" + std::string{key} + "\" not found!"});
      return m_json.at(std::string(key)).template get<U>();
    } catch (const std::exception& e) {
      RECORD_ERROR(e);
      return U{};
    }
  }
};

template <template <typename> class Type, typename T = double>
class IJsonConfigMap : public IJsonConfig<T> {
 private:
  std::unordered_map<std::string_view, Type<T>> m_paramsMap;

 public:
  IJsonConfigMap(std::string_view fileName);

 public:
  [[nodiscard]] Type<T> getParams(std::string_view key) const {
    try {
      if (auto it = m_paramsMap.find(key); it != m_paramsMap.end())
        return it->second;
      throw std::runtime_error("Object \"" + std::string{key} +
                               "\" not found!");
    } catch (const std::exception& e) {
      RECORD_ERROR(e);
      return Type<T>{};
    }
  };
};
#endif  // R2D2_CONFIG_JSON_HPP
