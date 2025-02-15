#pragma once

#include "Iterator.h"
#include <initializer_list>
#include <cassert>
#include <utility>

template<typename T, std::size_t Size>
class Array
{
public:

	using value_t 			= T;
	using pointer_t 		= T*;
	using const_pointer_t 	= const T*;
	using reference_t 		= T&;
	using difference_t		= std::ptrdiff_t;

public:

	using iterator       	= ContiguousIterator<Array<T, Size>>;
	using const_iterator 	= ConstContiguousIterator<Array<T, Size>>;

	iterator begin() { return iterator(m_Data); }
	const_iterator begin() const { return const_iterator(m_Data); }

	iterator end() { return iterator(m_Data + Size); }
	const_iterator end() const { return const_iterator(m_Data + Size); }

public:

	// Constructor

	Array() = default;

	Array(std::initializer_list<T> list);

	// Capacity

	constexpr std::size_t size() const { return Size; };

	// Accessors

	T* data() { return m_Data; };
	const T* data() const { return m_Data; };

	T& operator[](std::size_t index);
	const T& operator[](std::size_t index) const;

	T& front();
	const T& front() const;

	T& back();
	const T& back() const;

private:

	value_t m_Data[Size];
};

template<typename T, std::size_t Size>
Array<T, Size>::Array(std::initializer_list<T> list)
{
	const T* data = list.begin();
	for (std::size_t i = 0; i < Size; ++i)
		m_Data[i] = std::move(data[i]);
}

template<typename T, std::size_t Size>
T& Array<T, Size>::operator[](std::size_t index)
{
	assert(index < Size);

	return m_Data[index];
}

template<typename T, std::size_t Size>
const T& Array<T, Size>::operator[](std::size_t index) const
{
	assert(index < Size);

	return m_Data[index];
}

template<typename T, std::size_t Size>
T& Array<T, Size>::front()
{
	static_assert(Size != 0, "Size of array must not be zero!");

	return m_Data[0];
}

template<typename T, std::size_t Size>
const T& Array<T, Size>::front() const
{
	static_assert(Size != 0, "Size of array must not be zero!");

	return m_Data[0];
}

template<typename T, std::size_t Size>
T&  Array<T, Size>::back()
{
	static_assert(Size != 0, "Size of array must not be zero!");

	return m_Data[Size - 1];
}

template<typename T, std::size_t Size>
const T&  Array<T, Size>::back() const
{
	static_assert(Size != 0, "Size of array must not be zero!");

	return m_Data[Size - 1];
}
