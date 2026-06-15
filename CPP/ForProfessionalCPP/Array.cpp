#include "Array.h"
#include <iostream>
#include <memory>
using namespace std;

template<typename T>
Array<T>::Array()
{
	m_size = _allocSize;
	m_elements = new T[m_size] {};
}

template<typename T>
Array<T>::~Array()
{
	delete[] m_elements;
}

template<typename T>
Array<T>::Array(Array&& src) noexcept
	: m_elements(std::exchange(src.m_elements, nullptr)),
	m_size(std::exchange(src.m_size, 0))
{

}

template<typename T>
Array<T>& Array<T>::operator=(Array&& rhs) noexcept
{
	if (this == &rhs)
	{
		return *this;
	}

	delete[] m_elements;
	m_size = std::exchange(rhs.m_size, 0);
	return *this;
}

template<typename T>
T& Array<T>::operator[](size_t x)
{
	if (x >= m_size)
	{
		resize(x + _allocSize);
	}

	return m_elements[x];
}

template<typename T>
const T& Array<T>::operator[](size_t x) const
{
	if (x >= m_size)
	{
		throw out_of_range{ "" };
	}

	return m_elements[x];
}

template<typename T>
const T& Array<T>::getElementAt(size_t x) const
{
	if (x >= m_size)
	{
		throw std::out_of_range{ "" };
	}
	return m_elements[x];
}

template<typename T>
void Array<T>::setElementAt(size_t x, const T& value)
{
	if (x >= m_size)
	{
		resize(x + _allocSize);
	}
	m_elements[x] = value;
}

template<typename T>
size_t Array<T>::getSize() const noexcept
{
	return m_size;
}

template<typename T>
void Array<T>::resize(size_t newSize)
{
	auto newArray = std::make_unique<T[]>(newSize);

	for (size_t i = 0; i < m_size; ++i)
	{
		newArray[i] = std::move(m_elements[i]);
	}

	delete[] m_elements;
	m_size = newSize;
	m_elements = newArray.release();
}

template class Array<int>;