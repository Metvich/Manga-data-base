#include "dbPersons.h"

bool Persons::push_person(const Person &person)
{
	for (auto it = this->persons.begin(); it != this->persons.end(); ++it)
		if (person.discord_id == it->get()->discord_id)
			return false;
	persons.push_back(ptrPerson(new Person(person)));
	return true;
}

ptrPerson Persons::search(const uint64_t &discord_id)
{
	for (auto it = persons.begin(); it != persons.end(); ++it)
	{
		if (it->get()->discord_id == discord_id)
			return *it;
	}
	return nullptr;
}

ptrPerson Persons::search(const std::string &link)
{
	for (auto it = persons.begin(); it != persons.end(); ++it)
	{
		for (int i = 0; i < it->get()->links.size(); i++)
			if (link == it->get()->links[i].second)
				return *it;
	}
	return nullptr;
}

std::list<ptrPerson> Persons::search(const Role &need_roles, const std::vector<std::string> &genreBlackList)
{
	std::list<ptrPerson> result;
	bool temp;
	for (auto it = persons.begin(); it != persons.end(); ++it)
	{
		temp = false;
		if (genreBlackList.size() != 0)
			for (int i = 0; i < it->get()->genreBlackList.size(); i++)
				for (int j = 0; j < genreBlackList.size(); j++)
					if (genreBlackList[i] == it->get()->genreBlackList[j])
					{
						temp = true;
						break;
					}
		if (temp)
			continue;
		if (crossRole(need_roles, it->get()->roles))
			result.push_back(*it);
	}
	return result;
}

bool Persons::remove(const uint64_t &discord_id)
{
	for (auto it = this->persons.begin(); it != this->persons.end(); ++it)
	{
		if (it->get()->discord_id == discord_id)
		{
			this->persons.erase(it);
			return true;
		}
	}
	return false;
}

int Persons::GetCountPerosn()
{
	return static_cast<int>(this->persons.size());
}