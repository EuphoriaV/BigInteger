#include "LN.h"
LN::LN()
{
	negative = false;
	nan = false;
	number = MyString("0");
}
LN::LN(const std::string st, const bool neg, const bool nann)
{
	number = MyString(st.c_str());
	negative = neg;
	nan = nann;
}
LN::LN(const long long val)
{
	if (val >= 0)
	{
		number = MyString(std::to_string(val).c_str());
		negative = false;
	}
	else
	{
		number = MyString(std::to_string(-val).c_str());
		negative = true;
	}
	nan = false;
}
void LN::constructFromString(const MyString temp) {
	if (temp.get(0) == '-')
	{
		negative = true;
		number = temp.subst(1, temp.getLength());
	}
	else
	{
		negative = false;
		number = temp;
	}
	removeZeroes();
	if (number == MyString("0"))
	{
		negative = false;
	}
	nan = false;
}
LN::LN(const char* val)
{
	MyString temp(val);
	constructFromString(temp);
}
LN::LN(const std::string_view val)
{
	MyString temp(val.data());
	constructFromString(temp);
}
LN::LN(const MyString obj)
{
	constructFromString(obj);
}
LN::LN(const LN& obj)
{
	number = obj.number;
	negative = obj.negative;
	nan = obj.nan;
}
LN::LN(LN&& obj)
{
	number = (MyString&&)obj.number;
	negative = obj.negative;
	nan = obj.nan;
}
LN& LN::operator=(const LN& obj)
{
	number = obj.number;
	negative = obj.negative;
	nan = obj.nan;
	return *this;
}
LN& LN::operator=(LN&& obj)
{
	number = (MyString&&)obj.number;
	negative = obj.negative;
	nan = obj.nan;
	return *this;
}
void LN::removeZeroes()
{
	int i = 0;
	while (i + 1 < number.getLength() && number.get(i) == '0')
	{
		i++;
	}
	number = number.subst(i, number.getLength());
}
void LN::turnNegate()
{
	if (number != MyString("0"))
	{
		negative = !negative;
	}
}
int LN::compare(const LN& obj) const
{
	if (negative && !obj.negative)
	{
		return -1;
	}
	if (!negative && obj.negative)
	{
		return 1;
	}
	if (!negative && !obj.negative)
	{
		if (number.getLength() > obj.number.getLength())
		{
			return 1;
		}
		if (number.getLength() < obj.number.getLength())
		{
			return -1;
		}
		for (int i = 0; i < number.getLength(); i++)
		{
			if (number.get(i) > obj.number.get(i))
			{
				return 1;
			}
			if (number.get(i) < obj.number.get(i))
			{
				return -1;
			}
		}
		return 0;
	}
	if (number.getLength() < obj.number.getLength())
	{
		return 1;
	}
	if (number.getLength() > obj.number.getLength())
	{
		return -1;
	}
	for (int i = 0; i < number.getLength(); i++)
	{
		if (number.get(i) < obj.number.get(i))
		{
			return 1;
		}
		if (number.get(i) > obj.number.get(i))
		{
			return -1;
		}
	}
	return 0;
}
LN LN::negate() const
{
	if (nan)
	{
		return LN("NaN", 0, 1);
	}
	LN temp = *this;
	temp.turnNegate();
	return temp;
}
LN LN::abs() const
{
	if (nan)
	{
		return LN("NaN", 0, 1);
	}
	return negative ? this->negate() : *this;
}
LN LN::sqrt() const
{
	if (nan || negative)
	{
		return LN("NaN", 0, 1);
	}
	LN l("0"), r = *this + LN("1");
	while (l + LN("1") < r)
	{
		LN m = (l + r) / LN("2");
		if (m * m <= *this)
		{
			l = m;
		}
		else if (m * m > *this)
		{
			r = m;
		}
	}
	return l;
}
LN LN::add(const LN& obj) const
{
	if (nan || obj.nan)
	{
		return LN("NaN", 0, 1);
	}
	LN first = this->abs(), second = obj.abs();
	if (negative && obj.negative)
	{
		return (first.plus(second)).negate();
	}
	if (!negative && obj.negative)
	{
		return first.subtract(second);
	}
	if (negative && !obj.negative)
	{
		return second.subtract(first);
	}
	LN ans = first.plus(second);
	return ans;
}
LN LN::plus(const LN& obj) const
{
	MyString s, a, b, t;
	if (obj.number.getLength() > number.getLength())
	{
		a = obj.number;
		t = number;
	}
	else
	{
		t = obj.number;
		a = number;
	}
	int dif = a.getLength() - t.getLength();
	for (int i = 0; i < dif; i++)
	{
		b.append("0");
	}
	b.append(t);
	int cur = 0;
	for (int i = a.getLength() - 1; i >= 0; i--)
	{
		int neww = cur + to(a.get(i)) + to(b.get(i));
		if (neww > 9)
		{
			cur = 1;
		}
		else
		{
			cur = 0;
		}
		s.append(from(neww % 10));
	}
	if (cur)
	{
		s.append("1");
	}
	s.reverse();
	return LN(s);
}
LN LN::subtract(const LN& obj) const
{
	if (nan || obj.nan)
	{
		return LN("NaN", 0, 1);
	}
	LN first = this->abs(), second = obj.abs();
	if (!negative && obj.negative)
	{
		return first.plus(second);
	}
	if (negative && !obj.negative)
	{
		return (first.plus(second)).negate();
	}
	if (!negative && !obj.negative)
	{
		if (first > second)
		{
			return first.minus(second);
		}
		return (second.minus(first)).negate();
	}
	if (second > first)
	{
		return second.minus(first);
	}
	return (first.minus(second)).negate();
}
LN LN::minus(const LN& obj) const
{
	MyString s, a = number, t = obj.number, b;

	int dif = a.getLength() - t.getLength();
	for (int i = 0; i < dif; i++)
	{
		b.append("0");
	}
	b.append(t);
	int cur = 0;
	for (int i = a.getLength() - 1; i >= 0; i--)
	{
		int x = to(a.get(i)), y = to(b.get(i));
		if (x - cur >= y)
		{
			s.append(from(x - cur - y));
			cur = 0;
		}
		else
		{
			s.append(from(10 + x - cur - y));
			cur = 1;
		}
	}
	s.reverse();
	return LN(s);
}
LN LN::multiply(const LN& obj) const
{
	if (nan || obj.nan)
	{
		return LN("NaN", 0, 1);
	}
	MyString a, b;
	if (obj.number.getLength() > number.getLength())
	{
		a = obj.number;
		b = number;
	}
	else
	{
		b = obj.number;
		a = number;
	}
	LN ans(0LL);
	for (int i = b.getLength() - 1; i >= 0; i--)
	{
		int cur = 0;
		MyString curString("");
		for (int j = b.getLength() - 1; j > i; j--)
		{
			curString.append("0");
		}
		for (int j = a.getLength() - 1; j >= 0; j--)
		{
			int neww = cur + to(a.get(j)) * to(b.get(i));
			cur = neww / 10;
			curString.append(from(neww % 10));
		}
		curString.append(from(cur));
		curString.reverse();
		ans += LN(curString);
	}
	if ((negative && obj.negative) || (!negative && !obj.negative))
	{
		return ans;
	}
	else
	{
		return ans.negate();
	}
}
LN LN::divide(const LN& obj) const
{
	if (nan || obj.nan || obj == LN("0"))
	{
		return LN("NaN", 0, 1);
	}
	LN first = this->abs(), second = obj.abs(), ans(0LL);
	if (first < second)
	{
		return LN(0LL);
	}
	if (first == second)
	{
		if ((!negative && !obj.negative) || (negative && obj.negative))
		{
			return LN(1LL);
		}
		else
		{
			return LN(-1LL);
		}
	}
	MyString a = first.number, b = second.number;
	while (first >= second)
	{
		bool good = false;
		for (int i = 0; i < a.getLength(); i++)
		{
			MyString curString("");
			for (int j = 0; j <= i; j++)
			{
				curString.append(a.get(j));
			}
			for (int k = 9; k > 0; k--)
			{
				if (LN(curString) >= LN(second * LN(k)))
				{
					curString = LN(second * LN(k)).number;
					MyString toAns("");
					for (int j = i + 1; j < a.getLength(); j++)
					{
						curString.append("0");
						toAns.append("0");
					}
					toAns.append(from(k));
					toAns.reverse();
					ans += LN(toAns);
					first -= LN(curString);
					a = first.number;
					good = true;
				}
				if (good)
				{
					break;
				}
			}
			if (good)
			{
				break;
			}
		}
	}
	if ((negative && !obj.negative) || (!negative && obj.negative))
	{
		return ans.negate();
	}
	return ans;
}
LN LN::mod(const LN& obj) const
{
	if (nan || obj.nan)
	{
		return LN("NaN", 0, 1);
	}
	LN temp = this->divide(obj);
	temp *= obj;
	return this->subtract(temp);
}
LN LN:: operator+(const LN& obj) const {
	return this->add(obj);
}
LN LN:: operator-(const LN& obj) const {
	return this->subtract(obj);
}
LN LN:: operator*(const LN& obj) const {
	return this->multiply(obj);
}
LN LN:: operator/(const LN& obj) const {
	return this->divide(obj);
}
LN LN:: operator%(const LN& obj) const {
	return this->mod(obj);
}
LN LN:: operator-() const {
	return this->negate();
}
LN LN:: operator~() const {
	return this->sqrt();
}
LN LN::operator+=(const LN& obj)
{
	*this = this->add(obj);
	return *this;
}
LN LN::operator-=(const LN& obj)
{
	*this = this->subtract(obj);
	return *this;
}
LN LN::operator*=(const LN& obj)
{
	*this = this->multiply(obj);
	return *this;
}
LN LN::operator/=(const LN& obj)
{
	*this = this->divide(obj);
	return *this;
}
LN LN::operator%=(const LN& obj)
{
	*this = this->mod(obj);
	return *this;
}
LN::operator long long() const
{
	if (this->nan)
	{
		throw std::runtime_error("Value is not a number");
	}
	if (*this > LN("9223372036854775807") || *this < LN("-9223372036854775808"))
	{
		throw std::runtime_error("Number can't be casted into long long");
	}
	long long res;
	if (*this == LN("-9223372036854775808")) {
		res = LLONG_MIN;
	}
	else {
		res = std::stoll(this->number.getStr());
		if (negative)
		{
			res = -res;
		}
	}
	return res;
}
LN operator""_ln(const char* str)
{
	return LN(str);
}
void LN::toString(std::ofstream& out) const
{
	out << (negative ? "-" : "") << number.getStr() << "\n";
}
