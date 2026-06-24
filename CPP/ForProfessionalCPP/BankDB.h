#pragma once
#include <string>
#include <map>

class BankAccount
{
public:
	BankAccount(int accountNumber, std::string name)
		: _accountNumber(accountNumber), _clientName(std::move(name))
	{

	}

	void setAccountNumber(int accountNumber)
	{
		_accountNumber = accountNumber;
	}

	int getAccountNumber() const
	{
		return _accountNumber;
	}

	void setClientName(std::string name)
	{
		_clientName = std::move(name);
	}

	const std::string& getClientName() const
	{
		return _clientName;
	}

private:
	int _accountNumber;
	std::string _clientName;
};

class BankDB
{
public:
	bool addAccount(const BankAccount& account);
	void deleteAccount(int accountNumber);

	BankAccount& findAccount(int accountNumber);
	BankAccount& findAccount(std::string_view name);

	void mergeDatabase(BankDB& db);

private:
	std::map<int, BankAccount> _accounts;
};