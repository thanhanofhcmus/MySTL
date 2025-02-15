#pragma once

#include "Iterator.h"
#include "MySTL/algorithms.h"
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace mystl {

template <typename T>
class Vector {
public:
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = T const &;

  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;

  using iterator = ContiguousIterator<Vector<T>>;
  using const_iterator = ConstContiguousIterator<Vector<T>>;

public:
  constexpr explicit Vector() : m_Capacity(0), m_Size(0), m_Data(nullptr) {}

  constexpr explicit Vector(size_type size, const T &val = T{}) : Vector{} {
    realloc_and_resize(size);
    algo::fill(m_Data, m_Data + size, val);
  }

  constexpr explicit Vector(std::initializer_list<T> iList) : Vector{} {
    realloc_and_resize(iList.size());
    algo::copy(iList.begin(), iList.end(), m_Data);
  }

  constexpr explicit Vector(const Vector<T> &copy) : Vector{} {
    realloc_and_resize(copy.m_Size);
    algo::copy(copy.begin(), copy.end(), m_Data);
  }

  constexpr explicit Vector(Vector<T> &&move)
      : m_Capacity(move.m_Capacity), m_Size(move.m_Size), m_Data(move.m_Data) {
    move.m_Data = nullptr;
  }

  constexpr ~Vector() { clear(); }

  constexpr Vector &operator=(const Vector<T> &copy) {
    if (this != &copy) {
      deallocate();
      realloc_and_resize(copy.m_Size);
      algo::copy(copy.begin(), copy.end(), m_Data);
    }
    return *this;
  }

  constexpr Vector &operator=(Vector<T> &&move) {
    if (this != &move) {
      deallocate();
      m_Capacity = move.m_Capacity;
      m_Size = move.m_Size;
      m_Data = move.m_Data;
      move.m_Data = nullptr;
    }
    return *this;
  }

  constexpr size_type capacity() const { return m_Capacity; }

  constexpr size_type size() const { return m_Size; }

  constexpr bool empty() const { return (m_Size == 0); }

  constexpr iterator begin() { return iterator{m_Data}; }
  constexpr const_iterator begin() const { return cbegin(); }
  constexpr const_iterator cbegin() const { return const_iterator{m_Data}; }

  constexpr iterator end() { return iterator{m_Data + m_Size}; }
  constexpr const_iterator end() const { return cend(); }
  constexpr const_iterator cend() { return const_iterator{m_Data + m_Size}; }

  constexpr void reserve(size_type capacity) {
    // Do not use reallocate
    T *newData = new T[capacity];
    for (int i = 0; i < m_Size; ++i) {
      newData[i] = std::move(m_Data[i]);
    }
    deallocate();
    m_Data = newData;
    m_Capacity = capacity;
  }

  template <typename Self>
  constexpr auto &&operator[](this Self &&self, size_type index) {
    if (index < 0) {
      throw std::out_of_range("Invalid index");
    }
    if (self.m_Size == 0) {
      throw std::out_of_range("Vector has zero element");
    }
    if (index > self.m_Size) {
      throw std::out_of_range("Vector index out of range");
    }
    return std::forward<Self>(self).m_Data[index];
  }

  template <typename Self>
  constexpr auto &&front(this Self &&self) {
    return std::forward<Self>(self)[0];
  }

  template <typename Self>
  constexpr auto &&back(this Self &&self) {
    return std::forward<Self>(self)[self.m_Size - 1];
  }

  // Modifiers

  constexpr void clear() {
    for (std::size_t i = 0; i < m_Size; ++i) {
      m_Data[i].~T();
    }
    deallocate();
    m_Capacity = 0;
    m_Size = 0;
    m_Data = nullptr;
  }

  // TODO: add iterator versions

  constexpr void insert(size_type pos, const T &val = T{},
                        size_type count = 1) {
    size_t newSize = algo::max(m_Size, pos) + count;
    size_t oldSize = m_Size;

    if (newSize > m_Capacity) {
      realloc_and_resize(newSize);
    }

    // if pos > current size, default init the gap values
    algo::fill(m_Data + oldSize, m_Data + pos, T{});

    // if pos is in the array, move the current values to offset amount
    // make room for the new values
    if (pos < oldSize) {
      size_type offset = oldSize - pos;
      for (size_type i = 1; i <= count; ++i) {
        m_Data[newSize - i] = std::move(m_Data[pos + offset - i]);
      }
    }

    algo::fill(m_Data + pos, m_Data + pos + count, val);
  }

  constexpr void erase(size_type pos, size_type count = 1) {
    if (pos >= m_Size) {
      return;
    }

    count = algo::min(m_Size - pos, count);

    for (size_type i = pos; i < m_Size - count; ++i) {
      m_Data[i] = std::move(m_Data[i + count]);
    }

    m_Size -= count;

    if ((m_GrowthFactor * m_Size < m_Capacity) ||
        (m_Size < m_Capacity - m_MaxSizeDiff))
      reallocate();
  }

  constexpr void push_back(const T &val) {
    if (m_Size + 1 >= m_Capacity) {
      reallocate(true);
    }
    m_Data[m_Size] = std::move(val);
    m_Size++;
  }

  constexpr void push_back(T &&val) {
    if (m_Size + 1 >= m_Capacity) {
      reallocate(true);
    }
    m_Data[m_Size] = std::move(val);
    m_Size++;
  }

  template <typename... Args>
  constexpr void emplace_back(Args &&...args) {
    if (m_Size + 1 >= m_Capacity) {
      reallocate();
    }
    new (&m_Data[m_Size++]) T{std::forward<Args>(args)...};
  }

  constexpr void pop_back() {
    if (m_Size <= 0) {
      return;
    }

    delete &m_Data[m_Size - 1];
    --m_Size;
    if ((m_GrowthFactor * m_Size < m_Capacity) ||
        (m_Size < m_Capacity - m_MaxSizeDiff))
      reallocate();
  }

  void resize(size_type size, const T &val = T{}) {
    if (size == m_Size) {
      return;
    }
    size_type oldSize = m_Size;
    realloc_and_resize(size);
    algo::fill(m_Data + oldSize, m_Data + size, val);
  }

private:
  void reallocate(size_type newCapacity) {
    if (newCapacity - m_Size > m_MaxSizeDiff) {
      newCapacity = m_Size + m_MaxSizeDiff;
    }

    T *newData = (T *)::operator new(newCapacity * sizeof(T));
    for (size_type i = 0; i < m_Size; ++i) {
      newData[i] = std::move(m_Data[i]);
      m_Data[i].~T();
    }

    deallocate();
    m_Data = newData;
    m_Capacity = newCapacity;
  }

  void reallocate(bool increaseSize = false) {
    auto newSize = m_GrowthFactor * (static_cast<float>(m_Size) +
                                     static_cast<float>(increaseSize));
    reallocate(static_cast<size_type>(newSize));
  }

  void deallocate() {
    // Deallocate does not attemp to set m_Size and m_Capacity to valid data
    // ::operator delete(m_Data, m_Capacity * sizeof(T));
    ::operator delete(m_Data);
  }

  void realloc_and_resize(size_type newSize) {
    auto newCapacity = m_GrowthFactor * static_cast<float>(newSize);
    reallocate(static_cast<size_type>(newCapacity));
    m_Size = newSize;
  }

private:
  static constexpr float m_GrowthFactor = 1.5f;
  static constexpr float m_MaxSizeDiff = 50.0f;

  size_type m_Capacity;
  size_type m_Size;
  pointer m_Data;
};

} // namespace mystl
