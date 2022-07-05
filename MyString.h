#pragma once
#include <iostream>
#include <cstring>
class MyString
{
private:
	char* str = NULL;
	size_t size = 0;
	size_t capacity = 0;

public:
	MyString()
	{
		str = new char[1];
		capacity = 1;
		size = 0;
		str[0] = 0;
	}
	MyString(const char s)
	{
		size = 1;
		capacity = size + 1;
		str = new char[size + 1];
		str[0] = s;
		str[1] = 0;
	}
	MyString(const char* s)
	{
		size = strlen(s);
		capacity = size + 1;
		str = new char[size + 1];
		strncpy(str, s, size + 1);
	}
	MyString(const MyString& obj)
	{
		size = obj.size;
		capacity = size + 1;
		str = new char[size + 1];
		strncpy(str, obj.str, size + 1);
	}
	MyString& operator=(const MyString& obj)
	{
		destroy();
		size = obj.size;
		capacity = size + 1;
		str = new char[size + 1];
		strncpy(str, obj.str, size + 1);
		return *this;
	}
	MyString(MyString&& obj)
	{
		size = obj.size;
		capacity = size + 1;
		str = obj.str;
		obj.str = NULL;
	}
	MyString& operator=(MyString&& obj)
	{
		destroy();
		size = obj.size;
		capacity = size + 1;
		str = obj.str;
		obj.str = NULL;
		return *this;
	}
	void destroy() {
		if (str != NULL)
		{
			delete[] str;
		}
		capacity = 0;
		size = 0;
	}
	~MyString()
	{
		destroy();
	}
	void check()
	{
		if (1 + size >= capacity) {
			char* temp = new char[2 * capacity];
			for (size_t i = 0; i < capacity; i++) {
				temp[i] = str[i];
			}
			delete[] str;
			capacity *= 2;
			str = temp;
		}
	}
	void push(const char ch)
	{
		check();
		str[size] = ch;
		size++;
		str[size] = 0;
	}
	void append(const char* obj)
	{
		for (size_t i = 0; i < strlen(obj); i++) {
			push(obj[i]);
		}
	}
	void append(const MyString& obj)
	{
		for (size_t i = 0; i < obj.size; i++) {
			push(obj.str[i]);
		}
	}
	size_t getLength() const { return size; }
	const char* getStr() const { return str; }
	void reverse()
	{
		for (size_t i = 0; i < size / 2; i++)
		{
			char t = str[i];
			str[i] = str[size - 1 - i];
			str[size - 1 - i] = t;
		}
	}
	MyString subst(const size_t start, const size_t end) const
	{
		char* temp = new char[end - start + 1];
		for (size_t i = start; i < end; i++)
		{
			temp[i - start] = str[i];
		}
		temp[end - start] = 0;
		MyString ans(temp);
		delete[] temp;
		return ans;
	}
	char get(const size_t n) const { return str[n]; }
	void set(const size_t n, const char c) { str[n] = c; }
	void print()const { std::cout << getStr() << "\n"; }
	bool operator==(const MyString& obj)const
	{
		if (size != obj.size)
		{
			return 0;
		}
		for (size_t i = 0; i < size; i++)
		{
			if (str[i] != obj.str[i])
			{
				return 0;
			}
		}
		return 1;
	}
	bool operator!=(const MyString& obj) const { return !(*this == obj); }
};