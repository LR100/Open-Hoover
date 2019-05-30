#include <RobovacControllerSC.h>

#define RF24_PIN_CE 7
#define RF24_PIN_CS 8

class RobovacController : public RobovacControllerSC
{
  public: 
    RobovacController() : RobovacControllerSC(RF24_PIN_CE, RF24_PIN_CS)
    {
      
    }

  void Initialize() 
  {
     InitCommunication();
  }

  void Operate(unsigned long dtMs)
  {
    _dtMs = (float)dtMs;
    UpdateCommunication(); // Update Server
    SendCommandMovement(MovementType::FORWARD);
    SendMsg("Coucou");
  }

  private:


  float               _dtMs;
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
