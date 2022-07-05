#include "LN.h"
#include "return_codes.h"

#include <fstream>
#include <iostream>
#include <vector>
bool check(const std::string s)
{
	int pos = 0;
	if (s[pos] == '-')
	{
		pos++;
	}
	while (pos < s.length() && std::isdigit(s[pos]))
	{
		pos++;
	}
	return pos == s.length() && s != "-";
}
int main(int argc, char** argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "Wrong number of arguments");
		return ERROR_INVALID_PARAMETER;
	}
	std::ifstream in(argv[1]);
	if (!in.is_open())
	{
		fprintf(stderr, "Couldn't open input file");
		return ERROR_FILE_NOT_FOUND;
	}
	std::vector< LN > stack;
	std::string s;
	bool badAlloc = false;
	try
	{
		while (in >> s)
		{
			if (check(s))
			{
				LN a(s.c_str());
				if (a.checkBadAllocation()) {
					badAlloc = true;
					break;
				}
				stack.push_back(a);
			}
			else if (s == "_")
			{
				LN a = (LN&&)(-stack.back());
				if (a.checkBadAllocation()) {
					badAlloc = true;
					break;
				}
				stack.pop_back();
				stack.push_back(a);
			}
			else if (s == "~")
			{
				LN a = (LN&&)(~stack.back());
				if (a.checkBadAllocation()) {
					badAlloc = true;
					break;
				}
				stack.pop_back();
				stack.push_back(a);
			}
			else
			{
				LN b = (LN&&)stack.back();
				stack.pop_back();
				LN a = (LN&&)stack.back();
				stack.pop_back();
				if (s == "+")
				{
					LN x = (LN&&)(a + b);
					if (x.checkBadAllocation()) {
						badAlloc = true;
						break;
					}
					stack.push_back(x);
				}
				else if (s == "-")
				{
					LN x = (LN&&)(a - b);
					if (x.checkBadAllocation()) {
						badAlloc = true;
						break;
					}
					stack.push_back(x);
				}
				else if (s == "*")
				{
					LN x = (LN&&)(a * b);
					if (x.checkBadAllocation()) {
						badAlloc = true;
						break;
					}
					stack.push_back(x);
				}
				else if (s == "/")
				{
					LN x = (LN&&)(a / b);
					if (x.checkBadAllocation()) {
						badAlloc = true;
						break;
					}
					stack.push_back(x);
				}
				else if (s == "%")
				{
					LN x = (LN&&)(a % b);
					if (x.checkBadAllocation()) {
						badAlloc = true;
						break;
					}
					stack.push_back(x);
				}
				else
				{
					bool res;
					if (s == ">")
					{
						res = a > b;
					}
					else if (s == "<")
					{
						res = a < b;
					}
					else if (s == ">=")
					{
						res = a >= b;
					}
					else if (s == "<=")
					{
						res = a <= b;
					}
					else if (s == "==")
					{
						res = a == b;
					}
					else if (s == "!=")
					{
						res = a != b;
					}
					LN x = (LN&&)(res ? LN("1") : LN("0"));
					if (x.checkBadAllocation()) {
						badAlloc = true;
						break;
					}
					stack.push_back(x);
				}
			}
		}
	}
	catch (std::bad_alloc& e)
	{
		in.close();
		fprintf(stderr, "Couldn't allocate memory");
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	in.close();
	if (badAlloc) {
		fprintf(stderr, "Couldn't allocate memory");
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	std::ofstream out(argv[2]);
	if (!out.is_open())
	{
		fprintf(stderr, "Couldn't open/create output file");
		return ERROR_FILE_NOT_FOUND;
	}
	for (int i = stack.size() - 1; i >= 0; i--)
	{
		stack[i].toString(out);
	}
	out.close();
	return ERROR_SUCCESS;
}