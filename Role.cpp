#include "Role.h"

std::string numToName(Role role)
{
	int i = 1, intRole = role;
	while ((intRole & 0x1) == 0)
	{
		intRole = intRole >> 1;
		i++;
	}
	return nameRoles[i];
}

Role nameToNum(const std::string &nameRole)
{
	for (int i = 1; i < countRole; i++)
		if (nameRole == nameRoles[i])
			return static_cast<Role>(1 << (i - 1));
	return none;
}

vectorRole extractRole(const Role &roles)
{
	vectorRole result;
	if (roles == none)
		result.push_back(none);
	else
	{
		int temp;
		for (int i = 0; i < (countRole - 1); i++)
		{
			temp = (roles & (1 << i));
			if (temp != 0)
				result.push_back(std::move(static_cast<Role>(temp)));
		}
	}
	return result;
}