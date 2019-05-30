#ifdef _WIN32
#include <stdint.h>
#else

#endif


#include <RobovacCommands.h>
#include <RF24SCClient.h>

class RobovacSC : public RF24Client // Can Become a client too
{
public:

	RobovacSC(uint8_t pinCE, uint8_t pinCS) : RF24Client(pinCE, pinCS)
	{

	}

private:

	
	virtual void	OnReadData(char* data, uint16_t dataSize, uint8_t type) override
	{
		RF24Client::OnReadData(data, dataSize, type);
		Serial << "RobovacSC on Read Data of type: (" << type << ") \n";
		if (type == RobovacRF24DataType::COMMAND)
		{
			Serial << "RobovacSC on Read a Command\n";
			RobovacCommand* command = (RobovacCommand*)data;
			Serial << "Command Type (" << command->type <<")\n";
			if (command->type == RobovacCommandType::RBVC_CMD_CALIBRATE)
			{
				OnReadCommandCalibrate(command);
			}
			else if (command->type == RobovacCommandType::RBVC_CMD_MOVEMENT)
			{
				Serial << "Robovac Command Movement\n";
				RobovacCommandMovement* commandMovement = (RobovacCommandMovement*)command;
				OnReadCommandMovement(commandMovement);
			}

		}
	}

	virtual void	OnReadCommandCalibrate(RobovacCommand* command) = 0;
	virtual void	OnReadCommandMovement(RobovacCommandMovement* command) = 0;
	
};



