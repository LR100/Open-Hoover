#ifdef _WIN32
#include <stdint.h>
#else

#endif

//#include <RF24SC/RF24SCClient.h>
#include <RobovacDefines.h>
#include <RF24SC.h>

typedef enum RobovacCommandType : uint8_t
{
	RBVC_CMD_CALIBRATE = 0,
	RBVC_CMD_MOVEMENT = 1,
	RBVC_CMD_STOP, // Low power mode but still listening
	RBVC_CMD_START,
	RBVC_CMD_RESTART,
} RobovacCommandType;

typedef enum RobovacRF24DataType : uint16_t
{
	COMMAND = RF24DataType::RF24USER_DATA_TYPE,
	CALIBRATE_INFO,
} RobovacRF24DataType;


struct RobovacCommand
{
	RobovacCommandType		type;
};

struct RobovacCommandMovement : public RobovacCommand
{
	RobovacCommandMovement() {
		type = RobovacCommandType::RBVC_CMD_MOVEMENT;
	}

	MovementType	movementType;
	float			power;
};

struct RobovacCommandCalibrate : public RobovacCommand
{
	RobovacCommandCalibrate() {
		type = RobovacCommandType::RBVC_CMD_CALIBRATE;
	}
};
