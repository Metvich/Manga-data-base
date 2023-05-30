#include "dbTitles.h"

bool Title::pushLink(const Link &link)
{
	for (int i = 0; i < this->links.size(); i++)
		if (link.first == this->links[i].first || link.second == this->links[i].second)
			return false;
	this->links.push_back(link);
	return true;
}

Link Title::getLink(const std::string &name)
{
	for (int i = 0; i < this->links.size(); i++)
		if (name == this->links[i].first)
			return Link(this->links[i]);
	return Link();
}

bool Title::removeLink(const std::string &name)
{
	for (int i = 0; i < this->links.size(); i++)
		if (name == this->links[i].first)
		{
			this->links.erase(links.begin() + i);
			return true;
		}
	return false;
}

bool Title::pushPerson(const ptrPerson &person, Role roles)
{
	bool tempResult = false;
	for (int i = 0; i < this->posts.size(); i++)
		if ((roles & this->posts[i]->role) != 0)
		{
			roles = eraseRole(roles, this->posts[i]->role);
			bool temp = true;
			for (int j = 0; j < this->posts[i]->persons.size(); j++)
				if (person.get() == this->posts[i]->persons[j].get())
				{
					temp = false;
					break;
				}
			if (temp)
			{
				tempResult = true;
				this->posts[i]->persons.push_back(person);
			}
		}
	if (roles != Role::none)
	{
		tempResult = true;
		doAllRole(roles, [&](const Role &roles)
				  {
			this->posts.push_back(ptrPost(new Post()));
			this->posts.rbegin()->get()->role = roles;
			this->posts.rbegin()->get()->persons.push_back(person); });
	}
	return tempResult;
}

ptrPerson Title::searchPerson(int id_discord)
{
	for (int i = 0; i < this->posts.size(); i++)
		for (int j = 0; j < this->posts[i]->persons.size(); j++)
			if (id_discord == this->posts[i]->persons[j]->discord_id)
				return ptrPerson(this->posts[i]->persons[j]);
	return nullptr;
}

ptrPost Title::getPost(const Role &role)
{
	for (int i = 0; i < this->posts.size(); i++)
		if (this->posts[i]->role == role)
			return ptrPost(this->posts[i]);
	return nullptr;
}

bool Title::removePost(const Role &roles)
{
	bool temp = false;
	for (int i = 0; i < this->posts.size(); i++)
		if (crossRole(roles, this->posts[i]->role))
		{
			temp = true;
			this->posts.erase(this->posts.begin() + i);
		}
	return temp;
}

bool Title::removePerson(int id_discord, const Role &roles)
{
	bool tempResult = false;
	for (int i = 0; i < this->posts.size(); i++)
		for (int j = 0; j < this->posts[i]->persons.size(); j++)
			if (id_discord == this->posts[i]->persons[j]->discord_id && (roles == Role::none || crossRole(roles, this->posts[i]->role)))
			{
				this->posts[i]->persons.erase(this->posts[i]->persons.begin() + j);
				tempResult = true;
			}
	return tempResult;
}

Role Title::getRoles()
{
	int temp = 0;
	for (int i = 0; i < this->posts.size(); i++)
		temp = temp | (this->posts[i]->role);
	return static_cast<Role>(temp);
}

ptrTitle Titles::CreateTitle()
{
	this->titles.push_back(std::pair<int, ptrTitle>((this->free_id.empty() ? this->titles.size() : *(this->free_id.begin())), new Title()));
	if (!(this->free_id.empty()))
		this->free_id.erase(free_id.begin());
	return ptrTitle(this->titles.rbegin()->second);
}

std::pair<int, ptrTitle> Titles::search(int id)
{
	for (auto &el : this->titles)
		if (id == el.first)
			return std::pair<int, ptrTitle>(el);
	return std::pair<int, ptrTitle>(-1, nullptr);
}

std::pair<int, ptrTitle> Titles::search(std::string nameTitle)
{
	for (auto &el : this->titles)
		if (nameTitle == el.second->getName())
			return std::pair<int, ptrTitle>(el);
	return std::pair<int, ptrTitle>(-1, nullptr);
}

bool Titles::remove(int id)
{
	for (auto it = this->titles.begin(); it != this->titles.end(); ++it)
		if (it->first == id)
		{
			this->titles.erase(it);
			this->free_id.emplace(id);
			return true;
		}
	return false;
}

bool Titles::remove(std::string nameTitle)
{
	for (auto it = this->titles.begin(); it != this->titles.end(); ++it)
		if (it->second->getName() == nameTitle)
		{
			this->titles.erase(it);
			this->free_id.emplace(it->first);
			return true;
		}
	return false;
}