#ifdef _WIN32
#include <stdint.h>
#else

#endif


#include <Robovac/RobovacCommands.h>
#include <RF24SC/RF24SCClient.h>

class RobovacSC : public RF24Client // Can Become a client too
{
public:

	RobovacSC(uint8_t pinCE, uint8_t pinCS) : RF24Client(pinCE, pinCS)
	{

	}

private:

	
	virtual void	OnReadData(char* data, uint16_t dataSize, uint8_t type) override
	{
		if (type == RobovacRF24DataType::COMMAND)
		{
			RobovacCommand* command = (RobovacCommand*)data;
			if (command->type == RobovacCommandType::CALIBRATE)
			{
				OnReadCommandCalibrate(command);
			}
			else if (command->type == RobovacCommandType::MOVEMENT)
			{
				RobovacCommandMovement* commandMovement = (RobovacCommandMovement*)command;
				OnReadCommandMovement(commandMovement);
			}

		}
	}

	virtual void	OnReadCommandCalibrate(RobovacCommand* command) = 0;
	virtual void	OnReadCommandMovement(RobovacCommandMovement* command) = 0;
	
};



