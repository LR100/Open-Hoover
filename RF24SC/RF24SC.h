#ifndef __RF24SC_H__
#define __RF24SC_H__


#ifdef _WIN32

#include <time.h>
#include <string>
#include <iostream>
#include <bitset> // For Binary Display


#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
#define String std::string
#define Serial std::cout 

#else

#include <RF24.h>
#include <nRF24L01.h>
#include <SPI.h>

#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
template<class E> inline Print &operator <<(Print &obj, E arg) { obj.print(arg); return obj; } // For Chainable Print

#endif

#define DEBUGRF24_READ 0
#define DEBUGRF24_READ_REQUEST 0
#define DEBUGRF24_SEND 0
#define DEBUGRF24_SEND_CALL 0
#define DEBUGRF24_MEMORY 0

/*
 Name:		RF24SC.h
 Created:	3/25/2019 1:47:14 PM
 Author:	Loic Robin
*/

#include "Queue.h"

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

////////////////////////
////////////////////////
///// RF24_TYPES //////
////////////////////////
////////////////////////

#define RF24_DATA_FAT_MAX_SIZE 512 // In Bytes
#define RF24_SERIAL_SPEED 115200

typedef enum RF24DataType : uint16_t
{
	// All Basics C/C++ Types
	RF24NODATA = 0,
	RF24INT8 = 1,
	RF24UINT8 = 2,
	RF24INT16 = 3,
	RF24UINT16 = 4,
	RF24INT32 = 5,
	RF24UINT32 = 6,
	RF24INT64 = 7,
	RF24UINT64 = 8,
	RF24INT,
	RF24UINT,
	RF24FLOAT,
	RF24DOUBLE,
	RF24STRING,
	RF24DATASPEC,
	RF24USER_DATA_TYPE // From this value (user)
} RF24DataType;

typedef enum RF24RequestType : uint8_t
{
	RF24NOREQUEST = 0,
	RF24DATA = 1,
	RF24DATA_FAT_BEGIN = 2,
	RF24DATA_FAT_PACKET,
	RF24DATA_FAT_END,
	RF24TEST_PING,
	RF24AUTH_WRITE, // Say to the client that he can now write his buffer
} RF24RequestType;

////////////////////////
////////////////////////
///// RF24_HEADER //////
////////////////////////
////////////////////////

PACK(typedef struct
{
	uint16_t			size = 0;
	RF24DataType		type = RF24DataType::RF24NODATA;

	void	SetFromData(uint8_t _data) { Set(sizeof(uint8_t), RF24DataType::RF24UINT8); }
	void	SetFromData(uint16_t _data) { Set(sizeof(uint16_t), RF24DataType::RF24UINT16); }
	void	SetFromData(uint32_t _data) { Set(sizeof(uint32_t), RF24DataType::RF24UINT32); }
	void	SetFromData(uint64_t _data) { Set(sizeof(uint64_t), RF24DataType::RF24UINT64); }
	void	SetFromData(int8_t _data) { Set(sizeof(int8_t), RF24DataType::RF24INT8); }
	void	SetFromData(int16_t _data) { Set(sizeof(int16_t), RF24DataType::RF24INT16); }
	void	SetFromData(int32_t _data) { Set(sizeof(int32_t), RF24DataType::RF24INT32); }
	void	SetFromData(int64_t _data) { Set(sizeof(int64_t), RF24DataType::RF24INT64); }

	//void	SetByData(int _data) { Set(sizeof(int), RF24DataType::INT); }
	//void	SetByData(unsigned int _data) { Set(sizeof(unsigned int), RF24DataType::UINT); }
	void	SetFromData(const char* _data, uint8_t size) { Set(sizeof(char) * size, RF24DataType::RF24STRING); }

	void	Set(const uint16_t& _size, RF24DataType& _type)
	{
		size = _size;
		type = _type;
	}
	void	Set(const uint16_t& _size, const uint8_t& _type)
	{
		size = _size;
		type = (RF24DataType)_type;
	}

}) RF24DataSpecification;

#define RF24_HEADER_VALID 0b01010111

//template<class E> inline Print &operator <<(Print &obj, E arg) { obj.print(arg); return obj; } // For Chainable Print

PACK(typedef struct
{
	/*RF24Header()
	{
		requestType = RF24RequestType::RF24NOREQUEST;
		dataSpec.type = RF24DataType::RF24NODATA;
		dataSpec.size = 0;
		magic = 0;
	}*/

	void	Display()
	{
		Serial << "Request Type (" << requestType << ") ";
		Serial << "Data Type (" << dataSpec.type << ") ";
		Serial << "Data Size (" << dataSpec.size << ") ";
		Serial << "Magic (" << magic << ") ";
		Serial << "Sender Device ID (" << senderDeviceID << ") ";
		Serial << "Reciever Device ID (" << receiverDeviceID << ")";
	}

	RF24DataSpecification	dataSpec; // 16 bits

	RF24RequestType			requestType = RF24RequestType::RF24NOREQUEST; // 32 bits
	uint8_t					magic = 0;
	uint8_t					senderDeviceID = 0;
	uint8_t					receiverDeviceID = 0;


	bool			IsValid() { return (magic == RF24_HEADER_VALID); };

})	RF24Header;

