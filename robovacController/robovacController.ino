#include <RobovacControllerSC.h>

#define RF24_PIN_CE 7
#define RF24_PIN_CS 8

#include <MemoryFree.h> // TO VERIFY Memomry Used

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
        Serial << "Robovac Controller is Init\n";
  }

  void Initialize() 
  {
     InitCommunication();
  }

  void Operate(unsigned long dtMs)
  {
    _dtMs = (float)dtMs;
   // CheckJoystick();
    static int i = 0;
    UpdateCommunication(); // Update Server
   SendMsg("Wesh" + String(i)); 
   i += 1;
  }

  private:

  void  CheckJoystick()
  {
    _joyStick.Update();
    int         cmdJoy;
    static int  lastCmdJoy = 0;
    static int  timeSinceLastCmd = 0;
    int         sameCmdCount = 0;
    const int   maxTimeCountCmd = 500; // After ms Launch cmd

    float x = _joyStick.GetX();
    float y = _joyStick.GetY();
    //Serial << "Joystick X: (" << _joyStick.GetX() << ") Y: (" << _joyStick.GetY() << ")\n";

    cmdJoy = 0;

    if (_joyStick.GetSW()){
      cmdJoy = 1;
    } else if (abs(x) > 0.02) {
      // Rotate
      if (x < 0) {
        cmdJoy = 2;
      } else { 
        cmdJoy = 3;
      }
    } else if (abs(y) > 0.02) {
      if (y < 0) { // BackWard
        cmdJoy = 4;
      } else { // Forward
        cmdJoy = 5;
      }
    }  
    
//    if (cmdJoy != 0)
//    {
//        timeSinceLastCmd = 0;
//        lastCmdJoy = cmdJoy;
//        
//    } else {
//      
//        if (lastCmdJoy != 0)
//        {
//            timeSinceLastCmd += _dtMs;
//            
//              if (timeSinceLastCmd > maxTimeCountCmd) {
//                  // Execute
//                
//                  lastCmdJoy = 0; // Remove last Cmd  
//              }
//        }
//    }
    doCommand(cmdJoy, 1, x, y);
  }

  void doCommand(int cmd, int count, float x, float y) {
    if (cmd == 1) {
      if (count == 1) {// Double Click SW -> Calibrate
        SendCommandCalibrate();
      } else if (count == 0) {
        
      }
      
    } else if (cmd == 2) {
      SendCommandMovement(MovementType::ROTATE_LEFT, abs(x) / 2);  
    } else if (cmd == 3) {
      SendCommandMovement(MovementType::ROTATE_RIGHT, abs(x) / 2);
    } else if (cmd == 4) {
      SendCommandMovement(MovementType::FORWARD, abs(y) );  
    } else if (cmd == 5) {
      SendCommandMovement(MovementType::BACKWARD, abs(y));  
    }
  }

  float               _dtMs;
  JoyStick            _joyStick;
  float               _dtLastCmd;
};

/////////////////////////////////////////////////////
//////////////// END ROBOVAC CONTROLLER //////////////////////////////
///////////////////////////////////////////////////////////

RobovacController*       robovacController;

#include <printf.h>

unsigned long timeStart;
unsigned long timeElapsed;
unsigned long timeCurrent;

void setup() {
	Serial.begin(9600);
    while(!Serial);
   // This is needed so that radio.printDetails(); actually works!!!!
  printf_begin();
  Serial << "Robovac Controller Go !\n";
	 //put your setup code here, to run once:
	timeStart = millis();
 robovacController = new RobovacController();
  robovacController->Initialize();
}


void loop()
{
  while (true)
  {
	delay(10); // Avoid Useless CPU Consumption
	timeCurrent = millis();
  //Serial.print("freeMemory()=");
  //Serial.println(freeMemory());
	timeElapsed = timeCurrent - timeStart;
	timeStart = timeCurrent;
  robovacController->Operate(timeElapsed);
	// put your main code here, to run repeatedly:
  }

}












