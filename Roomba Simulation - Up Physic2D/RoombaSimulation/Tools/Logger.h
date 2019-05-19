#pragma once


#include <stdlib.h>
#include <stdarg.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdexcept>
#include <unordered_map>
#include <vector>


#include "TimeDateFormatter.h"
#include "File.h"

#define ACTIVE_LOGGER 1

/// END Loggers
#define LOGDEBUG "DEBUG"
#define LOGPATH "Log/"
//////////////////////////////
/// GLOGGER INSTANCE AT END

class ALogger
{
public:
	ALogger(bool debug);
	virtual ~ALogger() {};
	virtual void CLog(const char * format, ...);
	virtual void Log(const std::string& msg, bool force = false);   // If Force, write NOW
	virtual bool IsDebug() const { return _debug; };
	virtual void Flush();

protected:

	virtual void DebugWrite(const std::string& msg);
	virtual void Write(const std::string& msg);
	virtual bool WriteBuffer();
	virtual void CheckWriteBuffer();
	virtual void WriteInBuffer(std::string msg);


	//template <typename T>
	//ALogger* operator<<(T const& value);

	bool                        _debug;
	unsigned int                _sizeMaxBuffer;
	std::string					_buffer;
};

//template <typename T>
//ALogger * operator<<(const T& s, ALogger * a);
template <typename T>
ALogger * operator<<(ALogger * a, const T& s);


class FileLogger : public ALogger
{
public:
	FileLogger(const std::string& filePath, bool overwrite = false, bool debug = false, bool withDate = true, bool logWithTime = true);
	virtual ~FileLogger() {};
protected:
	virtual void	Write(const std::string& msg);
	virtual bool	WriteBuffer();

	FluxFile *      _file;
	bool            _logWithTime;
};

class GLogger
{
public:

	void		AddLogger(ALogger * Logger, std::string nameLogger);
	ALogger*	GLog(const std::string& nameLogger);
	void		Flush();

	static GLogger&	Get();
	

private:
	GLogger() 
	{
		_safeLogger = new FileLogger("SafeLog");
	};
	
	ALogger*							_safeLogger;
	std::unordered_map<std::string, ALogger*>		_loggers;
};

template<typename T>
inline ALogger * operator<<(ALogger * a, const T & s)
{
	std::stringstream ss;
	ss << s;
	a->Log(ss.str());
	return (a);
}
