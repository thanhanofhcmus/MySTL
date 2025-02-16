#pragma once

#include "Vector.h"

namespace mystl {

template <typename T, typename Container_t = Vector<T>>
class Queue {
public:
  using value_type = typename Container_t::value_t;
  using pointer = typename Container_t::pointer_t;
  using const_pointer_t = typename Container_t::const_pointer_t;
  using reference = typename Container_t::reference_t;
  using const_reference = typename Container_t::reference_type;
  using size_type = typename Container_t::size_type;

  using iterator = typename Container_t::iterator;
  using const_iterator = typename Container_t::const_iterator;

public:
  constexpr explicit Queue() = default;

  constexpr explicit Queue(Container_t const &containter)
      : m_Underlying(containter) {}

  constexpr size_type size() const { return m_Underlying.size(); }

  constexpr bool empty() const { return m_Underlying.empty(); }

  iterator begin() { return m_Underlying.begin(); }
  const_iterator begin() const { return m_Underlying.begin(); }
  const_iterator cbegin() const { return m_Underlying.cbegin(); }

  iterator end() { return m_Underlying.end(); }
  const_iterator end() const { return m_Underlying.end(); }
  const_iterator cend() const { return m_Underlying.cend(); }

  template <typename Self>
  constexpr auto &&front(this Self &&self) {
    return std::forward<Self>(self).front();
  }

  template <typename Self>
  constexpr auto &&back(this Self &&self) {
    return std::forward<Self>(self).back();
  }

  constexpr void push(T const &val) { m_Underlying.push_back(val); }
  constexpr void push(T &&val) { m_Underlying.push_back(std::move(val)); }

  template <typename... Args>
  constexpr void emplace(Args &&...args) {
    m_Underlying.emplace_back(std::forward<Args>(args)...);
  }

  constexpr void pop() { m_Underlying.pop_back(); }

private:
  Container_t m_Underlying;
};

} // namespace mystl