////
////// Load up the libraries
////#include <SPI.h>
////#include <nRF24L01.h>
////#include <RF24.h>
////#include <printf.h>
////
////// Set the pins up
////#define CE_PIN  7
////#define CSN_PIN 8
////
////// A pipe is needed
////const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe
////
////// Create a Radio
////RF24 radio(CE_PIN, CSN_PIN); 
////
////void setup() 
////{
////  // Start Serial
////  Serial.begin(9600);
////  while(!Serial);
////  
////  // Saying hello to you
////  Serial.println("NRF24L01P test Script starting...");
////  Serial.println("If you do not see a long message after this line, something is not right!");
////  
////  // This is needed so that radio.printDetails(); actually works!!!!
////  printf_begin();
////
////  
////  // Start the radio
////  radio.begin();
////  
////  // Start the radio, again set ti min & slow as I'm guessing while testing theire really close to each oth
////  radio.setPALevel(RF24_PA_MIN);   // RF24_PA_MIN ,RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
////  radio.setDataRate( RF24_250KBPS ); // RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
////  
////  // Set the pipe
////  radio.openReadingPipe(1,pipe);
////  
////  // Start listening
////  radio.startListening();
////  
////  // And cruically print the details
////  radio.printDetails();
////}
////void loop()
////{
////  // nothing here on purpose :)
////}
////
/////*
////======
////You should be seeing something like this, if you are, have a beer, throw a party my friend, she's working :)
////======
////STATUS     = 0x0e RX_DR=0 TX_DS=0 MAX_RT=0 RX_P_NO=7 TX_FULL=0
////RX_ADDR_P0-1   = 0x3130303030 0xe8e8f0f0e1
////RX_ADDR_P2-5   = 0xc3 0xc4 0xc5 0xc6
////TX_ADDR    = 0x3130303030
////RX_PW_P0-6   = 0x20 0x20 0x00 0x00 0x00 0x00
////EN_AA    = 0x3f
////EN_RXADDR  = 0x02
////RF_CH    = 0x4c
////RF_SETUP   = 0x01
////CONFIG     = 0x0f
////DYNPD/FEATURE  = 0x00 0x00
////Data Rate  = 1MBPS
////Model    = nRF24L01+
////CRC Length   = 16 bits
////PA Power   = PA_MIN
////======
////And it's NOT working if you see the below. Check the pins again (and again!)
////======
////STATUS     = 0x00 RX_DR=0 TX_DS=0 MAX_RT=0 RX_P_NO=0 TX_FULL=0
////RX_ADDR_P0-1   = 0xffffffffff 0xffffffffff
////RX_ADDR_P2-5   = 0xff 0xff 0xff 0xff
////TX_ADDR    = 0xffffffffff
////RX_PW_P0-6   = 0xff 0xff 0xff 0xff 0xff 0xff
////EN_AA    = 0xff
////EN_RXADDR  = 0xff
////RF_CH    = 0xff
////RF_SETUP   = 0xff
////CONFIG     = 0xff
////DYNPD/FEATURE  = 0xff 0xff
////Data Rate  = 1MBPS
////Model    = nRF24L01
////CRC Length   = 16 bits
////PA Power   = PA_MAX
////*/
//
//// Load up the libraries
//#include <SPI.h>
//#include <nRF24L01.h>
//#include <RF24.h>
//
//// define the pins
//#define CE_PIN   7
//#define CSN_PIN 8
//
//// Create a Radio
//RF24 radio(CE_PIN, CSN_PIN); 
//
//// The tx/rx address
//const byte rxAddr[6] = "00001";
//
//void setup()
//{
//
//  // Start the serial
//  Serial.begin(9600);
//  while(!Serial);
//  Serial.println("NRF24L01P Receiver Starting...");
//  
//  // Start the radio, again set to min & slow as I'm guessing while testing theire really close to each other
//  radio.begin();
//  radio.setPALevel(RF24_PA_MIN);   // RF24_PA_MIN ,RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
//  radio.setDataRate( RF24_250KBPS ); // RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
//  
//  // Set the reading pipe and start listening
//  radio.openReadingPipe(0, rxAddr);
//  radio.startListening();
//  
//}
//
//void loop()
//{
//  if (radio.available())
//  {
//    // the buffer to store the received message in
//    char text[100] = {0};
//    
//    // Now read the message, old examples have done = radio.read(), that doesn't work anymore!!!
//    radio.read(&text, sizeof(text));
//    
//    // Print the message out to the COM window
//    Serial.println("Received Message: ");
//    Serial.print(text);
//    Serial.println("");
//  } 
//}
