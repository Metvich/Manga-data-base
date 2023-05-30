#include <string>
#include <vector>
#include <list>
#include <set>
#include <memory>
#include "Date.h"
#include "Role.h"
#include "dbPersons.h"
#include "Succession.h"

#pragma once
#ifndef _there_is_Titles
#define _there_is_Titles

struct Chapther
{
	int chapther = 0;
	int underChapther = 0;
};
struct Post
{
	std::vector<ptrPerson> persons;
	Role role;
	Chapther lastChapther;
};

typedef std::shared_ptr<Post> ptrPost;

class Title;
typedef std::shared_ptr<Title> ptrTitle;

class Title
{
public:
	// Устанавливает имя тайтла
	inline void setName(const std::string &name)
	{
		this->nameTitle = name;
	}

	// Возвращает имя тайтла
	inline std::string getName()
	{
		return std::string(this->nameTitle);
	}

	// Добавляет сслыки; если ссылка уже есть, то возвращает false, иначе true
	bool pushLink(const Link &link);

	// Возвращает ссылку по имени; если ссылки нет, то вернёт false, иначе true
	Link getLink(const std::string &name);

	// Удаляет сслыку по имени; если удалит, то вернёт true, иначе false
	bool removeLink(const std::string &name);

	// Добавляет человека; если человек добавился, то вернёт true, иначе false
	bool pushPerson(const ptrPerson &person, Role roles);

	// Ищет человека по id дискорда
	ptrPerson searchPerson(int id_discord);

	// Возвращает пост с определённой ролью
	ptrPost getPost(const Role &role);

	// Удаляет пост по ролям
	bool removePost(const Role &roles);

	// Удаляет человека по id дискорда с постов по ролям(если Role::none, то со всех)
	bool removePerson(int id_discord, const Role &roles = Role::none);

	// Устанавлявает последовательность
	inline int setSuccession(const std::string &command)
	{
		return this->queue.setCommand(command);
	}

	// Устанавливает готовность в последоватлеьности
	inline bool setReadySuccession(const Role &roles, const bool &ready)
	{
		return this->queue.setReady(roles, ready);
	}

	// Устанавливает готовность всем в последовательности
	inline bool setAllReadySuccession(const bool &ready)
	{
		return this->queue.setAllReady(ready);
	}

	// Находит роли, которые не готовы, и возвращает их
	inline Role getUnReady()
	{
		return this->queue.getUnReady();
	}

	// Возвращает последовательность в виде строки
	inline std::string getSuccession()
	{
		return this->queue.getCommad();
	}

	// Возвращает роли, используемые в последовательности выполнения работы
	inline Role getRolesSuccession()
	{
		return this->queue.getRoles();
	}

	// Возвращает роли, присутствующие на постах
	Role getRoles();

	// Возвращает роли, которых не хватает для начала работы
	inline Role getRolesNotEqual()
	{
		return eraseRole(this->queue.getRoles(), this->getRoles());
	}

	// Возвращает роли на постах, неиспользуемые в последовательности
	inline Role getRolesEqual()
	{
		return eraseRole(this->getRoles(), this->queue.getRoles());
	}

private:
	std::string nameTitle;		// Имя тайтла
	std::vector<ptrPost> posts; // Массив постов, распределённые по ролям
	Succession queue;			// Очерёдность выполнения
	std::vector<Link> links;	// Ссылки
	Date deadLine;				// Крайний срок
	Date dateLastChapther;		// Дата выпуска последней главы
};

class Titles
{
public:
	// Создает тайтл в базе
	ptrTitle CreateTitle();

	// Ищет тайтл по id
	std::pair<int, ptrTitle> search(int id);

	// Ищет тайтл по имени
	std::pair<int, ptrTitle> search(std::string nameTitle);

	// Удаляет тайтл из базы по id
	bool remove(int id);

	// Удаляет тайтл из базы по имени
	bool remove(std::string nameTitle);

	// Возвращает кол-во тайтлов
	inline int getCountTitles()
	{
		return this->titles.size();
	}

private:
	std::list<std::pair<int, ptrTitle>> titles; // База с тайтлами
	std::set<int> free_id;						// Освободившиеся id
};
#endif // !_there_is_Titles