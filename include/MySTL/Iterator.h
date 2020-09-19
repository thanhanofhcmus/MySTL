#pragma once

#include <stdint.h>
#include <cstddef>

template <typename value_t, typename pointer_t, typename reference_t, 
			typename difference_t = std::ptrdiff_t>
class ContiguousIterator
{
public:

	ContiguousIterator()
		: m_ProxyData(nullptr) {}

	ContiguousIterator(pointer_t vectorData)
		: m_ProxyData(vectorData) {}
	
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
};

template <typename value_t, typename pointer_t, typename reference_t, 
			typename difference_t = std::ptrdiff_t>
ContiguousIterator<value_t, pointer_t, reference_t, difference_t> 
operator+(difference_t offset, 
			ContiguousIterator<value_t, pointer_t, reference_t, difference_t> iter)
{
	return (iter += offset);
}