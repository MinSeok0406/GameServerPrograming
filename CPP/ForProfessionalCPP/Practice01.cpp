#include <iostream>
using namespace std;

template<typename T>
class Rational
{
public:
	Rational(const T& num = 0, const T& den = 1) : _num(num), _den(den)
	{

	}

	const T number() const { return _num; }
	const T denomi() const { return _den; }

	friend const Rational operator*(const Rational& lhs, const Rational& rhs)
	{
		return Rational(lhs.number() * rhs.number(), lhs.denomi() * rhs.denomi());
	}

private:
	T _num;
	T _den;
};

int wmain()
{
	Rational<int> r(4, 1);
	Rational<int> result;

	result = r * 2;
	result = 2 * r;

    return 0;
}