
#include <string>
#include <ctime>

#pragma once
#ifndef _there_is_Date
#define _there_is_Date

class Date
{
public:
	// Конструктор создания
	Date(const std::string &timeDay, const std::string &dateDay);

	// Конструктор копирования
	Date(const Date &date);

	// Конструктор создания
	Date(std::tm &date);

	// Конструктор по умолчанию
	Date();

	// Переводит строки в структуру и записывает дату
	bool setDate(std::string timeDay, std::string dateDay);

	// Записывает дату по инфромации из структуры
	bool setDate(std::tm &date);

	// Возвращает структуру, заполненную информацией о дате
	std::tm getDateStruct();

	// Возвращает строку, в которой записана информация о дате
	std::string GetDateString();

	// Возвращает разницу в минутах между датами
	int32_t DateDif(const Date &other);

	// Добавляет к дате года
	void increaseYear(const int &countYear);

	// Добавляет к дате дни
	void increaseDay(const int &countDay);

	// Добавляет к дате минуты
	void increaseMinute(const int &countMinute);

private:
	std::time_t time;
};

#endif // !_there_is_Date