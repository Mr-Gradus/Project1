#include <iostream>
#include <string>
#include <optional>


struct Person {
	
	std::string surname;
	std::string name;
	std::string patronymic;

};


std::ostream& operator<< (std::ostream& out, const Person& person)
{
	return out << person.surname << " " << person.name << " " << person.patronymic;
}


struct PhoneNumber {

	int pageCode;
	int cityCode;
	int phoneNum;
	int addNum;
};



int main()
{

	Person person;
	std::cout << person;


	return 0;
}