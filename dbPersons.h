#include <string>
#include <vector>
#include <list>
#include <memory>
#include "Role.h"
#include "Date.h"

#pragma once
#ifndef _there_is_Person
#define _there_is_Person

typedef std::pair<std::string, std::string> Link; // Первый элемент - имя; Второй - сслыка
struct Person;
typedef std::shared_ptr<Person> ptrPerson;

struct Person
{
	uint64_t discord_id = 0;
	std::string name;
	std::string nick;
	int16_t timeZone;
	Role roles = none;
	std::vector<Link> links;
	std::vector<std::string> genreBlackList;
	Date dateStart = Date();
};

class Persons
{
public:
	// Добавляет человека в базу по структуре человека
	bool push_person(const Person &person);

	// Ищет человека по id дискорда и возвращает указатель на него
	ptrPerson search(const uint64_t &discord_id);

	// Ищет человека по сслыке и возвращает указатель на него
	ptrPerson search(const std::string &link);

	// Ищет людей по ролям и учитывает чёрный список жанров, возвращает список указателей на подходящих людей
	std::list<ptrPerson> search(const Role &need_roles, const std::vector<std::string> &genreBlackList);

	// Удаляет человека из базы по id дискорда
	bool remove(const uint64_t &discord_id);

	// Возвращает кол-во людей в базе
	int GetCountPerosn();

private:
	std::list<ptrPerson> persons;
};
#endif // !_there_is_Person