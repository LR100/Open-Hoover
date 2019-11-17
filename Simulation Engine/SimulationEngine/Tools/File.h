#pragma once


#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
#include <string>

/// FOR CFILE
#include <cstdio>
#include <stdarg.h>

#define DEBUGFILE 0

class File
{
public:
	File() {};
	File(const std::string& filePath);
	virtual const std::string	GetPath() const;
	virtual const std::string	GetName() const;
	virtual const std::string	GetExtension() const;
	static	bool				Exist(const std::string& filePath);
	bool						Exist();



	static std::string ExtractExtension(const std::string& filePath);
	static std::string ExtractName(const std::string& filePath);

protected:
	void SetNameAndExtension();

	std::string _filePath;
	std::string _fileName;
	std::string _fileExtension;
	bool		_exist;
};

class FluxFile : public File
{
public:
	FluxFile(const std::string& filePath, bool overwrite = false);
	FluxFile(const FluxFile&);
	FluxFile& operator=(const FluxFile&);

	bool			Write(const std::string& msg);
	std::string		Read();
	bool			ReadByLine(std::string& line);

private:
	std::fstream	_outStream;
	std::ifstream	_inStream;
};

class CFile : public File
{
public:
	CFile(const std::string& filePath, const char* modes);
	~CFile() {};
	void			Write(const std::string& msg);
	char*			CRead();
	void			CWrite(const char* msg);
	void			CWrite(const char* format, ...);
	void			Close();
	int				GetSize() { return (_size); };

private:
	void			InWrite(const char* msg);

	FILE*			_file;
	int				_size;
	const char*		_modes;
};

