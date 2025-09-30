#ifndef R2D2_COLLECTIONS_HPP
#define R2D2_COLLECTIONS_HPP

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

template <template <typename> class Type, typename T>
class NamedHandlerCollection {
 private:
  template <typename Func, typename... Args>
  using InvokeResultType = std::invoke_result_t<Func, Type<T>&, Args...>;

 protected:
  std::vector<Type<T>> m_objectVector;
  std::unordered_map<std::string, size_t> m_indexMap;

 public:
  template <typename Node, typename... Args>
  NamedHandlerCollection(Node* node, Args&&... names) {
    constexpr size_t size_{sizeof...(names)};
    static_assert(size_ > 0, "At least one name is required!");
    m_objectVector.reserve(size_);
    m_indexMap.reserve(size_);
    initializeCollection(node, std::forward<Args>(names)...);
  };
  Type<T>& operator()(const std::string& name) {
    if (auto it = m_indexMap.find(name); it != m_indexMap.end())
      return m_objectVector[it->second];
    throw std::out_of_range("Name \"" + name + "\" not found!");
  };

 private:
  template <typename Node>
  void initializeCollection(Node*) {};
  template <typename Node, typename First, typename... Rest>
  void initializeCollection(Node* node, First&& first, Rest&&... rest) {
    static_assert(std::is_convertible_v<First, std::string>,
                  "Name must be convertible to string!");
    m_objectVector.emplace_back(node, std::forward<First>(first));
    m_indexMap.emplace(std::forward<First>(first), m_objectVector.size() - 1);
    if constexpr (sizeof...(rest) > 0)
      initializeCollection(node, std::forward<Rest>(rest)...);
  };

 public:
  template <typename Func, typename... Args>
  void call_each(Func func, Args&&... args) {
    std::for_each(cbegin(), cend(),
                  [&](auto& obj) { (obj.*func)(std::forward<Args>(args)...); });
  };
  template <typename Func, typename... Args>
  auto get_each(Func func, Args&&... args) const
      -> std::vector<InvokeResultType<Func, Args...>> {
    std::vector<InvokeResultType<Func, Args...>> results_{size()};
    std::transform(cbegin(), cend(), results_.begin(), [&](auto& obj) {
      return (obj.*func)(std::forward<Args>(args)...);
    });
    return results_;
  };

 public:
  size_t size() const { return m_objectVector.size(); };
  auto begin() { return m_objectVector.begin(); };
  auto end() { return m_objectVector.end(); };
  auto cbegin() const { return m_objectVector.cbegin(); };
  auto cend() const { return m_objectVector.cend(); };
};
#endif  // R2D2_COLLECTIONS_HPP
