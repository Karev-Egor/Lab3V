#include "Table.h"
#include <iostream>
#include <string.h>
#include <string>
#include <stdexcept>
using namespace std;

namespace Hi {

	Item::Item(): m_key(-1), m_busy(0)
	{
		m_info = NULL;
	}

	Item::Item(int key, char* info):m_key(key), m_busy(1)
	{
		int size;	// кол-во символов в элементе
		for (int k = 0;; k++)
		{
			if (info[k] == '\0')
			{
				size = k;
				break;
			}
		}

		m_info = new char(size + 1);
		for (int i = 0; i < size; i++)
			m_info[i] = info[i];
		m_info[size] = '\0';
	}


	Table::Table(const Item & item)
	{
		top = 1;
		mas = new Item[1];
		mas[0] = Item(item);
	}

	Table::Table(const Table & tab)
	{
		top = tab.top;
		mas = new Item[tab.top + 1];
		for (int i = 0; i <= tab.top; i++)
			mas[i] = tab.mas[i];
	}

	Table::Table(Table && tab) : top(tab.top), mas(tab.mas)
	{
		tab.mas = nullptr;
	}

	Table&Table:: operator =(const Table & tab)
	{
		if (this != &tab)
			{
				top = tab.top;
				delete[] mas;
				mas = new Item[tab.top + 1];
				for (int i = 0; i < top; i++)
					mas[i] = tab.mas[i];
			};

		return *this;
	}

	Table&Table:: operator =(Table && tab)
	{
		int tmp = top;
		top = tab.top;
		tab.top = tmp;
		Item *ptr = mas;
		mas = tab.mas;
		tab.mas = ptr;
		return *this;
	}

	int Table::push(Item item)
	{
		Item *old = mas;
		mas = new Item[top + 1];
		for (int i = 0; i < top; i++)
			mas[i] = old[i];
		if (top > 0)
			delete[] old;
		mas[top++] = item;
		return top;
	};

	bool Table::exist(int key)
	{
		try
		{
			// Проверим - нет ли среди существующих элементов вновь добавляемого.
			for (int i = 0; i < top; ++i)
			{
				if (key == mas[i].m_key)
				{
					throw std::exception("Key not unique");
					//cout << "Key not unique" << endl;
				}
			};
		}
		catch (const std::exception & er)
		{
			std::cout << er.what() << std::endl;
			return true;
		};

		return false;
	};

	Table&Table:: operator +=(Table & tab)
	{
		for (int i = 0; i < tab.top; i++)
		{
			if (!exist(tab.mas[i].m_key))
				push(tab.mas[i]);	// память добавляется на этом этапе.
		}
		return *this;
	}

	void getstr (const char *msg, char *str)
	{
		char* tmp = new char[255];
		int num;

		cout << msg;
		cin >> str;
		while (cin.peek() != '\n')
		{
			cin >> tmp;
			num = strlen(str);
			str[num] = ' ';
			for (int k = 0; k < strlen(tmp); k++)
				str[num + k + 1] = tmp[k];
			str[num + strlen(tmp) + 1] = '\0';
		};
		cin.ignore(255, '\n');	// очистим входной поток до Enter-а
	}

	int getNum(const char *msg)
	{
		int k;
		const char* errmsg = "";
		do
		{
			//std::cout << errmsg << std::endl;
			std::cout << msg;
			std::cin >> k;
			try
			{
				if (!(std::cin.good()) || k < 0)
				{
					std::cin.clear();
					std::cin.ignore(255, '\n');
					throw std::exception("Invalid value, try again\n");
					//errmsg = "Invalid value, try again\n";
				}
				else
				{
					//std::cin.clear();
					std::cin.ignore(255, '\n');
					return k;
				}
			}
			catch (const std::exception & er)
			{
				std::cout << er.what() << std::endl;
			};
		} while (true);
	}

	std::ostream & operator << (std::ostream & s, const Table & tab) 
	{
		Table tmp = tab;
		Item *IT = tmp.getitem();
		for (int i = 0; i < tab.topo(); ++i)
		{
			if (IT[i].m_busy == 1)
				s << "Key-> " << IT[i].m_key << " Info-> " << IT[i].m_info << std::endl;
			else if (IT[i].m_busy == 2)
				s << "Key-> " << IT[i].m_key << " Info-> " << IT[i].m_info << " DELETED" << std::endl;
		}
		return s;
	}


	const char* Table::operator [](int key) const
	{
		for (int i = 0; i < top; ++i)
		{
			if (key == mas[i].m_key && mas[i].m_busy == 1)
			{
				return mas[i].m_info;
			}
		}
		return NULL;
	}

	int Table::operator [](const char* info) const
	{
		for (int i = 0; i < top; ++i)
			if (strcmp(mas[i].m_info, info) == 0)
				return i;
		return -1;
	}


	bool  Table::operator()(int key) const
	{
		for (int i = 0; i < top; ++i)
		{
			if (key == mas[i].m_key && mas[i].m_busy == 1)
				return true;
		}
		return false;

	}


	Table & Table::operator-=(int k)
	{
		this->delElem(k);
		return *this;
	}


	int Table::findindex(int k)
	{
		for (int i = 0; i < top; ++i)
		{
			if (k == mas[i].m_key && mas[i].m_busy == 1)
			{
				return i;
			}
		};
		return -1;
	}


	void Table::clear()
	{
        if (top > 0) 
		{
			Table tmp = *this;
	
			delete [] mas;
			top = 0;
			for (int i = 0; i < tmp.top; i++)
			{
				if (tmp.mas[i].m_busy != 2)
				{
					Table tmp(tmp.mas[i]);
					*this += tmp;
				}
			};
        }
	}

	void Table::SetN(int n)
	{
		int key, nExist;
		char* info = new char[255];

		if (n < 1)
			throw std::length_error("");

		for (int i = 0; i < n; i++)
		{
			cout << "insert key and info" << std::endl;
			key = getNum("Enter key -> ");
			getstr("Enter info -> ", info);

			Item item(key, info);
			Table tmp(item);
			*this += tmp;
		}
	}


	void Table::copy_array_from(Item * dest, const Item * src, int size) 
	{
        for ( int i = 0 ; i < size ; i++ )
		{
			dest[i] = src[i];
		}
	}


	void Table::delElem(int key)
	{
		int ind;
		ind = findindex(key);
		if (ind != -1)
		{
			mas[ind].m_busy = 2; // 2 - элемент удален
		}
		else
			cout << "Key not found\n";
	}
}
