#pragma once

template <typename Container_t>
class ConstBidirectionalIterator;

template <typename Container_t>
class BidirectionalIterator {
public:
  using value_type = typename Container_t::value_type;
  using pointer_type = typename Container_t::pointer_type;
  using reference_type = typename Container_t::reference_type;
  using nodeptr_type = typename Container_t::nodeptr_type;

  using const_type = ConstBidirectionalIterator<Container_t>;

public:
  explicit BidirectionalIterator() : m_ProxyData(nullptr) {}

  explicit BidirectionalIterator(nodeptr_type proxyData)
      : m_ProxyData(proxyData) {}

  explicit BidirectionalIterator(const BidirectionalIterator &) = default;

  ~BidirectionalIterator() = default;

  pointer_type operator->() const { return &(m_ProxyData->data); }

  reference_type operator*() const { return m_ProxyData->data; }

  BidirectionalIterator &operator++() {
    m_ProxyData = m_ProxyData->next;
    return *this;
  }

  BidirectionalIterator operator++(int) {
    auto tmp = *this;
    (*this).operator++();
    return tmp;
  }

  BidirectionalIterator operator--() {
    m_ProxyData = m_ProxyData->prev;
    return *this;
  }

  BidirectionalIterator operator--(int) {
    auto tmp = *this;
    (*this).operator--();
    return tmp;
  }

  bool operator==(const BidirectionalIterator &other) {
    return (m_ProxyData == other.m_ProxyData);
  }

  bool operator!=(const BidirectionalIterator &other) {
    return !operator==(other);
  }

private:
  nodeptr_type m_ProxyData;

  friend Container_t;
  friend const_type;
};

template <typename Container_t>
class ConstBidirectionalIterator {
public:
  using value_t = const typename Container_t::value_t;
  using pointer_t = typename Container_t::const_pointer_t;
  using reference_t = const typename Container_t::reference_t;
  using nodeptr_t = typename Container_t::nodeptr_t;
  // Might need to change from nodeptr_t to const_nodeptr_t

  using non_const_t = BidirectionalIterator<Container_t>;

public:
  ConstBidirectionalIterator() : m_ProxyData(nullptr) {}

  ConstBidirectionalIterator(nodeptr_t proxyData) : m_ProxyData(proxyData) {}

  ConstBidirectionalIterator(const non_const_t &iterator)
      : m_ProxyData(iterator.m_ProxyData) {}

  ConstBidirectionalIterator(const ConstBidirectionalIterator &) = default;

  ~ConstBidirectionalIterator() {}

  pointer_t operator->() const { return &(m_ProxyData->data); }

  reference_t operator*() const { return m_ProxyData->data; }

  ConstBidirectionalIterator &operator++() {
    m_ProxyData = m_ProxyData->next;
    return *this;
  }

  ConstBidirectionalIterator operator++(int) {
    auto tmp = *this;
    (*this).operator++();
    return tmp;
  }

  ConstBidirectionalIterator operator--() {
    m_ProxyData = m_ProxyData->prev;
    return *this;
  }

  ConstBidirectionalIterator operator--(int) {
    auto tmp = *this;
    (*this).operator--();
    return tmp;
  }

  bool operator==(const ConstBidirectionalIterator &other) {
    return (m_ProxyData == other.m_ProxyData);
  }

  bool operator!=(const ConstBidirectionalIterator &other) {
    return !operator==(other);
  }

private:
  nodeptr_t m_ProxyData;

  friend Container_t;
};

namespace internal {

template <typename pointer_t, typename reference_t, typename difference_t>
class base_cont_iter {
public:
  using pointer_type = pointer_t;
  using reference_type = reference_t;
  using difference_type = difference_t;

  constexpr explicit base_cont_iter() : m_ProxyData(nullptr) {}

  constexpr explicit base_cont_iter(pointer_t proyData)
      : m_ProxyData(proyData) {}

  constexpr explicit base_cont_iter(const base_cont_iter &) = default;

  ~base_cont_iter() {}

  pointer_t operator->() const { return m_ProxyData; }

  reference_t operator*() const { return *m_ProxyData; }

  base_cont_iter &operator++() {
    ++m_ProxyData;
    return *this;
  }

  base_cont_iter operator++(int) {
    base_cont_iter tmp = *this;
    ++(*this);
    return tmp;
  }

  base_cont_iter &operator--() {
    --m_ProxyData;
    return *this;
  }

  base_cont_iter operator--(int) {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }

  base_cont_iter &operator+=(difference_t offset) {
    m_ProxyData += offset;
    return *this;
  }

  base_cont_iter &operator-=(difference_t offset) {
    (*this) += -offset;
    return *this;
  }

  base_cont_iter operator+(difference_t offset) {
    auto tmp = *this;
    return tmp.operator+=(offset);
  }

  base_cont_iter operator-(difference_t offset) {
    auto tmp = *this;
    return tmp.operator-=(offset);
  }

  base_cont_iter &operator[](difference_t index) {
    m_ProxyData += index;
    return *this;
  }

  difference_t operator-(const base_cont_iter &other) {
    return (m_ProxyData - other.m_ProxyData);
  }

  constexpr bool operator==(base_cont_iter const &other) const {
    return this->m_ProxyData == other.m_ProxyData;
  }

  constexpr auto operator<=>(base_cont_iter const &other) const {
    return this->m_ProxyData <=> other.m_ProxyData;
  }

private:
  pointer_t m_ProxyData;
};

template <typename Container_t>
using BaseContigiousIterator_t =
    base_cont_iter<typename Container_t::pointer,
                   typename Container_t::reference,
                   typename Container_t::difference_type>;

template <typename Container_t>
using BaseConstContiguousIterator_t =
    base_cont_iter<typename Container_t::const_pointer,
                   typename Container_t::const_reference,
                   typename Container_t::difference_type>;

} // namespace internal

template <typename Container_t>
struct ContiguousIterator
    : public internal::BaseContigiousIterator_t<Container_t> {
  constexpr explicit ContiguousIterator() = default;

  constexpr explicit ContiguousIterator(Container_t::pointer proxyData)
      : internal::BaseContigiousIterator_t<Container_t>{proxyData} {}
};

template <typename Container_t>
struct ConstContiguousIterator
    : public internal::BaseConstContiguousIterator_t<Container_t> {

  constexpr explicit ConstContiguousIterator() = default;

  constexpr explicit ConstContiguousIterator(
      Container_t::const_pointer proxyData)
      : internal::BaseConstContiguousIterator_t<Container_t>{proxyData} {}
};
