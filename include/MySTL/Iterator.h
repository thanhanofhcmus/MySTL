#pragma once

#include <stdint.h>
#include <cstddef>

template <typename Container_t>
class ConstBidirectionalIterator;
template <typename Container_t>
class BidirectionalIterator
{
public:

	using value_t 		= typename Container_t::value_t;
	using pointer_t 	= typename Container_t::pointer_t;
	using reference_t 	= typename Container_t::reference_t;
	using nodeptr_t 	= typename Container_t::nodeptr_t;

	using const_t		= ConstBidirectionalIterator<Container_t>;

public:

	BidirectionalIterator()
		: m_ProxyData(nullptr) {}

	BidirectionalIterator(nodeptr_t proxyData)
		: m_ProxyData(proxyData) {}

	BidirectionalIterator(const BidirectionalIterator&) = default;

	~BidirectionalIterator() {}

	pointer_t operator->() const { return &(m_ProxyData->data); }

	reference_t operator*() const { return m_ProxyData->data; }

	BidirectionalIterator& operator++()
	{
		m_ProxyData = m_ProxyData->next;
		return *this;
	}

	BidirectionalIterator operator++(int)
	{
		auto tmp = *this;
		(*this).operator++();
		return tmp;
	}

	BidirectionalIterator operator--()
	{
		m_ProxyData = m_ProxyData->prev;
		return *this;
	}

	BidirectionalIterator operator--(int)
	{
		auto tmp = *this;
		(*this).operator--();
		return tmp;
	}

	bool operator==(const BidirectionalIterator& other)
	{
		return (m_ProxyData == other.m_ProxyData);
	}

	bool operator!=(const BidirectionalIterator& other)
	{
		return !operator==(other);
	}

private:

	nodeptr_t m_ProxyData;

	friend Container_t;
	friend const_t;

};

template <typename Container_t>
class ConstBidirectionalIterator
{
public:

	using value_t 		= const typename Container_t::value_t;
	using pointer_t 	= typename Container_t::const_pointer_t;
	using reference_t 	= const typename Container_t::reference_t;
	using nodeptr_t 	= typename Container_t::nodeptr_t;
	// Might need to change from nodeptr_t to const_nodeptr_t

	using non_const_t 	= BidirectionalIterator<Container_t>;

public:

	ConstBidirectionalIterator()
		: m_ProxyData(nullptr) {}

	ConstBidirectionalIterator(nodeptr_t proxyData)
		: m_ProxyData(proxyData) {}

	ConstBidirectionalIterator(const non_const_t& iterator) 
		: m_ProxyData(iterator.m_ProxyData) {}

	ConstBidirectionalIterator(const ConstBidirectionalIterator&) = default;

	~ConstBidirectionalIterator() {}

	pointer_t operator->() const { return &(m_ProxyData->data); }

	reference_t operator*() const { return m_ProxyData->data; }

	ConstBidirectionalIterator& operator++()
	{
		m_ProxyData = m_ProxyData->next;
		return *this;
	}

	ConstBidirectionalIterator operator++(int)
	{
		auto tmp = *this;
		(*this).operator++();
		return tmp;
	}

	ConstBidirectionalIterator operator--()
	{
		m_ProxyData = m_ProxyData->prev;
		return *this;
	}

	ConstBidirectionalIterator operator--(int)
	{
		auto tmp = *this;
		(*this).operator--();
		return tmp;
	}

	bool operator==(const ConstBidirectionalIterator& other)
	{
		return (m_ProxyData == other.m_ProxyData);
	}

	bool operator!=(const ConstBidirectionalIterator& other)
	{
		return !operator==(other);
	}

private:

	nodeptr_t m_ProxyData;
	
	friend Container_t;

};



template <typename Container_t>
class ConstContiguousIterator;
template <typename Container_t>
class ContiguousIterator
{
public:

	using value_t 		= typename Container_t::value_t;
	using pointer_t 	= typename Container_t::pointer_t;
	using reference_t 	= typename Container_t::reference_t;
	using difference_t 	= typename Container_t::difference_t;

	using const_t 		= ConstContiguousIterator<Container_t>;

public:

	ContiguousIterator()
		: m_ProxyData(nullptr) {}

	ContiguousIterator(pointer_t proxyData)
		: m_ProxyData(proxyData) {}
	
	ContiguousIterator(const ContiguousIterator&) = default;

	~ContiguousIterator() {}

	pointer_t operator->() const { return m_ProxyData; }

	reference_t operator*() const { return *m_ProxyData; }

	ContiguousIterator& operator++()
	{
		++m_ProxyData;
		return *this;
	}

	ContiguousIterator operator++(int)
	{
		ContiguousIterator tmp = *this;
		++(*this);
		return tmp;
	}

	ContiguousIterator& operator--()
	{
		--m_ProxyData;
		return *this;
	}

	ContiguousIterator operator--(int)
	{
		ContiguousIterator tmp = *this;
		++(*this);
		return tmp;
	}

