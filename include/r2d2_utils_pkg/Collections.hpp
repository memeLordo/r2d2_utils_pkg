#ifndef R2D2_COLLECTIONS_HPP
#define R2D2_COLLECTIONS_HPP

#include <cassert>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

template <template <typename> class Type, typename T>
class NamedNodeHandlerCollection {
 private:
  std::vector<Type<T>> m_objectVector;
  std::unordered_map<std::string, size_t> m_indexMap;

 public:
  template <typename Node, typename... Args>
  NamedNodeHandlerCollection(Node* node, Args&&... names) {
    const size_t size_{sizeof...(names)};
    static_assert(size_ > 0, "At least one joint name must be provided!");
    m_objectVector.reserve(size_);
    m_indexMap.reserve(size_);
    initializeJoints(node, std::forward<Args>(names)...);
  };
  Type<T>& operator()(const std::string& name) const {
    auto it{m_indexMap.find(name)};
    if (it == m_indexMap.end())
      throw std::out_of_range("Joint name " + name + " not found!");
    return m_objectVector[it->second];
  };

 private:
  template <typename Node, typename First, typename... Rest>
  void initializeCollection(Node* node, First&& first, Rest&&... rest) {
    static_assert(std::is_convertible<First, std::string>::value,
                  "Joint names must be string type!");
    const std::string name_{std::forward<First>(first)};
    m_objectVector.emplace_back(Type<T>(node, name_));
    m_indexMap.emplace(name_, m_objectVector.size() - 1);
    if (sizeof...(rest) > 0)
      initializeJoints(node, std::forward<Rest>(rest)...);
  };

 public:
  size_t size() const { return m_objectVector.size(); };
  typename std::vector<Type<T>>::iterator begin() {
    return m_objectVector.begin();
  };
  typename std::vector<Type<T>>::iterator end() {
    return m_objectVector.end();
  };
};

#endif  // R2D2_COLLECTIONS_HPP
