#include "Date.h"

Date::Date(const std::string &timeDay, const std::string &dateDay)
{
	setDate(timeDay, dateDay);
}

Date::Date(const Date &date)
{
	this->time = date.time;
}

Date::Date(std::tm &date)
{
	setDate(date);
}

Date::Date()
{
	this->time = std::time(0);
}

bool Date::setDate(std::string timeDay, std::string dateDay)
{
	if (timeDay.length() != 5)
		return false;
	for (int i = 0; i < 5; i++)
		if ((timeDay[i] < '0' || timeDay[i] > '9') && i != 2)
			return false;

	if (dateDay.length() != 10)
		return false;
	for (int i = 0; i < 10; i++)
		if ((dateDay[i] < '0' || dateDay[i] > '9') && i != 2 && i != 5)
			return false;

	std::tm t;
	t.tm_sec = 0;
	t.tm_min = std::stoi(timeDay.substr(3, 2));
	t.tm_hour = std::stoi(timeDay.substr(0, 2));
	t.tm_mday = std::stoi(dateDay.substr(0, 2));
	t.tm_mon = std::stoi(dateDay.substr(3, 2)) - 1;
	t.tm_year = std::stoi(dateDay.substr(6, 4)) - 1900;

	return setDate(t);
}

bool Date::setDate(std::tm &date)
{
	this->time = std::mktime(&date);
	return true;
}

std::tm Date::getDateStruct()
{
	return *(std::localtime(&this->time));
}

std::string Date::GetDateString()
{
	return std::string(std::ctime(&this->time));
}

int32_t Date::DateDif(const Date &other)
{
	return (this->time - other.time) / 60;
}

void Date::increaseYear(const int &countYear)
{
	std::tm *test = std::localtime(&this->time);
	test->tm_year += countYear;
	this->time = std::mktime(test);
}

void Date::increaseDay(const int &countDay)
{
	std::tm *test = std::localtime(&this->time);
	test->tm_yday += countDay;
	this->time = std::mktime(test);
}

void Date::increaseMinute(const int &countMinute)
{
	std::tm *test = std::localtime(&this->time);
	test->tm_min += countMinute;
	this->time = std::mktime(test);
}