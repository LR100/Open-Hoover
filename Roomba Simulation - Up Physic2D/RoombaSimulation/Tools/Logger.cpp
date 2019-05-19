#include "Logger.h"

#define SIZE_LOGBUFFER 8192


void GLogger::AddLogger(ALogger * Logger, std::string nameLogger)
{
	this->_loggers[nameLogger] = Logger;
	if (!Logger->IsDebug())
	{
		std::cout << "NOT A DEBUG LOGGER " << nameLogger << std::endl;
		_loggers[nameLogger]->CLog("----------------\n"\
			"--------------------------------\n"\
			"START (%s) Logger \n"\
			"--------------------------------\n"\
			,nameLogger.c_str());
	}
}

ALogger * GLogger::GLog(const std::string& nameLogger)
{
	if (_loggers.count(nameLogger))
		return (_loggers.at(nameLogger));
	else if (nameLogger != "")
	{
		ALogger *nLogger = new FileLogger(nameLogger);
		
		nLogger->CLog("----------------\n"\
			"--------------------------------\n"\
			"START (%s) Logger \n"\
			"--------------------------------\n"\
			, nameLogger.c_str());
		_loggers[nameLogger] = nLogger;
		return (nLogger);
	}
	return (_safeLogger);
}

void GLogger::Flush()
{
	std::unordered_map<std::string, ALogger*>::iterator it = _loggers.begin();
	while (it != _loggers.end())
	{
		it->second->Flush();
		it++;
	}
	_safeLogger->Flush();
}

GLogger & GLogger::Get()
{
	static GLogger	gLogger;
	return gLogger;
}

ALogger::ALogger(bool debug)
{
	_debug = debug;
	_sizeMaxBuffer = 1024; // SIZE Logger Buffer
}

void ALogger::CLog(const char * format, ...)
{
	if (!ACTIVE_LOGGER) { return; }
	char buffer[SIZE_LOGBUFFER];

	va_list params;
	va_start(params, format);
	vsprintf(buffer, format, params);
	va_end(params);
	std::string convertBuffer(buffer);
	if (_debug)
	{
		DebugWrite(convertBuffer);
	}
	else
	{
		Write(convertBuffer);
	}
}

void ALogger::Log(const std::string& msg, bool force)
{
	if (!ACTIVE_LOGGER) { return; }
	// DEBUG < ACTIVE_LOGGER
	if (_debug)
	{
		DebugWrite(msg);
	}
	else
	{
		Write(msg);
		if (force) // Write now
		{
			Flush();
		}
	}
}

void ALogger::Flush()
{
	if (WriteBuffer())
	{
		_buffer.clear();
	}
}

void ALogger::WriteInBuffer(std::string msg)
{
	_buffer += msg;
	CheckWriteBuffer();
}

void ALogger::CheckWriteBuffer()
{
	if (_buffer.size() >= _sizeMaxBuffer)
	{
		if (WriteBuffer())
		{
			_buffer.clear();
		}
	}
}

bool ALogger::WriteBuffer()
{
	for (size_t inc = 0; inc < _buffer.size(); inc++)
	{
		std::cout << _buffer.at(inc) << std::endl;
	}
	return (true);
}

void ALogger::Write(const std::string& msg)
{
	WriteInBuffer(std::string(msg));
}

void ALogger::DebugWrite(const std::string& msg)
{
	std::cout << msg << std::endl;
}


FileLogger::FileLogger(const std::string& filePath, bool overwrite, bool debug, bool withDate, bool logWithTime) : ALogger(debug)
{
	_logWithTime = logWithTime;
	std::string tmpPath;
	withDate = false;
	tmpPath = ((withDate) ? (filePath + "-" + TimeDateFormatter::GetFormatCurrentTime("%d-%m-%Y")) : (filePath));
	tmpPath += ".log";
	_file = new FluxFile(LOGPATH + tmpPath, overwrite);
}

void FileLogger::Write(const std::string& msg)
{
	_logWithTime = false;
	if (_logWithTime)
	{
		WriteInBuffer(TimeDateFormatter::GetFormatCurrentTime("%Hh%Mm%Ss:\t") + msg + "\n");
	}
	else
	{
		WriteInBuffer(msg + "\n");
	}
}

bool FileLogger::WriteBuffer()
{
	if (_file->Write(_buffer))
	{
		_buffer.clear();
		return (true);
	}
	return (false);
}
//
//ALogger * operator<<(ALogger * a, const std::string & s)
//{
//	a->Log(s);
//	return (a);
//}

//ALogger * operator<<(ALogger * a, const int & s)
//{
//	std::stringstream ss;
//	ss << s;
//	a->Log(ss.str());
//	return (a);
//}
