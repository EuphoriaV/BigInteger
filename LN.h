#pragma once
#include "MyString.h"
#include <string_view>

#include <iostream>
#include <fstream>
#include <string>
#include <climits>
class LN
{
private:
	MyString number;
	bool negative;
	bool nan;
	void removeZeroes();
	void turnNegate();
	void constructFromString(const MyString temp);
	int to(const char a) const { return a - 48; }
	char from(const int a) const { return char(a + 48); }
	LN plus(const LN& obj) const;
	LN minus(const LN& obj) const;
	LN negate() const;
	LN abs() const;
	LN sqrt() const;
	LN add(const LN& obj) const;
	LN subtract(const LN& obj) const;
	LN multiply(const LN& obj) const;
	LN divide(const LN& obj) const;
	LN mod(const LN& obj) const;
	int compare(const LN& obj) const;

public:
	LN();
	LN(const std::string st, const bool neg, const bool nann);
	LN(const long long val);
	LN(const char* val);
	LN(const std::string_view val);
	LN(const MyString obj);
	LN(const LN& obj);
	LN(LN&& obj);
	LN& operator=(const LN& obj);
	LN& operator=(LN&& obj);
	bool operator>(const LN& obj) const { return (nan || obj.nan) ? 0 : (this->compare(obj) == 1); }
	bool operator<(const LN& obj) const { return (nan || obj.nan) ? 0 : (this->compare(obj) == -1); }
	bool operator>=(const LN& obj) const { return (nan || obj.nan) ? 0 : (this->compare(obj) != -1); }
	bool operator<=(const LN& obj) const { return (nan || obj.nan) ? 0 : (this->compare(obj) != 1); }
	bool operator==(const LN& obj) const { return (nan || obj.nan) ? 0 : (this->compare(obj) == 0); }
	bool operator!=(const LN& obj) const { return (nan || obj.nan) ? 1 : (this->compare(obj) != 0); }
	LN operator+(const LN& obj) const;
	LN operator-(const LN& obj) const;
	LN operator*(const LN& obj) const;
	LN operator/(const LN& obj) const;
	LN operator%(const LN& obj) const;
	LN operator~() const;
	LN operator-() const;
	LN operator+=(const LN& obj);
	LN operator-=(const LN& obj);
	LN operator*=(const LN& obj);
	LN operator/=(const LN& obj);
	LN operator%=(const LN& obj);
	operator long long() const;
	operator bool() const { return *this != LN("0"); }
	void toString(std::ofstream& out) const;
	bool checkBadAllocation() { return number.getLength() == 0; }
};
LN operator"" _ln(const char* str);