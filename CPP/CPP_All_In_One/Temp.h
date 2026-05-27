#pragma once
#include <string>
#include <memory>

class PersonImpl;

class Date;
class Address;

class Person
{
public:
	Person(const std::string& name, const Date& birthday, const Address& addr);

	std::string name() const;
	const Date birthDate() const;
	const Address address() const;

private:
	std::shared_ptr<PersonImpl> pImpl;
};