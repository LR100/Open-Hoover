#include "StrTools.h"

void StrTools::CleanFrontBackStr(std::string & str, const std::string & tokens)
{
	CleanFrontStr(str, tokens);
	CleanBackStr(str, tokens);
}

void StrTools::CleanFrontStr(std::string & str, const std::string & tokens)
{
	if (!str.size())
		return;
	size_t inc = 0;
	for (; inc < str.size() && IsDelim(str[inc], tokens); inc++);
	if (inc > 0)
		str = str.substr(inc, std::string::npos);
}

void StrTools::CleanBackStr(std::string & str, const std::string & tokens)
{
	if (!str.size())
		return;
	size_t inc = (str.size() - 1);
	for (; IsDelim(str[inc], tokens) && inc > 0 ; inc--);
	if (inc < (str.size() - 1))
		str = str.substr(0, (inc + 1));
}

bool StrTools::IsDelim(const char& c, const std::string & delim)
{
	for (size_t inc = 0; inc < delim.size(); inc++)
		if (c == delim.at(inc))
			return (true);
	return false;
}

char StrTools::Lower(const char & c)
{
	if (c >= 65 && c <= 90)
		return (c + 32);
	return (c);
}

bool StrTools::StrContains(const std::string & str, const std::string & tokens)
{
	for (size_t inc = 0; inc < str.size(); inc++)
		if (IsDelim(str[inc], tokens))
			return (true);
	return (false);
}

bool StrTools::StrContainsStr(const std::string & str, const std::string & strB)
{
	if (!(str.find(strB, std::string::npos) != std::string::npos))
		return (true);
	return (false);
}

float StrTools::StrToFloat(const std::string & str)
{
	try
	{ return (std::stof(str)); }
	catch (std::exception& eer)
	{
		eer.what();
		return (0.0);
	}
}

double StrTools::StrToDouble(const std::string & str)
{
	try
	{ return (std::stod(str)); }
	catch (std::exception& eer)
	{
		eer.what();
		return (0.0);
	}
}

int StrTools::StrToInt(const std::string & str)
{
	try
	{ return (std::stoi(str)); }
	catch (std::exception& eer)
	{
		eer.what();
		return (0);
	}
}

size_t StrTools::StrToUInt(const std::string & str)
{
	try
	{ return ((size_t)(std::stod(str))); }
	catch (std::exception& eer)
	{
		eer.what();
		return (0.0);
	}
}

int StrTools::StrCmp(const std::string & a, const std::string & b)
{
	if (a.size() > b.size())
		return (a.at(a.size() - 1));
	else if (b.size() > a.size())
		return (-b.at(b.size() - 1));
	for (size_t inc = 0; inc < a.size(); inc++)
	{
		if (a.at(inc) != b.at(inc))
			return (a.at(inc) - b.at(inc));
	}
	return (0);
}

int StrTools::StrCmpNoCase(const std::string & a, const std::string & b)
{
	if (a.size() > b.size())
		return (a.at(a.size() - 1));
	else if (b.size() > a.size())
		return (-b.at(b.size() - 1));
	for (size_t inc = 0; inc < a.size(); inc++)
	{
		if (Lower(a.at(inc)) != Lower(b.at(inc)))
			return (a.at(inc) - b.at(inc));
	}
	return (0);
}

void StrTools::SortStrList(std::vector<std::string>& list, bool(*cmp)(const std::string&, const std::string&))
{
	Sorting::QuickSort(list, 0, list.size(), cmp);
}

bool StrCmpAlphabetNoCase(const std::string & a, const std::string & b)
{
	if (a.size() > b.size())
		return (a.at(a.size() - 1));
	else if (b.size() > a.size())
		return (-b.at(b.size() - 1));
	for (size_t inc = 0; inc < a.size(); inc++)
	{
		if (StrTools::Lower(a.at(inc)) != StrTools::Lower(b.at(inc)))
			return (a.at(inc) - b.at(inc) > 0);
	}
	return (false);
}

void StrTools::SortAlhabeticalStrList(std::vector<std::string>& list)
{
	Sorting::QuickSort(list, 0, list.size(), &StrCmpAlphabetNoCase);
}
