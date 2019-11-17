#ifndef __RF24SCServer_H__
#define __RF24SCServer_H__

#include "RF24SC.h"

#define RF24SERVER_CLIENT_MAX 4
#define RF24SERVER_CLIENT_UNKNOWN 5

//const uint64_t pipesClients[4] = { 0xE8E8F0F0A3LL, 0xE8E8F0F0A4LL, 0xE8E8F0F0A5LL, 0xE8E8F0F0A6LL };

////////////////////////
////////////////////////
///// RF24_SERVER //////
////////////////////////
////////////////////////

class RF24Server : public RF24Communicator
{
public:
	RF24Server(uint8_t pinCE, uint8_t pinCS) : RF24Communicator(pinCE, pinCS)//, _network(_radio)
	{
		_deviceID = RF24SERVER_ID;
		_timeState = 0;
	}

	virtual ~RF24Server()
	{
	}

private:

	virtual void UpdateState(const unsigned long& dtMs) override
	{
		_timeState += dtMs;
		//Serial << "Time State: (" << _timeState << ")\n";
		if (_state == RF24State::SEND_CALL)
		{
			SendCall();
			if (SendAuthWrite())
			{
				_timeState = 0;
				_state = RF24State::READ_CALL;
				ReadCall();
			}
		}
		if (_state == RF24State::READ_CALL)
		{
			if (ReadCallReceptive() || _timeState > 50)
			{
				_state = RF24State::SEND_CALL;
			}
		}
	}


	

	virtual bool InitExtended(const uint64_t& pipeServerWrite, const uint64_t& pipeServerRead) override
	{
		ClientsInit();
#ifndef _WIN32
		_radio.openReadingPipe(1, pipeServerRead);
		_radio.openWritingPipe(pipeServerWrite);
		//_radio.writeAckPayload(pipeServerRead, &_ackValueCertification, sizeof(int));
		Listen(true);
#endif // !_WIN32
		return (true);
}

	struct Client
	{
		bool		assigned;
		uint16_t	timeSinceLastCom;
		uint64_t	pipe;
	};

	void	ClientsInit()
	{
		_clientsCount = 0;
		for (size_t i = 0; i < RF24SERVER_CLIENT_MAX; i += 1)
		{
			_clients[i].assigned = false;
			_clients[i].timeSinceLastCom = 0;
		}
		_clients[0].pipe = 0xE8E8F0F0A3LL;
		_clients[1].pipe = 0xE8E8F0F0A4LL;
		_clients[2].pipe = 0xE8E8F0F0A5LL;
		_clients[3].pipe = 0xE8E8F0F0A6LL;
	}

	uint8_t	ClientGetAvailable()
	{
		for (size_t i = 0; i < RF24SERVER_CLIENT_MAX; i += 1)
		{
			if (!_clients[i].assigned)
			{
				_clients[i].assigned = true;
				_clients[i].timeSinceLastCom = 0;
				_clientsCount += 1;
				return (i);
			}
		}
		return (RF24SERVER_CLIENT_MAX);
	}

	void	ClientPing()
	{
		for (size_t i = 0; i < RF24SERVER_CLIENT_MAX; i += 1)
		{
			if (_clients[i].assigned)
			{
				_clients[i].assigned = false;
				_clients[i].timeSinceLastCom = 0;
			}
		}
	}

	//virtual bool IsAuthorized(RF24Packet* packetRequest) const
	//{
	//	for (size_t i = 0; i < RF24SERVER_CLIENT_MAX; i += 1)
	//	{
	//		if (i == packetRequest->header.senderId)
	//		{
	//			return (true);
	//		}
	//	}
	//	return (false);
	//}

	uint16_t			_timeState;
	uint8_t				_clientsCount;
	Client				_clients[RF24SERVER_CLIENT_MAX]; // CLIENT TIME SINCE LAST CO
};

#endif // __RF24SCServer_H__