	ContiguousIterator& operator+=(difference_t offset)
	{
		m_ProxyData += offset;
		return *this;
	}

	ContiguousIterator& operator-=(difference_t offset)
	{
		(*this) += -offset;
		return *this;
	}

	ContiguousIterator operator+(difference_t offset)
	{
		ContiguousIterator tmp = *this;
		return tmp.operator+=(offset);
	}

	ContiguousIterator operator-(difference_t offset)
	{
		ContiguousIterator tmp = *this;
		return tmp.operator-=(offset);
	}

	ContiguousIterator& operator[](difference_t index)
	{
		m_ProxyData += index;
		return *this;
	}

	difference_t operator-(const ContiguousIterator& other)
	{
		return (m_ProxyData - other.m_ProxyData);
	}

	bool operator<(const ContiguousIterator& other)
	{
		return (operator-(other) > 0);
	}

	bool operator>(const ContiguousIterator& other)
	{
		return (operator-(other) < 0);
	}

	bool operator>=(const ContiguousIterator& other)
	{
		return !(operator<(other));
	}

	bool operator<=(const ContiguousIterator& other)
	{
		return !(operator>(other));
	}

	bool operator==(const ContiguousIterator& other) 
	{
		return (this->m_ProxyData == other.m_ProxyData);
	}

	bool operator!=(const ContiguousIterator& other)
	{
		return !operator==(other);
	}

private:

	pointer_t m_ProxyData;

	friend Container_t;
	friend const_t;
};

template <typename Container_t>
ContiguousIterator<Container_t> operator+
(typename ContiguousIterator<Container_t>::difference_t offset, ContiguousIterator<Container_t> iter)
{
	return (iter += offset);
}

template <typename Container_t>
class ConstContiguousIterator
{
public:

	using value_t 		= const typename Container_t::value_t;
	using pointer_t 	= typename Container_t::const_pointer_t;
	using reference_t 	= const typename Container_t::reference_t;
	using difference_t 	= typename Container_t::difference_t;

	using non_const_t 	= ContiguousIterator<Container_t>;

public:

	ConstContiguousIterator()
		: m_ProxyData(nullptr) {}

	ConstContiguousIterator(pointer_t proyData)
		: m_ProxyData(proyData) {}

	ConstContiguousIterator(const non_const_t& iterator)
		: m_ProxyData(iterator.m_ProxyData) {}
	
	ConstContiguousIterator(const ConstContiguousIterator&) = default;

	~ConstContiguousIterator() {}

	pointer_t operator->() const { return m_ProxyData; }

	reference_t operator*() const { return *m_ProxyData; }

	ConstContiguousIterator& operator++()
	{
		++m_ProxyData;
		return *this;
	}

	ConstContiguousIterator operator++(int)
	{
		ConstContiguousIterator tmp = *this;
		++(*this);
		return tmp;
	}

	ConstContiguousIterator& operator--()
	{
		--m_ProxyData;
		return *this;
	}

	ConstContiguousIterator operator--(int)
	{
		ConstContiguousIterator tmp = *this;
		++(*this);
		return tmp;
	}

	ConstContiguousIterator& operator+=(difference_t offset)
	{
		m_ProxyData += offset;
		return *this;
	}

	ConstContiguousIterator& operator-=(difference_t offset)
	{
		(*this) += -offset;
		return *this;
	}

	ConstContiguousIterator operator+(difference_t offset)
	{
		ConstContiguousIterator tmp = *this;
		return tmp.operator+=(offset);
	}

	ConstContiguousIterator operator-(difference_t offset)
	{
		ConstContiguousIterator tmp = *this;
		return tmp.operator-=(offset);
	}

	ConstContiguousIterator& operator[](difference_t index)
	{
		m_ProxyData += index;
		return *this;
	}

	difference_t operator-(const ConstContiguousIterator& other)
	{
		return (m_ProxyData - other.m_ProxyData);
	}

	bool operator<(const ConstContiguousIterator& other)
	{
		return (operator-(other) > 0);
	}

	bool operator>(const ConstContiguousIterator& other)
	{
		return (operator-(other) < 0);
	}

	bool operator>=(const ConstContiguousIterator& other)
	{
		return !(operator<(other));
	}

	bool operator<=(const ConstContiguousIterator& other)
	{
		return !(operator>(other));
	}

	bool operator==(const ConstContiguousIterator& other) 
	{
		return (this->m_ProxyData == other.m_ProxyData);
	}

	bool operator!=(const ConstContiguousIterator& other)
	{
		return !operator==(other);
	}

private:

	pointer_t m_ProxyData;

	friend Container_t;
};

template <typename Container_t>
ConstContiguousIterator<Container_t> operator+
(typename ConstContiguousIterator<Container_t>::difference_t offset, ConstContiguousIterator<Container_t> iter)
{
	return (iter += offset);
}