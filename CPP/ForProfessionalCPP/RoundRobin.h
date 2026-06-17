#pragma once
#include <vector>

template<typename T>
class RoundRobin
{
public:
	explicit RoundRobin(size_t numExpected = 0);
	virtual ~RoundRobin() = default;

	RoundRobin(const RoundRobin& src) = delete;
	RoundRobin& operator=(const RoundRobin& rhs) = delete;

	RoundRobin(RoundRobin&& src) noexcept = default;
	RoundRobin& operator=(RoundRobin&& rhs) noexcept = default;

	void add(const T& element);
	void remove(const T& element);
	T& getNext();

private:
	std::vector<T> m_element;
	typename std::vector<T>::iterator m_nextElement;
};

template<typename T>
inline RoundRobin<T>::RoundRobin(size_t numExpected)
{
	m_element.reserve(numExpected);
	m_nextElement = std::begin(m_element);
}

template<typename T>
inline void RoundRobin<T>::add(const T& element)
{
	ptrdiff_t pos = m_nextElement - std::begin(m_element);
	m_element.push_back(element);
	m_nextElement = std::begin(m_element) + pos;
}

template<typename T>
inline void RoundRobin<T>::remove(const T& element)
{
	for (auto it = std::begin(m_element); it != std::end(m_element); ++it)
	{
		if (*it == element)
		{
			ptrdiff_t newPos;

			if (m_nextElement == std::end(m_element) - 1 && m_nextElement == it)
			{
				newPos = 0;
			}
			else if (m_nextElement <= it)
			{
				newPos = m_nextElement - std::begin(m_element);
			}
			else
			{
				newPos = m_nextElement - std::begin(m_element) - 1;
			}

			m_element.erase(it);
			m_nextElement = std::begin(m_element) + newPos;
			return;
		}
	}
}

template<typename T>
inline T& RoundRobin<T>::getNext()
{
	if (m_element.empty())
	{
		// throw
	}

	auto& toReturn = *m_nextElement;
	
	++m_nextElement;
	if (m_nextElement == std::end(m_element))
	{
		m_nextElement = std::begin(m_element);
	}

	return toReturn;
}