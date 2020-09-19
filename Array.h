#pragma once

#include "Iterator.h"
#include <stdint.h>
#include <initializer_list>
#include <cassert>

template<typename T, size_t Size>
class Array
{
public:

	using iterator 			= ContiguousIterator<T, T*, T&>;
	using const_iterator	= ContiguousIterator<const T, const T*, const T&>;

	iterator begin() { return iterator(m_Data); }
	const_iterator begin() const { return const_iterator(m_Data); }

	iterator end() { return iterator(m_Data + Size); }
	const_iterator end() const { return const_iterator(m_Data + Size); }

public:

	// Constructor

	Array() = default;

	Array(std::initializer_list<T> list);

	// Capacity

	constexpr size_t size() const { return Size; };

	// Accessors

	T* data() { return m_Data; };
	const T* data() const { return m_Data; };

	T& operator[](size_t index);
	const T& operator[](size_t index) const;

	T& front();
	const T& front() const;

	T& back();
	const T& back() const;

private:

	T m_Data[Size];
};

template<typename T, size_t Size>
Array<T, Size>::Array(std::initializer_list<T> list)
{
	const T* data = list.begin();
	for (size_t i = 0; i < Size; ++i)
		m_Data[i] = std::move(data[i]);
}

template<typename T, size_t Size>
T& Array<T, Size>::operator[](size_t index)
{
	assert(index < Size);

	return m_Data[index];
}

template<typename T, size_t Size>
const T& Array<T, Size>::operator[](size_t index) const
{
	assert(index < Size);
	
	return m_Data[index];
}

template<typename T, size_t Size>
T& Array<T, Size>::front()
{
	static_assert(Size != 0, "Size of array must not be zero!");

	return m_Data[0];
}

template<typename T, size_t Size>
const T& Array<T, Size>::front() const
{
	static_assert(Size != 0, "Size of array must not be zero!");

	return m_Data[0];
}

template<typename T, size_t Size>
T&  Array<T, Size>::back()
{
	static_assert(Size != 0, "Size of array must not be zero!");

	return m_Data[Size - 1];
}

template<typename T, size_t Size>
const T&  Array<T, Size>::back() const
{
	static_assert(Size != 0, "Size of array must not be zero!");

	return m_Data[Size - 1];
}