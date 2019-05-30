#include <RobovacControllerSC.h>

#define RF24_PIN_CE 7
#define RF24_PIN_CS 8

class JoyStick 
{
public:
  JoyStick(uint8_t pinX, uint8_t pinY, uint8_t pinSW) : _pinX(pinX), _pinY(pinY), _pinSW(pinSW)
  {
    pinMode(_pinSW, INPUT);
    digitalWrite(_pinSW, HIGH);
  }

  void  Update()
  { 
    _x = analogRead(_pinX);
    _y = analogRead(_pinY);
    _sw = !digitalRead(_pinSW);
  }

  const float  GetX() 
  { 
    return (InterpolateValue(_x));
  }
  
  const float GetY()
  {
    return (-InterpolateValue(_y));
  }

  const bool GetSW()
  {
    return (_sw);
  }

private:

  const float InterpolateValue(uint16_t value)
  {
      float interpolated = value;
      interpolated -= 512.0f;
      interpolated /= 512.0f;
      return (interpolated);
  }

  bool          _sw;
  uint16_t      _x;
  uint16_t      _y;
  const uint8_t _pinX; // analog pin connected to X output
  const uint8_t _pinY; // analog pin connected to Y output
  const uint8_t _pinSW; // digital pin connected to switch output
};

#define JOYSTICK_PIN_X 0
#define JOYSTICK_PIN_Y 1
#define JOYSTICK_PIN_SW 2

class RobovacController : public RobovacControllerSC
{
  public:
  
  RobovacController() : RobovacControllerSC(RF24_PIN_CE, RF24_PIN_CS), _joyStick(JOYSTICK_PIN_X, JOYSTICK_PIN_Y, JOYSTICK_PIN_SW)
  {
        
  }

  void Initialize() 
  {
     InitCommunication();
  }

  void Operate(unsigned long dtMs)
  {
    _dtMs = (float)dtMs;
    CheckJoystick();
    
    UpdateCommunication(); // Update Server
    
  }

  private:

  void  CheckJoystick()
  {
    _joyStick.Update();

  float x = _joyStick.GetX();
  float y = _joyStick.GetY();
  //Serial << "Joystick X: (" << _joyStick.GetX() << ") Y: (" << _joyStick.GetY() << ")\n";

  if (_joyStick.GetSW()){
    SendCommandCalibrate();
  }
    
    if (abs(x) > 0.02) {
      // Rotate
      if (x < 0) {
        SendCommandMovement(MovementType::ROTATE_LEFT, abs(x) / 2);
      } else { 
       SendCommandMovement(MovementType::ROTATE_RIGHT, abs(x) / 2);
      }
    } else if (abs(y) > 0.02) {
      if (y < 0) { // BackWard
        SendCommandMovement(MovementType::FORWARD, abs(y) );
      } else { // Forward
        SendCommandMovement(MovementType::BACKWARD, abs(y));
      }
    }
  }

  float               _dtMs;
  JoyStick            _joyStick;
};

/////////////////////////////////////////////////////
//////////////// END ROBOVAC CONTROLLER //////////////////////////////
///////////////////////////////////////////////////////////

RobovacController       robovacController;

unsigned long timeStart;
unsigned long timeElapsed;
unsigned long timeCurrent;

void setup() {
	Serial.begin(9600);
	// put your setup code here, to run once:
	timeStart = millis();
  robovacController.Initialize();
}


void loop()
{
	delay(10); // Avoid Useless CPU Consumption
	timeCurrent = millis();

	timeElapsed = timeCurrent - timeStart;
	timeStart = timeCurrent;
  robovacController.Operate(timeElapsed);
	// put your main code here, to run repeatedly:

}
