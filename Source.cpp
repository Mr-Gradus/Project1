#include <iostream>
#include <string>
#include <optional>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>



struct Person {
	
	std::string surname;
	std::string name;
	std::optional<std::string> patronymic;

};

std::optional<std::string> getPatStr(std::string& patr)
{
	if ( patr == "-")
	{
		return std::nullopt;
	}

	return patr;
}

std::ostream& operator<< (std::ostream& out, const Person& person)
{
	out <<  std::setw(10) << person.surname << std::setw(13) << person.name;

	if (person.patronymic.has_value())
	{
		out << std::setw(13) << person.patronymic.value();
	}
	else
	{
		out << std::setw(14);
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

std::optional<int> getAddNum(std::string& add)
{
	if (add == "-")
	{
		return std::nullopt;
	}

	return std::stoi(add); 
}

std::ostream& operator<< (std::ostream& out, const PhoneNumber& pn)
{
	out << std::setw(7) << '+' << pn.pageCode << '(' << pn.cityCode << ')' << pn.phoneNum;

	if (pn.addNum.has_value())
	{
		out << std::setw(8) << pn.addNum.value();
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
	std::vector<std::pair<Person, PhoneNumber>> m_contacts;


public:

	PhoneBook(std::ifstream& in);

	~PhoneBook() {}
	
	friend std::ostream& operator<< (std::ostream& out, const PhoneBook& pb);

	void SortByName();

	void SortByPhone();
		
	void ChangePhoneNumber(const Person& person, const PhoneNumber& newpn);

	std::pair<std::string, std::vector<PhoneNumber>> GetPhoneNumber(const std::string& surname)
	{
		std::vector<PhoneNumber> phoneNums;
		int count = 0;

		std::for_each(m_contacts.begin(), m_contacts.end(), [&](const auto& entry)
			{
				if (entry.first.surname == surname)
				{
					phoneNums.push_back(entry.second);
					++count;
				}
				
			});

				
		switch (count)
		{
		case 0:
			return { "not found", phoneNums };
		case 1:
			return { "", phoneNums };
		default:
			return { "found more than 1", phoneNums };
		}
	}
	
};

PhoneBook::PhoneBook(std::ifstream& in)
{
	if (in.is_open())
	{
		std::string surname;
		std::string name;
		std::string patronymic;

		int pageCode;
		int cityCode;
		std::string phoneNum;
		std::string addNumber;

		while (!in.eof()) // До конца файла
		{
			in >> surname >> name >> patronymic >> pageCode >> cityCode >> phoneNum >> addNumber;

			std::optional <std::string> patr; // по умолчанию равен nullopt

			if (patronymic != "-") // Если в файле стоит прочерк
				patr = patronymic; // то optional заполняется данными и перестает быть nullopt

			std::optional <int> add;
			if (addNumber != "-")
				add = std::stoi(addNumber); // то же самое



			m_contacts.emplace_back(Person{ surname, name, patr }, PhoneNumber{ pageCode, cityCode, phoneNum, add });
		}
		in.close();
	}
	else
	{
		std::cout << "Error openning file." << std::endl;
	}
}

std::ostream& operator<< (std::ostream& out, const PhoneBook& pb)
{
	for (const auto& [first, second] : pb.m_contacts)
	{
		out << first << ' ' << second << std::endl;
	}
	return out;
}

void PhoneBook::SortByName()
{
	std::sort(m_contacts.begin(), m_contacts.end(), [](const std::pair<Person, PhoneNumber>& more, \
		const std::pair<Person, PhoneNumber>& less) {
			return more.first < less.first;
		});
}

void PhoneBook::SortByPhone()
{
	std::sort(m_contacts.begin(), m_contacts.end(), [](const std::pair<Person, PhoneNumber>& more, \
		const std::pair<Person, PhoneNumber>& less) {
			return more.second < less.second;
		});
}

void PhoneBook::ChangePhoneNumber(const Person& person, const PhoneNumber& newpn)
{
	auto find = std::find_if(m_contacts.begin(), m_contacts.end(), [&](const auto& find)
		{
			return find.first == person;
		});
	if (find != m_contacts.end())
	{
		find->second = newpn;
	}
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

	std::cout << "-----GetPhoneNumber-----" << std::endl;
	// лямбда функция, которая принимает фамилию и выводит номер телефона этого    	человека, либо строку с ошибкой
	
	auto print_phone_number = [&book](const std::string& surname) {
		std::cout << surname << "\t";
		auto answer = book.GetPhoneNumber(surname);
		if (get<0>(answer).empty())
			for (size_t i = 0; i < get<1>(answer).size(); ++i)
			{
				std::cout << get<1>(answer)[i];
			}
		else
			std::cout << get<0>(answer);
		std::cout << std::endl;
	};

	print_phone_number("Bublik");
	print_phone_number("Rybin");

	std::cout << "----ChangePhoneNumber----" << std::endl;
	book.ChangePhoneNumber(Person{ "Rybin", "Dmitry", "Valerievich" }, PhoneNumber{ 7, 922, "0341020", 13 });
	book.ChangePhoneNumber(Person{ "Hohryakov", "Denis", "Vadimovich" }, PhoneNumber{ 16, 465, "9155448", 13 });
	std::cout << book;


	return 0;
}