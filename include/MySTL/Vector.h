#pragma once

#include "Iterator.h"
#include <stdexcept>
#include <initializer_list>

template <typename T>
class Vector
{
public:

	using iterator       = ContiguousIterator<T, T*, T&>;
	using const_iterator = ContiguousIterator<const T, const T*, const T&>;

	iterator begin() { return iterator(m_Data); }
	const_iterator begin() const { return const_iterator(m_Data); }

	iterator end() { return iterator(m_Data + m_Size); }
	const_iterator end() const { return const_iterator(m_Data + m_Size); }

public:

	// Constructors - destructor

	Vector()
		:m_Capacity(0), m_Size(0), m_Data(nullptr) {}

	Vector(size_t size, const T& val = T());

	Vector(std::initializer_list<T> iList);

	Vector(const Vector<T>& copy);

	Vector(Vector<T>&& move);

	~Vector();

	// assignment operators

	Vector& operator=(const Vector<T>& copy);
	
	Vector& operator=(Vector<T>&& move);

	// Capacity

	size_t capacity() const { return m_Capacity; }

	size_t size() const { return m_Size; }

	bool empty() const { return (m_Size == 0); }

	void reserve(size_t capacity);

	// Accessors

	T& operator[](size_t index);
	const T& operator[](size_t index) const;

	T& front();
	const T& front() const;

	T& back();
	const T& back() const;

	// Modifiers

	void clear();

	// TODO: add iterator versions	

	void insert(size_t pos, const T& val = T(), size_t count = 1);

	void erase(size_t pos, size_t count = 1);

	void push_back(const T& val);
	void push_back(T&& val);

	template<typename... Args>
	void emplace_back(Args&&... args);

	void pop_back();

	void resize(size_t size, const T& val = T());

private:

	void reallocate(size_t newCapacity);

	void reallocate(bool increaseSize = false);

	void deallocate();

	void realloc_and_resize(size_t newSize);

private:

	static constexpr float m_GrowthFactor = 1.5f;
	static constexpr float m_MaxSizeDiff = 50.0f;

	size_t m_Capacity;
	size_t m_Size;
	T* m_Data;
};

template <typename T>
void Vector<T>::reallocate(size_t newCapacity)
{
	if (newCapacity - m_Size > m_MaxSizeDiff)
		newCapacity = m_Size + m_MaxSizeDiff;
	
	T* newData = (T*)::operator new(newCapacity * sizeof(T));
	for (size_t i = 0; i < m_Size; ++i)
	{
		newData[i] = std::move(m_Data[i]);
		m_Data[i].~T();
	}

	deallocate();
	m_Data = newData;
	m_Capacity = newCapacity;
}

template <typename T>
void Vector<T>::reallocate(bool increaseSize)
{
	reallocate((size_t)(m_GrowthFactor * (m_Size + (size_t)increaseSize)));
}

template<typename T>
void Vector<T>::deallocate()
{
	// Deallocate does not attemp to set m_Size and m_Capacity to valid data

	::operator delete(m_Data, m_Capacity * sizeof(T));
}

template<typename T>
void Vector<T>::realloc_and_resize(size_t newSize)
{
	reallocate((size_t)(m_GrowthFactor * newSize));
	m_Size = newSize;
}

template <typename T>
Vector<T>::Vector(size_t size, const T& val)
	:Vector()
{
	realloc_and_resize(size);

	for (size_t i = 0; i < size; ++i)
		m_Data[i] = val;
}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> iList)
	:Vector()
{
	realloc_and_resize(iList.size());

	const T* iListData = iList.begin();
	for (size_t i = 0; i < m_Size; ++i)
		m_Data[i] = std::move(iListData[i]);
}

template <typename T>
Vector<T>::Vector(const Vector<T>& copy)
	:Vector()
{
	realloc_and_resize(copy.m_Size);

	for (size_t i = 0; i < m_Size; ++i)
		m_Data[i] = copy.m_Data[i];
}

template <typename T>
Vector<T>::Vector(Vector<T>&& move)
	:m_Capacity(move.m_Capacity), m_Size(move.m_Size), m_Data(move.m_Data)
{
	move.m_Data = nullptr;
}