////////////////////////
////////////////////////
///// RF24_PACKET //////
////////////////////////
////////////////////////

#define RF24_PACKET_VALID 0b01101001
#define RF24_PACKET_DATA_SIZE_MAX 20 // RF24 Max size for full packet is 32Bytes (so sizeof(
#define RF24_PACKET_SIZE_MAX 32 // In Bytes

// To Contain Data RF24Packet must be included in a bigger memory range (like a buffer) 



PACK(typedef struct
{
public:
	RF24Header		header;
	uint16_t		time = 0;
	uint16_t		checksum = 0;

	void	Display(bool withHeader = true)
	{
		if (withHeader)
			header.Display();
		Serial << " Checksum (" << checksum << ")\n";
	}

	void			ChecksumSet()
	{
		checksum = ChecksumCompute();
	}

	uint16_t		ChecksumCompute()
	{
		uint16_t	_checksum = 0;
		uint16_t	tmp;
		uint8_t sizeObject = ((sizeof(*this)) - 2); // Checksum is not added
		char* object = (char*)&(*this);


		for (uint8_t i = 0; i < sizeObject; i += 1)
		{
			tmp = (uint16_t)object[i];
			_checksum += ((tmp * 3) % 3750);
		}
		_checksum += (time % 20000);

		return (_checksum);
	}

	char*		GetData()
	{
		char* data = ((char*)this);
		data = &data[sizeof(*this)];
		//Serial << "Size of this (" << sizeof(*this) << ")\n";
		return (data);
	}

	uint8_t			GetSize() const
	{
		return (sizeof(*this) + (header.dataSpec.size));
	}

	void	SetData(char* data, const RF24DataSpecification& dataSpec, bool checksumSet = true)
	{
		// COPY DATA on Packet Memory
		header.dataSpec = dataSpec;
		memcpy(GetData(), data, dataSpec.size);
		if (checksumSet)
			ChecksumSet();
	}

}) RF24Packet;




#define RF24_ACK_VALUE 55050

#define RF24SERVER_ID 42
#define RF24ALL_ID 0

PACK(typedef struct
{
	/*RF24DataBuffer()
	{
		spec.size = 0;
		spec.type = RF24NODATA;
		data = NULL;
	}

	RF24DataBuffer(const RF24DataSpecification& _spec)
	{
		spec.size = _spec.size;
		spec.type = _spec.type;
		data = NULL;
	}*/

	bool	Allocate()
	{
		Release(); // in Case already Allocate
		// Then Allocate
		if (!(data = (char*)malloc(spec.size)))
		{
			return (false);
		}
		//memset((void*)data, 0, spec.size);
		return (true);
	}

	bool	Release()
	{
		if (data)
		{
			free(data);
			data = NULL;
			return (true);
		}
		else
		{
			return (false);
		}
	}

	void	SetDataAddr(char* addr)
	{
		data = addr;
	}

	bool	Concat(char* other, uint16_t size, uint16_t offset = 0)
	{
		if ((offset + size) > spec.size)
			return (false);
		memcpy(&data[offset], other, size);
		return (true);
	}

	// Pack 
	RF24DataSpecification	spec;
	char*					data = NULL;

}) RF24DataBuffer;

//////////////////////////////
//////////////////////////////
///// RF24_COMMUNICATOR //////
//////////////////////////////
//////////////////////////////

class RF24Communicator
{
protected:
	typedef enum RF24State : uint8_t
	{
		NONE = 0,
		SEND_CALL = 1,
		SEND_AUTH_WRITE = 2,
		READ_CALL = 3
	} RF24State;

public:

	RF24Communicator(uint8_t pinCE, uint8_t pinCS) :
#ifndef _WIN32
		_radio(pinCE, pinCS),
#endif // !_WIN32
		_ackValueCertification(RF24_ACK_VALUE)
	{
		Serial << "Constructor\n";
		_memoryBufferUsed = sizeof(*this) + RF24_DATA_FAT_MAX_SIZE; // Size of all essential buffers 
		_memoryBufferMax = 1024; // 1024 octets
		_lastChecksum = 0;
		_deviceID = 0;
		_deviceIDReceiver = RF24ALL_ID;
		_state = RF24State::SEND_CALL; // Server Prior Mode is Send Call
	};

	virtual ~RF24Communicator()
	{
		if (_lastChecksum != 0) // Had been initialized
		{
			delete (_requestFcts);
			_dataFatBuffer.Release();
		}
	}

#if DEBUGRF24_MEMORY
	void	DisplayMemoryUsed()
	{
		Serial << "Memory Used (" << _memoryBufferUsed << ") on (" << _memoryBufferMax << ") Available\n";
	}
#endif

	// Init Basics and then overridable Extended
	bool	InitCommunication()
	{
		Serial << "Init\n";
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
			_lastChecksum = 1; // Last Check Sum is used as marker of Initialization
			// It's set only from Valid Packet and value could never be 0 or 1
#if DEBUGRF24_MEMORY
			DisplayMemoryUsed();
#endif
			return (true);
		}
		else
		{
			Serial << "Already Init\n";
		}
	}

	///////////////////
	///// UPDATE //////
	///////////////////
