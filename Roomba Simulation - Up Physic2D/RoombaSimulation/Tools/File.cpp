#include "File.h"

const std::string File::GetPath() const
{
	return (_filePath);
}

const std::string File::GetName() const
{
	return (_fileName);
}

const std::string File::GetExtension() const
{
	return (_fileExtension);
}

bool File::Exist(const std::string & filePath)
{
	std::ifstream f(filePath.c_str());
	return (f.good());
}

bool File::Exist()
{
	return (_exist);
}

std::string File::ExtractExtension(const std::string & filePath)
{
	size_t lp = 0;

	lp = filePath.find_last_of(".");
	if (lp != std::string::npos)
	{
		return (filePath.substr(lp));
	}
	return ("");
}

std::string File::ExtractName(const std::string & filePath)
{
	size_t lp = 0;

	lp = filePath.find_last_of(".");
	
	size_t lbs = 0;
	size_t ls = 0;

	ls = filePath.find_last_of("/");
	lbs = filePath.find_last_of("\\");
	if (ls != std::string::npos)
	{
		return (filePath.substr(ls + 1, (lp - (ls + 1))));
	}
	else if(lbs != std::string::npos)
	{
		return (filePath.substr(lbs + 1, (lp - (lbs + 1))));
	}
	else
	{
		return (filePath.substr(0, lp));
	}
}

void File::SetNameAndExtension()
{
	size_t lp = 0;

	lp = _filePath.find_last_of(".");
	if (lp != std::string::npos) 
	{
		_fileExtension = _filePath.substr(lp);		
	}
	size_t le = 0;

	le = _filePath.find_last_of("/");
	if (le != std::string::npos)
	{
		_fileName = _filePath.substr(le, (lp - le));
	}	
}

File::File(const std::string& filePath)
{
	_filePath = filePath;
	SetNameAndExtension();
}

FluxFile::FluxFile(const std::string &filePath, bool overwrite) : File(filePath)
{
	if (overwrite)
	{
		std::ofstream ofStream(_filePath.c_str());
		ofStream.close();
	}
}

bool FluxFile::Write(const std::string& msg)
{
	try 
	{
		std::ofstream ofStream(_filePath.c_str(), std::ofstream::app);
		ofStream << msg;
		ofStream.close();
	}
	catch (...)
	{
		std::cerr << "Cannot Write in file " << _filePath << std::endl;
		return (false);
	}
	return (true);
}


std::string FluxFile::Read()
{
	std::ifstream file(_filePath.c_str());
	std::stringstream buffer;

	buffer << file.rdbuf();
	std::string(dataStr) = buffer.str();
	file.close();
	return (dataStr);
}

CFile::CFile(const std::string &filePath, const char * modes) : _modes(modes)
{
	_file = fopen(filePath.c_str(), modes);
	/// Put _file at End Of File
	fseek(_file, 0, SEEK_END);
	/// Size from Beg To End
	_size = ftell(_file);
	/// Put _file at Begin Of File
	rewind(_file);
	if (_size < 0)
	{
		_size = 0;
	}
	_filePath = filePath;
	SetNameAndExtension();
	if (DEBUGFILE)
	{
		std::cout << "Load CFile Named:" << GetName() << " At Path:" << GetPath() << " Of Size:" << GetSize();
	}
}

char * CFile::CRead()
{
	int bufferSize = _size + 1;
	char * buffer = (char*)malloc(sizeof(char) * bufferSize);
	if (buffer == NULL)
	{
		return (NULL);
	}
	fread(buffer, 1, _size, _file);
	buffer[bufferSize] = '\0';
	return ((buffer));
}

void CFile::Write(const std::string &msg)
{
	InWrite(const_cast<char *>(msg.c_str()));
}

#define SIZE_CFILEBUFFER 512

void CFile::CWrite(const char * msg)
{
	InWrite(msg);
}

void CFile::CWrite(const char * format, ...)
{
	char buffer[SIZE_CFILEBUFFER];

	va_list params;
	va_start(params, format);
	vsprintf(buffer, format, params);
	va_end(params);
	InWrite(const_cast<char *>(buffer));
}

void CFile::InWrite(const char *msg)
{
	if (_file == NULL)
	{
		_file = fopen(_filePath.c_str(), _modes);
	}
	if (_file != NULL)
	{
		fputs(msg, _file);
	}
}

void CFile::Close()
{
	if (_file != NULL)
	{
		fclose(_file);
	}
}

