#include "RF24FakePool.h"

#include "RF24Fake.h"

#ifdef _WIN32

void	RF24PoolInterface::SetBufferData(const void* buf, uint8_t len)
{
	free(_bufferData);
	_bufferData = malloc(len);
	memcpy(_bufferData, buf, (size_t)len);
	_bufferLength = len;
	OnReceiveData();
}

bool	RF24Pool::Write(const void* buf, uint8_t len)
{
	for (size_t i = 0; i < 100; i += 1)
	{
		if (_mutex.try_lock())
		{
			// Here is implemented False FAKE
			for (uint16_t i = 0; i < _units.size(); i += 1)
			{
				_units.at(i)->SetBufferData(buf, len);
			}
			_mutex.unlock();
			return (true);
		}
	}
	Serial << "RF24POOL Failed !";
	return (false);
}

#endif // _WIN32