private:
	uint16_t debugLifeTimeCounter = 0;
public:

	void	UpdateCommunication()
	{
		if (_lastChecksum == 0)
			return;

		UpdateTime();
		debugLifeTimeCounter += _timeDtMs;
		if (debugLifeTimeCounter >= 1000)
		{
			Serial << "RF24Communicator is living\n";
			debugLifeTimeCounter = 0;
		}

		//Serial << "dtMs (" << dtMs << ")\n";
		UpdateState(_timeDtMs);
	}

	// It Return the packet to send and NULL if call is over
	typedef bool (RF24Communicator::*UpdateByStateFct)(); // Previous packet in Parameter and the state of its sending

	void	SetDeviceID(uint8_t deviceID)
	{
		_deviceID = deviceID;
	}

	void	SetDeviceIDReceiver(uint8_t deviceID)
	{
		_deviceIDReceiver = deviceID;
	}

	bool	SendBufferIsEmpty()
	{
		return (_callQueue.Size() == 0 && _callCurrent.call == NULL);
	}

	// Value are basic types implemented by RF24DataType

	///////////////////
	//// RF24 CALL ////
	///////////////////
	PACK(typedef struct
	{
		RF24DataBuffer	data;
		RF24RequestType	requestNext;
		uint8_t			flag; // Can be used for option (also used to pad struct)
		uint16_t		Size() const { return ((sizeof(*this) + data.spec.size)); };
	}) RF24Call; // Bufferized Call

	PACK(typedef struct
	{
		void			Reset()
		{
			call = NULL;
			dataSent = 0;
			packet = NULL;
			packetAttemptCount = 0;
			packetAttemptTime = 0;
		}

		RF24Call*		call = NULL;
		RF24Packet*		packet = NULL;
		uint16_t		packetAttemptCount = 0;
		uint16_t		packetAttemptTime = 0;

		uint16_t		dataSent = 0;


	}) RF24CallPacket; // Call Ready to be Send 


	// It Return the packet to send and NULL if call is over
	typedef RF24Packet* (RF24Communicator::*RequestFct)(RF24CallPacket*, bool success); // Previous packet in Parameter and the state of its sending
	// The function is called when the RF24Call is going to be send and each time the previous packet has been sent
	// ex: RequestFct		nextFct;

	// HERE IS THE PACKET PREPARATION
	template <typename T>
	bool	SendValue(T value)
	{
		RF24Call* call = new RF24Call();
		call->data.spec.SetFromData(value);
		return (PrepareCallSend(call, (char*)&value));
	}

	bool	SendMsg(const char * msg, uint16_t size)
	{
		return (SendData((char*)msg, size, (uint8_t)RF24DataType::RF24STRING));
	}

	bool	SendMsg(String msg)
	{
		return (SendMsg(msg.c_str(), (uint16_t)msg.length()));
	}

	bool	SendData(char* data, uint16_t size, uint8_t type)
	{
		RF24Call* call = new RF24Call();
		call->data.spec.Set(size, type);
		return (PrepareCallSend(call, data));
	}

	///////////////////////////////////////////////////
	////////////////////////////////// PROTECTED /////
