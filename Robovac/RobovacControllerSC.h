#ifdef _WIN32
#include <stdint.h>
#else

#endif


#include <Robovac/RobovacCommands.h>
#include <RF24SC/RF24SCServer.h>

class RobovacControllerSC : public RF24Server // Can Become a client too
{
public:

	RobovacControllerSC(uint8_t pinCE, uint8_t pinCS) : RF24Server(pinCE, pinCS)
	{

	}

private:

	
	virtual void	OnReadData(char* data, uint16_t dataSize, uint8_t type) override
	{
		RF24Server::OnReadData(data, dataSize, type);
		if (type == RobovacRF24DataType::CALIBRATE_INFO)
		{
			
		}
	}
	
	void	OnReadCalibreateInfo()
	{

	}

	void	SendCommand(RobovacCommand* command, uint16_t commandSize)
	{
		SendData((char*)command, commandSize, RobovacRF24DataType::COMMAND);
	}
};