template <typename T>
Vector<T>::~Vector()
{
	clear();
	deallocate();
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& copy)
{
	if (this != &copy)
	{
		deallocate();
		realloc_and_resize(copy.m_Size);

		for (size_t i = 0; i < m_Size; ++i)
			m_Data[i] = copy.m_Data[i];

		copy.m_Data = nullptr;
	}

	return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& move)
{
	if (this != &move)
	{
		deallocate();

		m_Capacity = move.m_Capacity;
		m_Size = move.m_Size;
		m_Data = move.m_Data;

		move.m_Data = nullptr;
	}
}

template <typename T>
void Vector<T>::reserve(size_t capacity)
{
	// Do not use reallocate

	T* newData = new T[capacity];
	for (int i = 0; i < m_Size; ++i)
		newData[i] = std::move(m_Data[i]);

	deallocate();
	m_Data = newData;
	m_Capacity = capacity;
}

template <typename T>
T& Vector<T>::operator[](size_t index)
{
	if (index > m_Size)
		throw std::out_of_range("Vector index out of range!");
	return m_Data[index];
}

template <typename T>
const T& Vector<T>::operator[](size_t index) const
{
	if (index > m_Size)
		throw std::out_of_range("Vector index out of range!");
	return m_Data[index];
}

template <typename T>
T& Vector<T>::front()
{
	if (m_Size == 0)
		throw std::out_of_range("Vector has zero element!");
	return *m_Data;
}

template <typename T>
const T& Vector<T>::front() const
{
	if (m_Size == 0)
		throw std::out_of_range("Vector has zero element!");
	return *m_Data;
}

template <typename T>
T& Vector<T>::back()
{
	if (m_Size == 0)
		throw std::out_of_range("Vector has zero element!");
	return m_Data[m_Size - 1];
}

template <typename T>
const T& Vector<T>::back() const
{
	if (m_Size == 0)
		throw std::out_of_range("Vector has zero element!");
	return *m_Data[m_Size - 1];
}

template <typename T>
void Vector<T>::clear()
{
	for (size_t i = 0; i < m_Size; ++i)
		m_Data[i].~T();

	deallocate();
	m_Capacity = 0;
	m_Size = 0;
	m_Data = nullptr;
}

template <typename T>
void Vector<T>::insert(size_t pos, const T& val, size_t count)
{
	const size_t interPos = (m_Size > pos) ? m_Size : pos;
	const size_t newSize = interPos + count;
	const size_t oldSize = m_Size;

	if (newSize > m_Capacity)
		realloc_and_resize(newSize);
	
	for (size_t i = oldSize; i < pos; ++i)
		m_Data[i] = T();

	if (pos < oldSize)
	{
		const size_t offset = oldSize - pos;
		for (size_t i = 1; i <= count; ++i)
			m_Data[newSize - i] = std::move(m_Data[pos + offset - i]);
	}

	for (size_t i = 0; i < count; ++i)
		m_Data[pos + i] = val;
}

template <typename T>
void Vector<T>::erase(size_t pos, size_t count)
{
	if (pos >= m_Size)
		return;
	
	count = (m_Size - pos > count) ? count : (m_Size - pos);

	for (size_t i = pos; i < m_Size - count; ++i)
		m_Data[i] = std::move(m_Data[i + count]);

	m_Size -= count;
	
	if ((m_GrowthFactor * m_Size < m_Capacity) ||
		(m_Size < m_Capacity - m_MaxSizeDiff))
		reallocate();
}

template <typename T>
template <typename... Args>
void Vector<T>::emplace_back(Args&&... args)
{
	if (m_Size + 1 >= m_Capacity)
		reallocate();

	new(&m_Data[m_Size++]) T{ std::forward<Args>(args)... };
}

template <typename T>
void Vector<T>::push_back(const T& val)
{
	if (m_Size + 1 >= m_Capacity)
		reallocate(true);

	m_Data[m_Size++] = val;
}

template <typename T>
void Vector<T>::push_back(T&& val)
{
	
	if (m_Size + 1 >= m_Capacity)
		reallocate(true);

	m_Data[m_Size] = std::move(val);
	m_Size++;
}

template <typename T>
void Vector<T>::pop_back()
{
	if (m_Size <= 0)
		return;

	delete &m_Data[m_Size - 1];
	--m_Size;

	if ((m_GrowthFactor * m_Size < m_Capacity) ||
		(m_Size < m_Capacity - m_MaxSizeDiff))
		reallocate();
}

template <typename T>
void Vector<T>::resize(size_t size, const T& val)
{
	if (size == m_Size)
		return;
		
	size_t oldSize = m_Size;

	realloc_and_resize(size);
	for (size_t i = oldSize; i < size; ++i)
		*(m_Data + i) = val;
}