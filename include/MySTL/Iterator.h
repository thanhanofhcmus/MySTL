#pragma once

namespace mystl {

namespace internal {

template <typename nodeptr_type, typename pointer_t, typename reference_t>
class base_bidirect_iter {
public:
  constexpr explicit base_bidirect_iter() : m_ProxyData(nullptr) {}

  constexpr explicit base_bidirect_iter(nodeptr_type proxyData)
      : m_ProxyData(proxyData) {}

  constexpr explicit base_bidirect_iter(const base_bidirect_iter &) = default;

  constexpr ~base_bidirect_iter() = default;

  constexpr pointer_t operator->() const { return &(m_ProxyData->data); }

  constexpr reference_t operator*() const { return m_ProxyData->data; }

  constexpr base_bidirect_iter &operator++() {
    m_ProxyData = m_ProxyData->next;
    return *this;
  }

  constexpr base_bidirect_iter operator++(int) {
    auto tmp = *this;
    (*this).operator++();
    return tmp;
  }

  constexpr base_bidirect_iter operator--() {
    m_ProxyData = m_ProxyData->prev;
    return *this;
  }

  constexpr base_bidirect_iter operator--(int) {
    auto tmp = *this;
    (*this).operator--();
    return tmp;
  }

  constexpr bool operator==(const base_bidirect_iter &other) {
    return (m_ProxyData == other.m_ProxyData);
  }

private:
  nodeptr_type m_ProxyData;
};

template <typename Container_t>
using BaseBidirectionalIterator_t =
    base_bidirect_iter<typename Container_t::nodeptr_type,
                       typename Container_t::pointer,
                       typename Container_t::reference>;

template <typename Container_t>
using BaseConstBidirectionalIterator_t =
    base_bidirect_iter<typename Container_t::nodeptr_type, // const_nodeptr_type
                       typename Container_t::const_pointer,
                       typename Container_t::const_reference>;

} // namespace internal

template <typename Container_t>
struct BidirectionalIterator
    : public internal::BaseBidirectionalIterator_t<Container_t> {
  constexpr explicit BidirectionalIterator() = default;

  explicit BidirectionalIterator(Container_t::nodeptr_type proxyData)
      : internal::BaseBidirectionalIterator_t<Container_t>{proxyData} {}
};

template <typename Container_t>
struct ConstBidirectionalIterator
    : public internal::BaseConstBidirectionalIterator_t<Container_t> {
  constexpr explicit ConstBidirectionalIterator() = default;

  constexpr explicit ConstBidirectionalIterator(
      Container_t::nodeptr_type proxyData)
      : internal::BaseConstBidirectionalIterator_t<Container_t>{proxyData} {}
};

namespace internal {

template <typename pointer_t, typename reference_t, typename difference_t>
class base_cont_iter {
public:
  using pointer_type = pointer_t;
  using reference_type = reference_t;
  using difference_type = difference_t;

  constexpr explicit base_cont_iter() : m_ProxyData{nullptr} {}

  constexpr explicit base_cont_iter(pointer_t proyData)
      : m_ProxyData{proyData} {}

  constexpr explicit base_cont_iter(const base_cont_iter &) = default;

  ~base_cont_iter() {}

  constexpr pointer_t operator->() const { return m_ProxyData; }

  constexpr reference_t operator*() const { return *m_ProxyData; }

  constexpr base_cont_iter &operator++() {
    ++m_ProxyData;
    return *this;
  }

  constexpr base_cont_iter operator++(int) {
    base_cont_iter tmp = *this;
    ++(*this);
    return tmp;
  }

  constexpr base_cont_iter &operator--() {
    --m_ProxyData;
    return *this;
  }

  constexpr base_cont_iter operator--(int) {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }

  constexpr base_cont_iter &operator+=(difference_t offset) {
    m_ProxyData += offset;
    return *this;
  }

  constexpr base_cont_iter &operator-=(difference_t offset) {
    (*this) += -offset;
    return *this;
  }

  constexpr base_cont_iter operator+(difference_t offset) {
    auto tmp = *this;
    return tmp.operator+=(offset);
  }

  constexpr base_cont_iter operator-(difference_t offset) {
    auto tmp = *this;
    return tmp.operator-=(offset);
  }

  constexpr base_cont_iter &operator[](difference_t index) {
    m_ProxyData += index;
    return *this;
  }

  constexpr difference_t operator-(const base_cont_iter &other) {
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

} // namespace mystl
