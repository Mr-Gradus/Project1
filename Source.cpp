#include <iostream>
#include <string>
#include <optional>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

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
	return std::tie(person1.surname, person1.name, person1.patronymic) < std::tie(person2.surname, person2.name, person2.patronymic);
}

bool operator== (const Person& person1, const Person& person2)
{
	return std::tie(person1.surname, person1.name, person1.patronymic) == std::tie(person2.surname, person2.name, person2.patronymic);
}

struct PhoneNumber {

	int pageCode;
	int cityCode;
	std::string phoneNum;
	std::optional<int> addNum;
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
	return std::tie(pn1.pageCode, pn1.cityCode, pn1.phoneNum, pn1.addNum) < std::tie(pn2.pageCode, pn2.cityCode, pn2.phoneNum, pn2.addNum);
}

bool operator== (const PhoneNumber& pn1, const PhoneNumber& pn2)
{
	return std::tie(pn1.pageCode, pn1.cityCode, pn1.phoneNum, pn1.addNum) == std::tie(pn2.pageCode, pn2.cityCode, pn2.phoneNum, pn2.addNum);
}


class PhoneBook {
private:
	std::vector<std::pair<Person, PhoneNumber>> m_contact;


public:

	PhoneBook(std::ifstream &in)
	{
		if (in.is_open())
		{
			std::string surname;
			std::string name;
			std::string patronymic;

			int pageCode;
			int cityCode;
			std::string number;
			std::string addNumber;

			while (!in.eof()) // До конца файла
			{
				in >> surname >> name >> patronymic >> pageCode >> cityCode >> number >> addNumber;
				std::optional <std::string> patr; // по умолчанию равен nullopt

				if (patronymic != "-") // Если в файле стоит прочерк
					patr = patronymic; // то optional заполняется данными и перестает быть nullopt

				std::optional <int> add;
				if (addNumber != "-")
					add = std::stoi(addNumber); // то же самое
				m_contact.emplace_back(Person{ surname, name, patr }, PhoneNumber{ pageCode, cityCode, number, add });
			}
			in.close();
		}
		else
		{
			std::cout << "Error openning file." << std::endl;
		}
	}

	~PhoneBook() {}
	
	friend std::ostream& operator<< (std::ostream& out, const PhoneBook& pb);

	void SortByName();

	void SortByPhone();


};

std::ostream& operator<< (std::ostream& out, const PhoneBook& pb)
{
	for (const auto& [first, second] : pb.m_contact)
	{
		out << first << " " << second << std::endl;
	}
	return out;
}

void PhoneBook::SortByName()
{
	std::sort(m_contact.begin(), m_contact.end(), [](const std::pair<Person, PhoneNumber>& more, \
		const std::pair<Person, PhoneNumber>& less) {
			return more.first < less.first;
		});
}

void PhoneBook::SortByPhone()
{
	std::sort(m_contact.begin(), m_contact.end(), [](const std::pair<Person, PhoneNumber>& more, \
		const std::pair<Person, PhoneNumber>& less) {
			return more.second < less.second;
		});
}

int main()
{
	std::ifstream file("Phonebook.txt"); // путь к файлу PhoneBook.txt
	PhoneBook book(file);
	std::cout << book;

	std::cout << "------SortByPhone-------" << std::endl;
	book.SortByPhone();
	std::cout << book;

	std::cout << "------SortByName--------" << std::endl;
	book.SortByName();
	std::cout << book;
/*
	std::cout << "-----GetPhoneNumber-----" << std::endl;
	// лямбда функция, которая принимает фамилию и выводит номер телефона этого    	человека, либо строку с ошибкой
	auto print_phone_number = [&book](const std::string& surname) {
		std::cout << surname << "\t";
		auto answer = book.GetPhoneNumber(surname);
		if (get<0>(answer).empty())
			std::cout << get<1>(answer);
		else
			std::cout << get<0>(answer);
		std::cout << std::endl;
	};

	print_phone_number("Ivanov");
	print_phone_number("Petrov");

	std::cout << "----ChangePhoneNumber----" << std::endl;
	book.ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" }, PhoneNumber{ 7, 123, "15344458", std::nullopt });
	book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" }, PhoneNumber{ 16, 465, "9155448", 13 });
	std::cout << book;
	*/

	return 0;
}