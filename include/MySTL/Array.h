#pragma once

#include "Iterator.h"
#include <cassert>
#include <initializer_list>
#include <utility>

namespace mystl {

template <typename T, std::size_t Size>
class Array {
public:
  static_assert(Size >= 0, "Size of array must be positive");

  using value_type = T;
  using pointer = T *;
  using const_pointer = T const *;
  using reference = T &;
  using const_reference = T const &;

  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;

  using iterator = ContiguousIterator<Array<T, Size>>;
  using const_iterator = ConstContiguousIterator<Array<T, Size>>;

public:
  constexpr explicit Array() = default;

  constexpr explicit Array(std::initializer_list<T> list) {
    const T *data = list.begin();
    for (std::size_t i = 0; i < Size; ++i)
      m_Data[i] = std::move(data[i]);
  }

  constexpr size_type size() const { return Size; }
  constexpr size_type max_size() const { return Size; }

  constexpr iterator begin() { return iterator{m_Data}; }
  constexpr const_iterator begin() const { return cbegin(); }
  constexpr const_iterator cbegin() const { return const_iterator{m_Data}; }

  constexpr iterator end() { return iterator{m_Data + Size}; }
  constexpr const_iterator end() const { return cend(); }
  constexpr const_iterator cend() { return const_iterator{m_Data + Size}; }

  template <typename Self>
  constexpr auto &&data(this Self &&self) {
    return std::forward<Self>(self).m_Data;
  }

  template <typename Self>
  constexpr auto &&operator[](this Self &&self, std::size_t index) {
    assert(index < Size);
    return std::forward<Self>(self).m_Data[index];
  }

  template <typename Self>
  constexpr auto &&front(this Self &&self) {
    static_assert(Size != 0, "Size of array must not be zero!");
    return std::forward<Self>(self).m_Data[0];
  }

  template <typename Self>
  constexpr auto &&back(this Self &&self) {
    static_assert(Size != 0, "Size of array must not be zero!");
    return std::forward<Self>(self).m_Data[Size - 1];
  }

private:
  value_type m_Data[Size];
};

} // namespace mystl
