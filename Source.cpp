#include <iostream>
#include <string>
#include <optional>
#include <vector>


struct Person {
	
	std::string surname;
	std::string name;
	std::optional<std::string> patronymic;

};

std::optional<std::string> getPatStr(std::string& p)
{
	if ( p == "")
	{
		return std::nullopt;
	}

	return p;
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

	int pageCode{7};
	int cityCode{343};
	std::string phoneNum{"12345"};
	std::optional<int> addNum{13};
};

std::optional<int> getAddNum(std::string& n)
{
	if (n == "")
	{
		return std::nullopt;
	}

	return stoi(n);
}

std::ostream& operator<< (std::ostream& out, const PhoneNumber& pn)
{
	out << '+' << pn.pageCode << '(' << pn.cityCode << ')' << pn.phoneNum;

	if (pn.addNum.has_value())
	{
		out << ' ' << pn.addNum.value() << std::endl;
	}
	else
	{
		out << std::endl;
	}
	return out;
}

bool operator< (const PhoneNumber& pn1, const PhoneNumber& pn2)
{
	return std::make_tuple(pn1.pageCode, pn1.cityCode, pn1.phoneNum, pn1.addNum) < std::make_tuple(pn2.pageCode, pn2.cityCode, pn2.phoneNum, pn2.addNum);
}

bool operator== (const PhoneNumber& pn1, const PhoneNumber& pn2)
{
	return std::make_tuple(pn1.pageCode, pn1.cityCode, pn1.phoneNum, pn1.addNum) == std::make_tuple(pn2.pageCode, pn2.cityCode, pn2.phoneNum, pn2.addNum);
}


class PhoneBook {
private:
std::vector<pair<Person, PhoneNumber>> m_contact 


public:

	void SortByName();

	void SortByPhone();




};


int main()
{
	ifstream file("ХХХ"); // путь к файлу PhoneBook.txt
	PhoneBook book(file);
	cout << book;

	cout << "------SortByPhone-------" << endl;
	book.SortByPhone();
	cout << book;

	cout << "------SortByName--------" << endl;
	book.SortByName();
	cout << book;

	cout << "-----GetPhoneNumber-----" << endl;
	// лямбда функция, которая принимает фамилию и выводит номер телефона этого    	человека, либо строку с ошибкой
	auto print_phone_number = [&book](const string& surname) {
		cout << surname << "\t";
		auto answer = book.GetPhoneNumber(surname);
		if (get<0>(answer).empty())
			cout << get<1>(answer);
		else
			cout << get<0>(answer);
		cout << endl;
	};

	print_phone_number("Ivanov");
	print_phone_number("Petrov");

	cout << "----ChangePhoneNumber----" << endl;
	book.ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" }, PhoneNumber{ 7, 123, "15344458", nullopt });
	book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" }, PhoneNumber{ 16, 465, "9155448", 13 });
	cout << book;


	return 0;
}