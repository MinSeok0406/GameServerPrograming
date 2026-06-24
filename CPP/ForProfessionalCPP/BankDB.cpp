#include "BankDB.h"

bool BankDB::addAccount(const BankAccount& account)
{
	auto ret = _accounts.emplace(account.getAccountNumber(), account);

	return ret.second;
}

void BankDB::deleteAccount(int accountNumber)
{
	_accounts.erase(accountNumber);
}

BankAccount& BankDB::findAccount(int accountNumber)
{
	auto it = _accounts.find(accountNumber);
	if (it == std::end(_accounts))
	{
		throw 1;
	}

	return it->second;
}

BankAccount& BankDB::findAccount(std::string_view name)
{
	for (auto& account : _accounts)
	{
		if (account.second.getClientName() == name)
		{
			return account.second;
		}
	}

	throw 1;
}

void BankDB::mergeDatabase(BankDB& db)
{
	_accounts.merge(db._accounts);
	
	db._accounts.clear();
}