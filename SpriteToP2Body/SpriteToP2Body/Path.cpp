#include "Path.h"

Path::Path()
{
}

Path::~Path()
{
}

#include <Windows.h>

std::string Path::GetExePath()
{
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return (std::string(buffer).substr(0, pos));
}
