#include <iostream>
#include <string>
#include <optional>


struct Person {
	
	std::string surname{"Rybin"};
	std::string name{"Ivan"};
	std::optional<std::string> patronymic {"Dmitrievich"};

};

std::optional<std::string> getPatStr(std::string& pat)
{
	if ( pat == "")
	{
		return std::nullopt;
	}

	return pat;
}

std::ostream& operator<< (std::ostream& out, const Person& person)
{
	out << person.surname << "   " << person.name << "   ";

	if (person.patronymic.has_value())
	{
		out << person.patronymic.value();
	}
	else
	{
		out << "";
	}
	return out;
}

bool operator< (const Person& person1, const Person& person2)
{
	return std::make_tuple(person1.surname, person1.name, person1.patronymic) < std::make_tuple(person2.surname, person2.name, person2.patronymic);
}

bool operator== (const Person& person1, const Person& person2)
{
	return std::make_tuple(person1.surname, person1.name, person1.patronymic) == std::make_tuple(person2.surname, person2.name, person2.patronymic);
}

struct PhoneNumber {

	int pageCode;
	int cityCode;
	int phoneNum;
	std::optional<int> addNum;
};

std::optional<int> getAddNum(int& addNum)
{
	if ( addNum == 0)
	{
		return std::nullopt;
	}

	return addNum;
}

int main()
{
	Person person;
	std::cout << person;


	return 0;
}