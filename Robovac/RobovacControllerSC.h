#ifdef _WIN32
#include <stdint.h>
#else

#endif

#include <RobovacCommands.h>
#include <RF24SCServer.h>

class RobovacControllerSC : public RF24Server // Can Become a client too
{
public:

	RobovacControllerSC(uint8_t pinCE, uint8_t pinCS) : RF24Server(pinCE, pinCS)
	{

	}

	void SendCommandMovement(MovementType move, float power = 10.0f, bool forceSendBuffer = false)
	{
		RobovacCommandMovement movementCmd; 

		movementCmd.movementType = move;
		movementCmd.power = power;
		Serial << "Send Command Movement of type (" << movementCmd.type << ")\n";
		
		if (SendBufferIsEmpty() || forceSendBuffer)
			SendCommand(&movementCmd, sizeof(RobovacCommandMovement));
	}

	void SendCommandCalibrate()
	{
		RobovacCommandCalibrate	calibrateCmd;
		SendCommand(&calibrateCmd, sizeof(RobovacCommandCalibrate));
	}

private:

	
	virtual void	OnReadData(char* data, uint16_t dataSize, uint8_t type) override
	{
		RF24Server::OnReadData(data, dataSize, type);
		if (type == RobovacRF24DataType::CALIBRATE_INFO)
		{
			OnReadCalibrateInfo();
		}
	}
	
	void	OnReadCalibrateInfo()
	{

	}

	void	SendCommand(RobovacCommand* command, uint16_t commandSize)
	{
		SendData((char*)command, commandSize, RobovacRF24DataType::COMMAND);
	}
};



