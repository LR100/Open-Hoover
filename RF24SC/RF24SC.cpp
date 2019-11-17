#include "RF24SC.h"


///////////////////////
/// DEBUG FUNCTIONS
//

#if DEBUGRF24_STATE
String RF24Communicator::RF24StateToString(RF24Communicator::RF24State state)
{
	if (state == RF24Communicator::RF24State::NONE)
		return ("NONE");
	else if (state == RF24Communicator::RF24State::READ_CALL)
		return ("READ_CALL");
	else if (state == RF24Communicator::RF24State::SEND_AUTH_WRITE)
		return ("SEND_AUTH_WRITE");
	else if (state == RF24Communicator::RF24State::SEND_CALL)
		return ("SEND_CALL");
}
#endif

void	DumpObject(char* object, uint8_t sizeObject)
{
	for (uint8_t i = 0; i < sizeObject; i += 1)
	{
#ifdef _WIN32
		std::cout << std::bitset<8>(object[i]) << std::endl;
#else
		Serial.println(object[i], BIN);
#endif // _WIN32
	}
}

RF24Communicator::RF24Communicator(uint8_t pinCE, uint8_t pinCS) : _radio(pinCE, pinCS), _ackValueCertification(RF24_ACK_VALUE)
{
	Serial << "(" << _deviceID << ")" << " RF24Communicator Constructor\n";

	Serial << " Size of RF24Packet (" << sizeof(RF24Packet) << ")\n";
	if (!_radio.begin())
	{
		Serial << "(" << _deviceID << ")" << " RF24Communicator Constructor ERROR: Radio failed to begin\n";
	}

	_deviceID = 0;

	_memoryBufferUsed = sizeof(*this) + RF24_DATA_FAT_MAX_SIZE; // Size of all essential buffers 
	_memoryBufferMax = 1024; // 1024 octets
	_lastChecksum = 0; // NEED To be set on ZERO for init check
	_time = 0;
	_timeDtMs = 0;
	_deviceIDReceiver = RF24ALL_ID;
	_requestFcts = NULL;
	_state = RF24State::SEND_CALL; // Server Prior Mode is Send Call

	//memset((void*)& _packetReceivedBufffer, 0, RF24_PACKET_SIZE_MAX);
	//memset((void*)& _packetSendBufffer, 0, RF24_PACKET_SIZE_MAX);
	//memset((void*)& _packetSendCallBufffer, 0, RF24_PACKET_SIZE_MAX);
	Serial << "(" << _deviceID << ")" << " RF24Communicator Constructor OK\n";
};

RF24Communicator::~RF24Communicator()
{
	if (_lastChecksum != 0) // Had been initialized
	{
		delete (_requestFcts);
		_dataFatBuffer.Release();
	}
}

bool	RF24Communicator::InitCommunication()
{
	Serial << "(" << _deviceID << ")" << "Init Communication\n";
	if (_lastChecksum == 0)
	{
		const uint64_t pipeServerWrite = 0xE8E8F0F0A1LL;
		const uint64_t pipeServerRead = 0xE8E8F0F0A2LL;

		if (!InitBasics())
		{
			return (false);
		}
		if (!InitExtended(pipeServerWrite, pipeServerRead))
		{
			return (false);
		}
		_lastChecksum = 14; // Last Check Sum is used as marker of Initialization
		// It's set only from Valid Packet and value could never be 0 or 1
#if DEBUGRF24_MEMORY
		DisplayMemoryUsed();
#endif
		return (true);
	}
	else
	{
		Serial << "(" << _deviceID << ")" << "Already Init\n";
	}
}

void	RF24Communicator::UpdateCommunication()
{

	if (_lastChecksum == 0)
	{
		Serial << "(" << _deviceID << ")" << "Bad Last Check Sum !! \n";
		return;
	}

	UpdateTime();
	debugLifeTimeCounter += _timeDtMs;
	if (debugLifeTimeCounter >= 1000)
	{
		Serial << "(" << _deviceID << ")" << "RF24Communicator is living\n";
		debugLifeTimeCounter = 0;
	}

	//Serial << "(" << _deviceID << ")" <<  "dtMs (" << dtMs << ")\n";
	UpdateState(_timeDtMs);
#if DEBUGRF24_STATE
	Serial << "(" << _deviceID << ")" << "State (" << RF24StateToString(_state) << ")\n";
#endif
	// Check if current call is timeout	
	UpdateTimeCall();
}
