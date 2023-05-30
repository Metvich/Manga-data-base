#include <string>
#include <vector>

#pragma once
#ifndef _there_is_Role
#define _there_is_Role

const unsigned int countRole = 11;	   // Кол-во ролей
const std::string nameRoles[countRole] // Массив имён ролей
	{
		"none",
		"raw",
		"waifu",
		"clean",
		"translate",
		"edit",
		"sound",
		"soundtranslate",
		"typeset",
		"qc",
		"exporter"};

enum Role
{
	none = 0x0000,
	raw = 0x0001,
	waifu = 0x0002,
	clean = 0x0004,
	translate = 0x0008,
	edit = 0x0010,
	sound = 0x0020,
	soundtranslate = 0x0040,
	typeset = 0x0080,
	qc = 0x0100,
	exporter = 0x0200
};

typedef std::vector<Role> vectorRole;

template <class TFunLambda>
// Все найденные роли передаёт в функцию
void doAllRole(const Role &roles, TFunLambda lambda)
{
	int temp;
	for (int i = 0; i < (countRole - 1); i++)
	{
		temp = (roles & (1 << i));
		if (temp != 0)
			lambda(static_cast<Role>(temp));
	}
}

// Переводит номер роли в имя роли
std::string numToName(Role role);

// Переводит имя роли в номер роли
Role nameToNum(const std::string &nameRole);

// Вырезает из переменной лишнии роли
inline Role eraseRole(const Role &roles, const Role &excessRoles)
{
	return static_cast<Role>(roles & (~excessRoles));
}

// Добавляет роли к переменной
inline Role pushRole(const Role &roles, const Role &needRoles)
{
	return static_cast<Role>(roles | needRoles);
}

// Возвращает true, если есть пересекающиеся роли, иначе false
inline bool crossRole(const Role &roles_1, const Role &roles_2)
{
	return (roles_1 & roles_2) != 0;
}

// Переводит роли в вектор отдельных ролей
vectorRole extractRole(const Role &roles);

#endif // !_there_is_Role