protected:

	/////////////////////
	//// OVERRIDABLE ////

	virtual bool InitExtended(const uint64_t& pipeServerWrite, const uint64_t& pipeServerRead) { return (true); };
	virtual bool IsAuthorized(RF24Packet* packetRequest) const { return (true); };
	virtual void OnReadPacket(RF24Packet* packet) {	};
	virtual void OnReadData(char* data, uint16_t dataSize, uint8_t type)
	{
		Serial << "(" << type << ") Say: (";
		if (type == RF24DataType::RF24INT)
			Serial << ((int*)data)[0];
		else if (type == RF24DataType::RF24STRING)
		{
			data[dataSize] = 0;
			Serial << data;
		}
		else
			Serial << data;
		Serial << ")\n";
	}

	virtual void HandlePacket(RF24Packet* packet)
	{
		OnReadPacket(packet);

		// If Packet is !Reply -> 

		if (packet->header.requestType == RF24RequestType::RF24DATA)
			HandleRequestData(packet);
		else if (packet->header.requestType == RF24RequestType::RF24DATA_FAT_BEGIN)
			HandleRequestDataFatBegin(packet);
		else if (packet->header.requestType == RF24RequestType::RF24DATA_FAT_PACKET)
			HandleRequestDataFatPacket(packet);
		else if (packet->header.requestType == RF24RequestType::RF24DATA_FAT_END)
			HandleRequestDataFatEnd(packet);
		else if (packet->header.requestType == RF24RequestType::RF24AUTH_WRITE)
			HandleRequestAuthWrite(packet);
		else
		{
			Serial << "RF24Server: Unkonwn Request\n";
			packet->Display();
		}

		// Else if packet is reply Notify concerned packet of the reply 
	}

	virtual void UpdateState(const unsigned long& dtMs) = 0;

	//// END OVERRIDABLE ////
	/////////////////////////

		// Return dtMs (Delta Time in MilliSeconds since last call)
	void	UpdateTime()
	{

		unsigned long timeLast = _time;
#ifndef _WIN32
		_time = millis();;
#else
		_time = time(NULL);
#endif // !1
		//Serial << "Update Time TimeLast(" << timeLast << ") Time ("<< _time << ")\n";
		unsigned long dtMs = (_time - timeLast);
		if (dtMs > UINT16_MAX)
			_timeDtMs = UINT16_MAX;
		else
			_timeDtMs = (uint16_t)dtMs;
	}

	bool MemoryGet(uint16_t memoryWanted)
	{
		if ((memoryWanted + _memoryBufferUsed) > (_memoryBufferMax - 50)) // To be sure we dont override Memory let Nbits of margin for temporary Mem Manip
		{
#if DEBUGRF24_MEMORY
			Serial << "Memory Get ERROR (" << memoryWanted << " + " << _memoryBufferUsed <<") > (" << (_memoryBufferMax - 50) << ")\n";
#endif
			return (false);
		}
		_memoryBufferUsed += memoryWanted;
#if DEBUGRF24_MEMORY
		Serial << "Memory Get (" << memoryWanted << ")\n";
#endif
		return (true);
	}

	void MemoryRelease(uint16_t memoryRelease)
	{
#if DEBUGRF24_MEMORY
		Serial << "Memory Release (" << memoryRelease << ")\n";
#endif
		_memoryBufferUsed -= memoryRelease;

	}

	bool ReadCall()
	{
		RF24Packet*	packet;

		if ((packet = ReadAvailable()))
		{
			//Serial << "Available on Check Received\n";
			// MSG is for everyone
			if (packet->header.receiverDeviceID == RF24ALL_ID \
				|| packet->header.receiverDeviceID == _deviceID) // MSG is for this Communicator
			{
				HandlePacket(packet);
				Listen(true);
				return (true);
			}
			else
			{
				//Serial.println("Packet is not for me");
			}
		}
		Listen(true);
		return (false);
	}

	bool SendCall()
	{
		// while max try
		if (_callCurrent.call == NULL)
		{
			if (PopCall())
				PopCallNextFct(true);
		}
		return (SendCallPacketCurrent());
	}

	bool SendAuthWrite()
	{
		// ID Device of Future recipient is not known for the moment // So, Send To Everyone 
		RF24Packet* packet = GetPacketSendBuffer();
		packet->header.requestType = RF24RequestType::RF24AUTH_WRITE;
		packet->ChecksumSet();

#if DEBUGRF24_SEND_CALL
		if (SendPacketPP(packet))
		{
			Serial << "Send Auth Write OK\n";
			return (true);
		}
		else
		{
			Serial << "Send Auth Write FAILED\n";
			return (false);
		}
#else
		return (SendPacketPP(packet));
#endif

	}

	RF24Packet*			GetPacketReceivedBuffer() const
	{
		memset((void*)&_packetReceivedBufffer, 0, RF24_PACKET_SIZE_MAX);
		RF24Packet*		packet = (RF24Packet*)&_packetReceivedBufffer;
		return (packet);
	}

	RF24Packet*			GetPacketSendBuffer() const
	{
		memset((void*)&_packetSendBufffer, 0, RF24_PACKET_SIZE_MAX);
		RF24Packet*		packet = (RF24Packet*)&_packetSendBufffer;

		packet->header.receiverDeviceID = _deviceIDReceiver;
		packet->header.senderDeviceID = _deviceID;
		packet->header.dataSpec.size = 0; // In case user do not use Data Size
		packet->header.dataSpec.type = RF24DataType::RF24NODATA; // Same for data type
		packet->header.magic = RF24_HEADER_VALID; // Sign the packet before sending it

		packet->time = (uint16_t)_time;

		return (packet);
	}

	RF24Packet*			GetPacketSendCallBuffer(RF24Call* call) const
	{
		memset((void*)&_packetSendBufffer, 0, RF24_PACKET_SIZE_MAX);
		RF24Packet*		packet = (RF24Packet*)&_packetSendCallBufffer;

		packet->header.receiverDeviceID = _deviceIDReceiver;
		packet->header.senderDeviceID = _deviceID;
		packet->header.dataSpec.size = 0; // In case user do not use Data Size
		packet->header.dataSpec.type = RF24DataType::RF24NODATA; // Same for data type
		packet->header.magic = RF24_HEADER_VALID; // Sign the packet before sending it

		packet->time = (uint16_t)_time;

		return (packet);
	}

	void				Listen(bool state)
	{
		if (_isListening != state) // Do Something
		{
#ifndef _WIN32
			if (state)
			{
				//Serial << "Start Listening\n";
				_radio.startListening();
			}
			else
			{
				//Serial << "Stop Listening\n";
				_radio.stopListening();
			}
#endif
			_isListening = state;
		}
	}


#ifndef _WIN32
	RF24				_radio;  // nRF24L01 (CE,CSN)
