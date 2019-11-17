#include "CStr.h"

char* CStr::SetLeftMem(char* dest, char* temp, int pos, int stop)
{
	free(temp);
	while (pos < stop)
	{
		dest[pos] = '\0';
		pos = pos + 1;
	}
	return (dest);
}

char* CStr::MyRealloCpy(char* dest, int new_size)
{
	char*	temp = NULL;
	int		pos_cpy;
	int		size_dest;

	pos_cpy = 0;
	size_dest = 0;
	while (dest[size_dest++] != '\0');
	if ((temp = StrSafeAlloc(temp, (size_dest + 1))) == NULL)
		return (NULL);
	while (pos_cpy <= size_dest)
	{
		temp[pos_cpy] = dest[pos_cpy];
		pos_cpy = pos_cpy + 1;
	}
	free(dest);
	if ((dest = StrSafeAlloc(dest, (size_dest + new_size + 1))) == NULL)
		return (NULL);
	pos_cpy = 0;
	while (pos_cpy <= size_dest)
	{
		dest[pos_cpy] = temp[pos_cpy];
		pos_cpy = pos_cpy + 1;
	}
	dest = SetLeftMem(dest, temp, pos_cpy, (size_dest + new_size + 1));
	return (dest);
}

char* CStr::StrCatRealloc(char* dest, const char* src, int size_src)
{
	int   i;
	int   first;

	i = 0;
	first = 0;
	if (dest != NULL)
	{
		while (dest[first] != '\0')
			first = first + 1;
		if ((dest = MyRealloCpy(dest, size_src)) == NULL)
			return (NULL);
	}
	else
		if ((dest = StrSafeAlloc(dest, (size_src + 1))) == NULL)
			return (NULL);
	while (i < size_src)
	{
		dest[first + i] = src[i];
		i = i + 1;
	}
	dest[first + i] = '\0';
	return (dest);
}

char* CStr::StrSafeAlloc(char* str, int size)
{
	if (size <= 0)
		return (NULL);
	if ((str = (char*)malloc(sizeof(char) * (size + 1))) == NULL)
		return (NULL);
	str = MyMemset(str, 0, size);
	return (str);
}

char* CStr::MyMemset(char* str, char d, int size)
{
	int	pos;

	pos = 0;
	while (pos < size)
	{
		str[pos] = d;
		pos = pos + 1;
	}
	str[pos] = '\0';
	return (str);
}

int	CStr::StrHadDelim(char* str, char*delim)
{
	int	p;
	int	n;

	p = 0;
	n = 0;
	if (str == NULL)
		return (-1);
	while (str[p] != '\0')
	{
		if ((IsDelim(str[p], delim)) == 1)
			n += 1;
		p += 1;
	}
	return (n);
}

int	CStr::StrHadOnlyDelim(char*str, char*delim, int newLine)
{
	int	p;

	p = 0;
	if (str == NULL)
		return (-1);
	while (str[p] != '\0')
	{
		if ((IsDelim(str[p], delim)) == 0 && str[p] != '\0')
		{
			if (newLine == 1 && str[p] != '\n')
				return (-1);
		}
		p += 1;
	}
	return (1);
}

char* CStr::GetWordDelim(char*word, char*str, char*delim, int n)
{
	static int	p;
	int		lp;

	if (n == 1)
		p = 0;
	word = NULL;
	if (str == NULL || delim == NULL)
		return (NULL);
	while (str[p] != '\0')
	{
		while ((IsDelim(str[p], delim)) == 1 && str[p] != '\0')
			p += 1;
		lp = p;
		while ((IsDelim(str[p], delim)) == 0 && str[p] != '\0')
			p += 1;
		if (lp != p)
		{
			if ((word = StrCatRealloc(word, &str[lp], (p - lp))) == NULL)
				return (NULL);
			return (word);
		}
	}
	p = 0;
	return (NULL);
}

int	CStr::IsDelim(char d, char*delim)
{
	int	i;

	i = 0;
	while (delim[i] != '\0')
	{
		if (d == delim[i])
			return (1);
		i = i + 1;
	}
	return (0);
}

int	CStr::NbWordDelim(char*str, char*delim)
{
	int	pos;
	int	lpos;
	int	n;

	n = 0;
	pos = 0;
	while (str[pos] != '\0')
	{
		while ((IsDelim(str[pos], delim)) == 1 && str[pos] != '\0')
			pos += 1;
		lpos = pos;
		while ((IsDelim(str[pos], delim)) == 0 && str[pos] != '\0')
			pos += 1;
		if (lpos != pos)
			n = n + 1;
	}
	return (n);
}

char* CStr::NewWord(char*word, char*str, int size, int *i)
{
	word = NULL;
	if ((word = StrCatRealloc(word, str, size)) == NULL)
		return (NULL);
	*i = *i + 1;
	return (word);
}

char** CStr::StrToWordTabDelim(const char*strC, const char*delimC)
{
	int	p;
	int	lp;
	int	i;
	char* str = NULL;
	str = StrCpy(str, strC);
	char* delim = NULL;
	delim = StrCpy(delim, delimC);
	char	**tab;
	i = 0;
	p = 0;
	if (str == NULL || delim == NULL)
		return (NULL);
	if ((tab = (char**)malloc(sizeof(char*) * (NbWordDelim(str, delim) + 1))) == NULL
		|| (NbWordDelim(str, delim) < 1))
		return (NULL);
	while (str[p] != '\0')
	{
		while ((IsDelim(str[p], delim)) == 1 && str[p] != '\0')
			p += 1;
		lp = p;
		while ((IsDelim(str[p], delim)) == 0 && str[p] != '\0')
			p += 1;
		if (lp != p)
			if ((tab[i] = NewWord(tab[i], &str[lp], (p - lp), &i)) == NULL)
				return (NULL);
	}
	tab[i] = '\0';
	return (tab);
}

const char* CStr::GetNWordFromWordTab(char** wordtab, int n)
{
	if (wordtab == NULL)
	{
		return (NULL);
	}
	char* result = NULL;
	if (n == CSTRFIRST)
	{
		return (wordtab[0]);
	}
	int it = 0;
	while (wordtab[it] != '\0')
	{
		if (n == it)
		{
			return (wordtab[it]);
		}
		it += 1;
	}
	if (n == CSTRLAST)
	{
		return (wordtab[it - 1]);
	}
	return (result);
}

void CStr::DeleteWordtab(char**wordtab)
{
	if (wordtab == NULL)
	{
		return;
	}
	int i = 1;
	while (wordtab[i] != '\0')
	{
		free(wordtab[i - 1]);
		i += 1;
	}
	free(wordtab);
}

int CStr::StrLen(const char* src)
{
	if (src == NULL)
	{
		return (0);
	}
	int it = 0;
	while (src[it] != '\0')
	{
		it += 1;
	}
	return (it);
}

char* CStr::StrCpy(char* dest, const char* src)
{
	if (src == NULL) { return (NULL); }
	dest = (char*)malloc(sizeof(char) * (StrLen(src) + 1));
	if (dest == NULL) { return (NULL); }
	int it = 0;
	while (src[it] != '\0')
	{
		dest[it] = src[it];
		it += 1;
	}
	dest[it] = '\0';
	return (dest);
}
