#ifndef __RF24SCClient_H__
#define __RF24SCClient_H__

#include "RF24SC.h"

class RF24Client : public RF24Communicator
{
public:

	RF24Client(uint8_t pinCE, uint8_t pinCS) : RF24Communicator(pinCE, pinCS)
	{
		_state = RF24State::READ_CALL;
		Listen(true);
		//_connection.clientID = RF24SERVER_CLIENT_UNKNOWN; // UNDEFINED
	}

	virtual ~RF24Client()
	{
	}

private:

	virtual void UpdateState(const unsigned long& dtMs) override
	{
		ReadCall(); // Write only if Handle Write Auth
	}

	virtual bool InitExtended(const uint64_t& pipeServerWrite, const uint64_t& pipeServerRead) override
	{
#ifndef _WIN32
		_radio.openReadingPipe(1, pipeServerWrite); // READ On Write of Server (0)
		_radio.openWritingPipe(pipeServerRead); // WRITE on Read of Server (1)
		_radio.writeAckPayload(pipeServerWrite, &_ackValueCertification, sizeof(int));
		_radio.startListening();
#endif
		return (true);
	}
	//////////////////
	///// UPDATE /////
	//////////////////



};

#endif // __RF24SCClient_H__
