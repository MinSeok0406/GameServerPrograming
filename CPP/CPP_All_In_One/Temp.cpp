#include "Temp.h"
#include "Address.h"
using namespace std;

Person::Person(const std::string& name, const Date& birthday, const Address& addr)
	: pImpl(new PersonImpl(name, birthday, addr))
{
}

std::string Person::name() const
{
	return pImpl->getName();
}

const Date Person::birthDate() const
{
	return pImpl->getDate();
}

const Address Person::address() const
{
	return pImpl->getAddress();
}