#endif // 

	//////////////////////////////
	// ATTRIBUTES PROTECTED //////
	RF24State			_state;
	uint8_t				_requestCount = 5; // If Modified must be set in Constructor
	uint8_t				_deviceID;
	uint8_t				_deviceIDReceiver; // ID of whom the message is send
	const int			_ackValueCertification;
	// END ATTRIBUTES PROTECTED //
	//////////////////////////////

	///////////////////////////////////////////////////
	////////////////////////////////// PRIVATE /////
private:

	bool	PrepareCallSend(RF24Call* call, const char *data)
	{
		if (call->data.spec.size == 0)
		{
			Serial << "ERROR: RF24Communicator Cannot Send Empty Data \n";
			return (false);
		}

		if (call->data.spec.size > RF24_PACKET_DATA_SIZE_MAX)
		{
			if (call->data.spec.size > RF24_DATA_FAT_MAX_SIZE)
			{
				Serial << "ERROR: RF24Communicator Cannot Send Data Fat (Too Fat (" << call->data.spec.size << ") > (" << RF24_DATA_FAT_MAX_SIZE << "))\n";
				return (false);
			}
			call->requestNext = RF24RequestType::RF24DATA_FAT_BEGIN;
		}
		else
		{
			call->requestNext = RF24RequestType::RF24DATA;
		}

		if (!MemoryGet(call->Size() + sizeof(Queue<RF24Call*>::Node)))
		{
#if DEBUGRF24_MEMORY
			Serial << "Prepare Call Send ERROR: Cannot Allocate more memory \n";
			DisplayMemoryUsed();
#endif
			// IF force Try to find Memory 
			// Else Stop 
			return (false);
		}

#if DEBUGRF24_MEMORY
		DisplayMemoryUsed();
#endif
		call->data.Allocate();
		call->data.Concat((char*)data, call->data.spec.size);
		PrepareCall(call);
		return (true);
	}

	void	PrepareCall(RF24Call* call)
	{
		_callQueue.PushBack(call);
		UpdateCommunication(); // Take Opportunity of having time with communicator to update Read/Write
	}

	// 
	RF24Packet*	OnSendCallData(RF24CallPacket* callPacket, bool state)
	{
		//Serial << "OnCallSend\n";
		RF24Packet* packet = GetPacketSendCallBuffer(callPacket->call);


		packet->header.requestType = RF24RequestType::RF24DATA;
		callPacket->call->requestNext = RF24RequestType::RF24NOREQUEST;
		packet->SetData(callPacket->call->data.data, callPacket->call->data.spec);

		return (packet);
	}

	RF24Packet*	OnSendCallDataFatEnd(RF24CallPacket* callPacket, bool state)
	{
		//Serial << "OnCallSendFatEnd\n";
		callPacket->call->requestNext = RF24RequestType::RF24NOREQUEST;
		return (NULL);
	}

	RF24Packet*	OnSendCallDataFatPacket(RF24CallPacket* callPacket, bool state)
	{
		if (!state)
		{
			//Serial << "OnCallSendFat FAILED\n";
			return (NULL);
		}
		//Serial << "OnCallSendFatPack";

		if (callPacket->packet->header.requestType == RF24RequestType::RF24DATA_FAT_PACKET)
		{
			callPacket->dataSent += callPacket->packet->header.dataSpec.size;
		}
		//Serial << " Data Sent (" << callPacket->dataSent << ")\n";


		uint16_t dataLeftToSend = (callPacket->call->data.spec.size - callPacket->dataSent);

		//Serial << "OnCallSendFatPack Data Left To Send(" << dataLeftToSend << ")\n";

		RF24Packet* packet = GetPacketSendCallBuffer(callPacket->call);

		if (dataLeftToSend == 0)
		{
			callPacket->call->requestNext = RF24RequestType::RF24DATA_FAT_END;
			packet->header.requestType = RF24RequestType::RF24DATA_FAT_END;
			packet->ChecksumSet();
		}
		else
		{
			callPacket->call->requestNext = RF24RequestType::RF24DATA_FAT_PACKET;
			packet->header.requestType = RF24RequestType::RF24DATA_FAT_PACKET;

			RF24DataSpecification dataSpec;

			dataSpec.type = callPacket->call->data.spec.type; // Type of data
			if (dataLeftToSend > RF24_PACKET_DATA_SIZE_MAX)
				dataSpec.size = RF24_PACKET_DATA_SIZE_MAX;
			else
				dataSpec.size = dataLeftToSend;
			packet->SetData(&callPacket->call->data.data[callPacket->dataSent], dataSpec);
		}
		return (packet);
	}

	RF24Packet*	OnSendCallDataFatBegin(RF24CallPacket* callPacket, bool state)
	{
		//Serial << "OnCallSendFatBegin Type(" << callPacket->call->data.spec.type << ") Size(" << callPacket->call->data.spec.size << ")\n";
		RF24Packet* packet = GetPacketSendCallBuffer(callPacket->call);

		RF24DataSpecification	dataSpec;

		dataSpec.size = sizeof(RF24DataSpecification);
		dataSpec.type = RF24DataType::RF24DATASPEC;

		packet->header.requestType = RF24RequestType::RF24DATA_FAT_BEGIN;
		callPacket->call->requestNext = RF24RequestType::RF24DATA_FAT_PACKET;

		packet->SetData((char*)&callPacket->call->data.spec, dataSpec);
		return (packet);
	}

	bool		InitBasics()
	{
		// INFO On Everyting
		Serial << "RF24Packet Size (" << sizeof(RF24Packet) << ")\n";
		Serial << "RF24DataBuffer Size (" << sizeof(RF24DataBuffer) << ")\n";
		Serial << "RF24DataSpecification Size (" << sizeof(RF24DataSpecification) << ")\n";
		Serial << "RF24CallPacket Size (" << sizeof(RF24CallPacket) << ")\n";
		Serial << "RF24Call Size (" << sizeof(RF24Call) << ")\n";
		InitRequestFcts();

#ifndef _WIN32
		_radio.begin();
		delay(500);

		_radio.setPALevel(RF24_PA_MIN);
		//_radio.setPALevel(RF24_PA_MAX);
		//_radio.setDataRate(RF24_2MBPS);
		_radio.setRetries(0, 0);
		//_radio.setAutoAck(true);
		//_radio.enableAckPayload();
		_radio.enableDynamicPayloads();
		_radio.powerDown();
		_radio.powerUp();
#endif // _WIN32
		return (true);
	}

	/////////////////
	///// READ //////
	/////////////////


	RF24Packet* ReadAvailable(uint8_t onPipe = 0) // 0 -> No Specific Pipe
	{
		uint8_t		pipe_num;
		RF24Packet* packet = GetPacketReceivedBuffer();
#ifndef _WIN32

		if (_radio.available(&pipe_num))
		{
#if DEBUGRF24_READ
			Serial << "Read on Pipe (" << pipe_num << ") OK\n";
#endif

			uint8_t payloadSize = _radio.getDynamicPayloadSize();
			_radio.read((char*)&packet[0], payloadSize);

			Listen(false);
			if (payloadSize >= sizeof(RF24Packet)) // Packet too
			{
#if DEBUGRF24_READ
				Serial << "Read Packet\n";
#endif
				//packet->Display();
				//Du1mpObject((char*)&packet[0], packet->GetSize());

				if (packet->header.IsValid()) // Header is Valid
				{
					if (packet->checksum != packet->ChecksumCompute() || packet->checksum == 0)
					{
#if DEBUGRF24_READ
						Serial << "Read Packet ERROR with bad Checksum (" << packet->checksum << ") != (" << packet->ChecksumCompute() << ")\n";
#endif
						return (NULL);
					}
					if (packet->checksum == _lastChecksum)
					{
#if DEBUGRF24_READ
						Serial << "Read Packet ERROR same Packet then last one Received (" << packet->checksum << ") == (" << _lastChecksum << ")\n";
#endif
						//packet->Display();
						return (NULL);
					}
					_lastChecksum = packet->checksum;
					if (onPipe == 0 || pipe_num == onPipe)
					{
						//Serial.println(" Valid");
						//DumpObject((char*)&packet[0], packet->GetSize());
#if DEBUGRF24_READ
						Serial << "Read Packet with Checksum (" << packet->checksum << ") OK\n";
#endif
						return (packet);
					}
				}
				else
				{
					//Serial.println(" With bad Header");
					//DumpObject((char*)&packet[0], payloadSize);
					return (NULL);
				}
			}
			else
			{
				//Serial.print("Data Unknown of Size ("); Serial.print(payloadSize); Serial.println(")");
				return (NULL);
			}

		}
		//Serial.println("Unavailable");
#endif
		return (NULL);
	}

	/////////////////
	///// SEND //////
	/////////////////

	//void		SendPacketPrepare(RF24Packet* packetToSend)
	//{	
	//	// Prepare Packet For Send
	//	packetToSend->ChecksumSet();
	//}

	// Considering that you are listening on the good port
	bool SendPacketPP(RF24Packet* packetToSend)
	{
#ifndef _WIN32
		Listen(false);
#else
		if (rand() % 4 == 0) // Fake success of sending packet or not
			return (true);
		else
			return (false);
#endif

		//uint8_t			pipe_num;
		//uint8_t			ppRequestTypeExpected = _ppRequestTypeExpected;

		uint8_t			connectionTryResponse = 0;
		const uint8_t	connectionTryResponseMax = 10;
		const uint8_t	connectionTryResponseDelay = 20; // in ms


		//packetToSend->Display();
		//DumpObject((char*)&packetToSend[0], packetToSend->GetSize());


#ifndef _WIN32

#if DEBUGRF24_SEND
		Serial << "Send PacketPP CS(" << packetToSend->checksum << ")\n";
#endif
		if (_radio.write(packetToSend, packetToSend->GetSize()))
		{
#if DEBUGRF24_SEND
			Serial << "Send PacketPP WRITE OK \n";
#endif
			//Serial << "Sent Packet\n";
			//packetToSend->Display();
			//DumpObject((char*)&packetToSend[0], packetToSend->GetSize());
//
//			while (!_radio.isAckPayloadAvailable() && connectionTryResponse < connectionTryResponseMax)
//			{
//				//Serial.println("Connection Try Response");
//				connectionTryResponse += 1;
//				delay(connectionTryResponseDelay);
//			}
//			////_radio.startListening();
//			if (connectionTryResponse < (connectionTryResponseMax - 1))
//			{
//				int	ackValue;
//				_radio.read(&ackValue, sizeof(ackValue));
//			
//#if DEBUGRF24_SEND
//				Serial << "Send PacketPP ACK OK \n";
//#endif
//				Listen(true);
//				return (true);
//			}
			Listen(true);
			return (true);
			//Serial << "SUCCES\n";

		}
#if DEBUGRF24_SEND
		Serial << "Send PacketPP  FAILED \n";
#endif

		Listen(true);
#endif
		return (false);
	}

	/////////////
	// SENDING //
	/////////////

	//////////
	// CALL //

	void CloseCall(bool withSuccess)
	{
		if (_callCurrent.call)
		{
#if DEBUGRF24_SEND_CALL
			Serial << "Close Call (" << _callCurrent.call->requestNext << ") Success (" << withSuccess << ")\n";
#endif
			
			MemoryRelease(_callCurrent.call->Size() + sizeof(Queue<RF24Call*>::Node));
#if DEBUGRF24_MEMORY
			DisplayMemoryUsed();
#endif
			// Call Is done Delete his data and him
			if (_callCurrent.call->data.data)
				_callCurrent.call->data.Release();
			delete(_callCurrent.call);
		}
		_callCurrent.Reset();
	}

	bool PopCall()
	{
		if (!_callQueue.Size())
			return (false);
		//Serial << "Pop Call\n";
		_callCurrent.call = _callQueue.Front();
		_callQueue.PopFront();
		return (true);
	}

	bool PopCallNextFct(bool successPrev)
	{
		//Serial << "Pop Call Packet\n";
		_callCurrent.packetAttemptCount = 0;
		_callCurrent.packetAttemptTime = 0;
		if (_callCurrent.call->requestNext == RF24RequestType::RF24NOREQUEST)
		{
			CloseCall(true);
			return (false);
		}
		// Even if last packet failed try next request to notify That last packet failed and call is going to be close 
		// Else get an other packet, else close call
		if (!(_callCurrent.packet = (this->*_requestFcts[_callCurrent.call->requestNext])(&_callCurrent, successPrev)))
		{
			CloseCall(successPrev);
			return (false);
		}
		if (!successPrev) // Succes Failed Close Packet
			CloseCall(false);
		_callCurrent.packet->ChecksumSet();
		//Serial << "Going to send packet with checksum (" << _callCurrent.packet->checksum << ")\n";
		return (true);
	}

	bool SendCallPacketCurrent()
	{
		if (!_callCurrent.packet || !_callCurrent.call)
			return (false);
		if (_callCurrent.packetAttemptCount) // Not first time
		{
#if DEBUGRF24_SEND_CALL
			Serial << "Send Call (" << _callCurrent.call->requestNext << ") Time Packet (" << _callCurrent.packetAttemptTime << ")\n";
#endif
			_callCurrent.packetAttemptTime += _timeDtMs;
			const uint16_t	maxAttemptTime = 100; // in Ms
			if (_callCurrent.packetAttemptTime > maxAttemptTime)
			{
				PopCallNextFct(false); // Signal to the Call function that previous packet failed to be send
				return (false);
			}
		}


		const uint8_t	minAttemptCount = 2;
		const uint8_t	maxAttemptCount = 10;


		if (SendPacketPP(_callCurrent.packet))
		{
#if DEBUGRF24_SEND_CALL
			Serial << "Send Call (" << _callCurrent.call->requestNext << ") OK\n";
#endif
			PopCallNextFct(true);
			return (true);
		}
		else
		{
#if DEBUGRF24_SEND_CALL
			Serial << "Send Call (" << _callCurrent.call->requestNext << ") FAILED Try (" << _callCurrent.packetAttemptCount << ")\n";
#endif
			_callCurrent.packetAttemptCount += 1;
		}
	}

	// END CALL //
	//////////////

	///////////////
	// RECEIVING //
	///////////////

	struct RF24DataFatBuffer
	{
		RF24DataFatBuffer()
		{
			_dataStep = 0;
		}

		bool	Init(uint16_t size, uint8_t type)
		{
			_dataStep = 0;
			_data.spec.size = (size + 1); // To Properly Mark end of Data Fat with a 0
			if (_data.spec.size > RF24_DATA_FAT_MAX_SIZE)
				return (false);
			_data.spec.type = (RF24DataType)type;
			if (_data.Allocate())
			{
				_data.data[_data.spec.size - 1] = 0;
				return (true);
			}
			return (false);
		}

		bool	Concat(char* data, uint16_t size)
		{
			if (_data.Concat(data, size, _dataStep))
			{
				_dataStep += size;
				return (true);
			}
			return (false);
		}

		bool	Release()
		{
			_dataStep = 0;
			return (_data.Release());
		}

		bool	IsValid() const { return (_dataStep == (_data.spec.size - 1)); }; /// handle the case where size == 0 With overflow of (0 - 1)

		char*			Data() const { return (_data.data); };
		const uint16_t&	DataStep() const { return (_dataStep); };
		const uint16_t&	Size() const { return (_data.spec.size); };
		const uint8_t&	Type() const { return (_data.spec.type); };

	private:

		uint16_t				_dataStep;
		RF24DataBuffer			_data;
	};

	void				HandleRequestData(RF24Packet* packet)
	{
		OnReadData(packet->GetData(), (uint16_t)packet->header.dataSpec.size, (uint8_t)packet->header.dataSpec.type);
	}

	void				HandleRequestDataFatBegin(RF24Packet* packet)
	{
		RF24DataSpecification*	dataFatSpec = (RF24DataSpecification*)&packet->GetData()[0];
		if (!_dataFatBuffer.Init(dataFatSpec->size, dataFatSpec->type))
		{
#if DEBUGRF24_READ_REQUEST
			Serial << "Read Data Fat Begin ERROR: Cannot Allocate Memory Or Received Bad Data Fat Spec : Size (" << dataFatSpec->size << ") Type(" << dataFatSpec->type << ")\n";
#endif
			return;
		}

#if DEBUGRF24_MEMORY
		DisplayMemoryUsed();
#endif

#if DEBUGRF24_READ_REQUEST
		Serial << "Read Data Fat Begin : Size (" << dataFatSpec->size << ") Type(" << dataFatSpec->type << ") OK\n";
#endif
	}

	void				HandleRequestDataFatPacket(RF24Packet* packet)
	{
		if (!_dataFatBuffer.Concat(packet->GetData(), packet->header.dataSpec.size))
		{
#if DEBUGRF24_READ_REQUEST
			Serial << "Read Data Fat Packet ERROR: Bad Data Packet Size : Data is probably Corrupted \n";
#endif
			return;
		}
#if DEBUGRF24_READ_REQUEST
		Serial << "Read Data Fat Packet OK\n";
#endif
	}

	void				HandleRequestDataFatEnd(RF24Packet* packet)
	{
		if (!_dataFatBuffer.IsValid())
		{
#if DEBUGRF24_READ_REQUEST
			Serial << "Read Data Fat End ERROR: Incorrect Data Size (" << _dataFatBuffer.Size() << ") != (" << _dataFatBuffer.DataStep() << ") : Data is probably Corrupted \n";
#endif
			return;
		}
#if DEBUGRF24_READ_REQUEST
		Serial << "Read Data Fat End OK\n";
#endif
		OnReadData(_dataFatBuffer.Data(), _dataFatBuffer.Size(), _dataFatBuffer.Type());
#if DEBUGRF24_MEMORY
		DisplayMemoryUsed();
#endif
		
	}

	void				HandleRequestAuthWrite(RF24Packet* packet)
	{
#if DEBUGRF24_READ_REQUEST
		Serial << "Read Auth Write OK\n";
#endif
		SendCall();
		// If multi clients is implemented // Here it would be possible to now from packet who want to write
	}

	bool	InitRequestFcts()
	{
		if (MemoryGet(sizeof(RequestFct) * _requestCount))
		{
			_requestFcts = new RequestFct[_requestCount];
			if (!_requestFcts)
			{
				Serial << "Init Request Fcts ERROR: Cannot Allocate request Fcts\n";
				return (false);
			}
			_requestFcts[RF24RequestType::RF24DATA] = &RF24Communicator::OnSendCallData;
			_requestFcts[RF24RequestType::RF24DATA_FAT_BEGIN] = &RF24Communicator::OnSendCallDataFatBegin;
			_requestFcts[RF24RequestType::RF24DATA_FAT_PACKET] = &RF24Communicator::OnSendCallDataFatPacket;
			_requestFcts[RF24RequestType::RF24DATA_FAT_END] = &RF24Communicator::OnSendCallDataFatEnd;
			return (true);
		}
		else
		{
			Serial << "Init Request Fcts ERROR: Cannot Allocate request Fcts\n";
			return (false);
		}
	}

	/////////////////////
	// END RECEIPTION ///
	/////////////////////

	uint16_t			_lastChecksum;
	char				_packetSendBufffer[RF24_PACKET_SIZE_MAX]; // For Direct Send and no retry
	char				_packetSendCallBufffer[RF24_PACKET_SIZE_MAX]; // For Call Buffering
	char				_packetReceivedBufffer[RF24_PACKET_SIZE_MAX];

	// Calls Managing (Bufferization)
	RF24CallPacket		_callCurrent;
	Queue<RF24Call*>	_callQueue;
	RequestFct*			_requestFcts;


	// Receiving Data Fat
	RF24DataFatBuffer	_dataFatBuffer;

	unsigned long		_time;
	uint16_t			_timeDtMs;
	bool				_isListening = false;
	uint16_t			_memoryBufferUsed; // Size Of Datas + Size Of Calls + Size Of Nodes + Size Of ItSelf + Size of Data Buffer
	uint16_t			_memoryBufferMax;
};

#endif // __RF24SC_H__