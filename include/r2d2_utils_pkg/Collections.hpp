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
  /**
   * @brief   Constructs a NamedHandlerVector with handlers for the specified
   *          names.
   *
   * @tparam  Node   The node handle type
   * @tparam  String Variadic string types for handler names
   * @param   node   Pointer to the node handle for constructing handlers
   * @param   names  Variadic list of handler names
   *
   * @details Creates handlers for each name and builds an index map for
   *          name-based access. Requires at least one name and all names must
   *          be convertible to string.
   */
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

  /**
   * @brief   Accesses a handler by name.
   *
   * @param   name The name of the handler to access
   * @return  Reference to the handler with the specified name
   *
   * @throws  r2d2_errors::collections::NameError if the name is not found
   */
  Handler<T>& operator()(std::string_view name) {
    if (auto it = m_indexMap.find(std::string{name}); it != m_indexMap.end())
      return m_objectVector[it->second];
    throw r2d2_errors::collections::NameError{name};
  };

 public:
  /**
   * @brief   Calls a member function on each handler in the vector.
   *
   * @tparam  Func The member function pointer type
   * @tparam  Args Variadic argument types
   * @param   func The member function pointer to call
   * @param   args Arguments to pass to the member function
   *
   * @details Invokes the specified member function on all handlers with the
   *          given arguments.
   */
  template <typename Func, typename... Args>
  void call_each(Func func, Args&&... args) {
    std::for_each(begin(), end(),
                  [&](auto& obj) { (obj.*func)(std::forward<Args>(args)...); });
  };

  /**
   * @brief   Calls a member function on each handler and collects the results.
   *
   * @tparam  Func The member function pointer type
   * @tparam  Args Variadic argument types
   * @param   func The member function pointer to call
   * @param   args Arguments to pass to the member function
   * @return       A vector containing the results from each handler
   *
   * @details Invokes the specified member function on all handlers and collects
   *          return values.
   */
  template <typename Func, typename... Args>
  auto get_each(Func func, Args&&... args) const {
    Vector<InvokeResultType<Func, Args...>> results_(size());
    std::transform(cbegin(), cend(), results_.begin(), [&](auto& obj) {
      return (obj.*func)(std::forward<Args>(args)...);
    });
    return results_;
  };

 public:
  /**
   * @brief   Gets the number of handlers in the vector.
   *
   * @return  The size of the handler vector
   */
  size_t size() const { return m_objectVector.size(); };

  /**
   * @brief   Gets an iterator to the beginning of the handler vector.
   *
   * @return  Iterator to the first handler
   */
  auto begin() { return m_objectVector.begin(); };

  /**
   * @brief   Gets an iterator to the end of the handler vector.
   *
   * @return  Iterator past the last handler
   */
  auto end() { return m_objectVector.end(); };

  /**
   * @brief   Gets a const iterator to the beginning of the handler vector.
   *
   * @return  Const iterator to the first handler
   */
  auto cbegin() const { return m_objectVector.cbegin(); };

  /**
   * @brief   Gets a const iterator to the end of the handler vector.
   *
   * @return  Const iterator past the last handler
   */
  auto cend() const { return m_objectVector.cend(); };
};
#endif  // INCLUDE_R2D2_UTILS_PKG_COLLECTIONS_HPP_
