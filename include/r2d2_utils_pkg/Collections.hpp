#ifndef INCLUDE_R2D2_UTILS_PKG_COLLECTIONS_HPP_
#define INCLUDE_R2D2_UTILS_PKG_COLLECTIONS_HPP_

#include <algorithm>
#include <cassert>

#include "Exceptions.hpp"

template <template <typename> class Vector, template <typename> class Handler,
          typename T>
class NamedHandlerVector {
 private:
  template <typename Func, typename... Args>
  using InvokeResultType = std::invoke_result_t<Func, Handler<T>&, Args...>;

 protected:
  Vector<Handler<T>> m_objectVector;
  std::unordered_map<std::string, size_t> m_indexMap;

 public:
  template <typename Node, typename... String>
  NamedHandlerVector(Node* node, String&&... names) {
    constexpr size_t size_{sizeof...(names)};
    static_assert(size_ > 0, "At least one name is required!");
    static_assert((std::is_convertible_v<String, std::string> && ...),
                  "All names must be convertible to string!");
    m_objectVector.reserve(size_);
    m_indexMap.reserve(size_);

    size_t index_{0};
    ((m_objectVector.emplace_back(node, std::forward<String>(names)),
      m_indexMap.emplace(std::forward<String>(names), index_++)),
     ...);
  };
  Handler<T>& operator()(std::string_view name) {
    if (auto it = m_indexMap.find(std::string{name}); it != m_indexMap.end())
      return m_objectVector[it->second];
    throw r2d2_errors::collections::NameError{name};
  };

 public:
  template <typename Func, typename... Args>
  void call_each(Func func, Args&&... args) {
    std::for_each(begin(), end(),
                  [&](auto& obj) { (obj.*func)(std::forward<Args>(args)...); });
  };
  template <typename Func, typename... Args>
  auto get_each(Func func, Args&&... args) const {
    Vector<InvokeResultType<Func, Args...>> results_(size());
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
#endif  // INCLUDE_R2D2_UTILS_PKG_COLLECTIONS_HPP_
