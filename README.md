# Manga-data-base
This database stores the workers and the titles. It also defines the order in which the title is executed.
Example of use
```
#include <iostream>
#include <string>
#include <vector>
#include "dbTitles.h"

int main()
{
	Persons dbPer;
	for (int i = 0; i < 12; i++)
		dbPer.push_person({(unsigned long)i, std::to_string(i) + " name", std::to_string(i)});

	Titles dbTitls;
	ptrTitle tempTitle = dbTitls.CreateTitle();
	tempTitle->setName("name");
	tempTitle->setSuccession("((raw>clean)&translate)>edit");
	tempTitle->pushPerson(dbPer.search(0), (Role)(Role::raw | Role::clean));
	tempTitle->pushPerson(dbPer.search(1), (Role)(Role::raw | Role::clean));
	tempTitle->pushPerson(dbPer.search(2), (Role)(Role::raw | Role::edit | Role::qc));

	tempTitle->removePost(Role::qc);

	std::cout << tempTitle->getRolesEqual() << '\n';

	return 0;
}
```
