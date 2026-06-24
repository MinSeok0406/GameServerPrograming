#pragma once
#include <set>
#include <string>
#include <vector>

class AccessList
{
public:
	AccessList() = default;
	AccessList(std::initializer_list<std::string_view> users)
	{
		_allowed.insert(std::cbegin(users), std::cend(users));
	}

	void addUser(std::string user)
	{
		_allowed.emplace(std::move(user));
	}

	void removeUser(const std::string& user)
	{
		_allowed.erase(user);
	}

	bool isAllowed(const std::string& user) const
	{
		auto it = _allowed.find(user);
		return (it != std::cend(_allowed));
	}
	
	std::vector<std::string> getAllUsers() const
	{
		return { std::cbegin(_allowed), std::cend(_allowed) };
	}

private:
	std::set<std::string> _allowed;
};