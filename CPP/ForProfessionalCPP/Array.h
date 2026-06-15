#pragma once
template<typename T>
class Array
{
public:
	Array();
	virtual ~Array();

	Array& operator=(const Array& rhs) = delete;
	Array(const Array& src) = delete;

	Array(Array&& src) noexcept;
	Array& operator=(Array&& rhs) noexcept;

	T& operator[](size_t x);
	const T& operator[](size_t x) const;

	const T& getElementAt(size_t x) const;
	void setElementAt(size_t x, const T& value);
	size_t getSize() const noexcept;

private:
	static const size_t _allocSize = 4;
	void resize(size_t newSize);
	T* m_elements = nullptr;
	size_t m_size = 0;
};