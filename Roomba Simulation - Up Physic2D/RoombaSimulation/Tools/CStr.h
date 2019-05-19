#pragma once

#include <cstring>
#include <stdlib.h>

enum ECStr : int
{
	CSTRLAST = -42,
	CSTRFIRST = -43
};

class CStr
{
public:
	virtual ~CStr() {};

	static char**		StrToWordTabDelim(const char* str, const char* delim);
	static void			DeleteWordtab(char** wordtab);
	static const char*  GetNWordFromWordTab(char** wordtab, int n);
	static char*		StrCatRealloc(char*dest, const char*src, int size_src);
	/// UTILS
	static int			StrLen(const char* src);
	static char*		StrCpy(char* dest, const char* src);
private:
	/// STRCATREALLOC
	static char*		SetLeftMem(char* dest, char* temp, int pos, int stop);
	static char*		MyRealloCpy(char* dest, int new_size);
	/// SAFEALLOC
	static char*		StrSafeAlloc(char* str, int size);
	/// MEMSET
	static char*		MyMemset(char* str, char d, int size);
	/// STRTOWORDTABDELIM
	static int			StrHadOnlyDelim(char* str, char* delim, int newLine);
	static int			StrHadDelim(char* str, char* delim);
	static int			IsDelim(char d, char* delim);
	static int			NbWordDelim(char* str, char* delim);
	static char*		GetWordDelim(char* word, char* str, char* delim, int n);
	static char*		NewWord(char* word, char* str, int size, int* i);
};
