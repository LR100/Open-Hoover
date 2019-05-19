#ifdef _WIN32
#include <stdint.h>
#else

#endif

//#include <RF24SC/RF24SCClient.h>
#include <Robovac/RobovacDefines.h>
#include <RF24SC/RF24SC.h>

typedef enum RobovacCommandType : uint8_t
{
	CALIBRATE = 0,
	MOVEMENT = 1,
	STOP, // Low power mode but still listening
	START,
	RESTART,
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
	MoveType	type;
	float		value;
};
