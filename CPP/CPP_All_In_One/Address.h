#pragma once
#include "Date.h"
#include <string>

class Address
{
public:


private:

};

class PersonImpl
{
public:
	PersonImpl(const std::string& name, const Date& date, const Address& address)
		: theName(name), theBirthDate(date), theAddress(address)
	{

	}

	const std::string getName()
	{
		return theName;
	}

	const Date getDate()
	{
		return theBirthDate;
	}

	const Address getAddress()
	{
		return theAddress;
	}

private:
	std::string theName;
	Date theBirthDate;
	Address theAddress;
};