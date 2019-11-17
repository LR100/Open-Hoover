#ifndef _STRTOOLS_H_
#define _STRTOOLS_H_

#include <vector>
#include <string>
#include "MySorting.h"

class StrTools
{
public:

	static void CleanFrontBackStr(std::string& str, const std::string& tokens);
	static void CleanFrontStr(std::string& str, const std::string& tokens);
	static void CleanBackStr(std::string& str, const std::string& tokens);
	static bool IsDelim(const char& c, const std::string& delim);
	static char Lower(const char& c);
	static bool	StrContains(const std::string& str, const std::string& tokens);
	static bool	StrContainsStr(const std::string& str, const std::string& strB);
	static float	StrToFloat(const std::string &str);
	static double	StrToDouble(const std::string &str);
	static int		StrToInt(const std::string &str);
	static size_t	StrToUInt(const std::string &str);
	static int		StrCmp(const std::string& a, const std::string& b);
	static int		StrCmpNoCase(const std::string& a, const std::string& b);
	static void		SortStrList(std::vector<std::string>& list, bool(*cmp)(const std::string&, const std::string&));
	static void		SortAlhabeticalStrList(std::vector<std::string>& list);

private:

};

#endif /* !_STRTOOLS_H_ */
