#pragma once
#include <queue>

template<typename T>
class Queue
{
public:
	explicit Queue(size_t maxSize = 0);
	virtual ~Queue() = default;

	bool bufferPacket(const T& packet);

	[[nodiscard]] T getNextPacket();

private:
	std::queue<T> _packets;
	size_t _maxSize;
};

template<typename T>
inline Queue<T>::Queue(size_t maxSize)
	: _maxSize(maxSize)
{
}

template<typename T>
inline bool Queue<T>::bufferPacket(const T& packet)
{
	if (_maxSize > 0 && _packets.size() == _maxSize)
	{
		return false;
	}

	_packets.push(packet);
	return true;
}

template<typename T>
inline T Queue<T>::getNextPacket()
{
	if (_packets.empty())
	{
		throw 1;
	}

	T temp = _packets.front();
	_packets.pop();
	return temp;
}
