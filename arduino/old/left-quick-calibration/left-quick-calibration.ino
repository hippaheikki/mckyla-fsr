/*For more information see www.ladyada.net/learn/sensors/fsr.html */

#define BASE_PRESSURE  0

#define CALIBRATE_LOW_THRESHOLD = 150
#define CALIBRATE_MEDIUM_THRESHOLD = 300
#define CALIBRATE_HIGH_THRESHOLD = 450

#define INITIAL_LEFT_PRESSURE  1000
#define INITIAL_DOWN_PRESSURE  1000
#define INITIAL_RIGHT_PRESSURE 1000
#define INITIAL_UP_PRESSURE    1000

#include <Keyboard.h>

int fsrAnalogPin = 0; // FSR is connected to analog 0
int LURD_pins[4] = {0, 2, 3, 1};
int LURD_Values[4] = {0, 0, 0, 0};
int LURD_State[4] = {0, 0, 0, 0};
int LURD_pressures[4] = {INITIAL_LEFT_PRESSURE, INITIAL_UP_PRESSURE, INITIAL_RIGHT_PRESSURE, INITIAL_DOWN_PRESSURE};
int LURD_calibration_pressures[4] = {0, 0, 0, 0};
int startupCalibrationThreshold = CALIBRATE_MEDIUM_THRESHOLD;

int oldValueWeight = 1;
float releaseMultiplier = 0.9f;

char LURD_Keys[5] = "uipo";
const unsigned int MAX_INPUT = 50;
void setup(void) {
  Serial.begin(9600);
  
  Keyboard.begin();
  calibrate();
}

void initDataForCaliration() {
  for (int i = 0; i < 50; i++)
  {
    updateAnalogValues();
    delay(20);
  }
  LURD_calibration_pressures = LURD_Values;
}

void calibrate() {
  initDataForCaliration();
  setCalibrationThresholds(startupCalibrationThreshold);
}

void setCalibrationThresholds(int threshold)
{
  for (int i = 0; i < 4; i++)
  {
    LURD_pressures[i] = LURD_Values[i] + threshold;
  }
}

// process data after null terminator is received
void process_data (char * data)
{

  //do some string parsing  
  data[4]=0;
  char index = data[0]-48;

  //pad side query (9) or set pressures (0-3)???
  if (index == 9) {
	  //I'm left!
    Serial.println('0');
  } else {
    if (index == 19) {
      //Given command char was 'C'alibrate (ASCII value 67 - 48 -> index 19). Wow, such amazing code.

      int threshold = atoi((const char *)&(data[1]));
      setCalibrationThresholds(threshold);
    }
    else if (index < 5)
    {
      LURD_pressures[index] = atoi((const char *)&(data[1]));
    }

    Serial.print("L pressure: ,");
    if (LURD_pressures[0] < 100) Serial.print("0");
    if (LURD_pressures[0] < 10) Serial.print("0");
    Serial.print(LURD_pressures[0]);
    Serial.println(",");

    Serial.print("U pressure: ,");
    if (LURD_pressures[1] < 100) Serial.print("0");
    if (LURD_pressures[1] < 10) Serial.print("0");
    Serial.print(LURD_pressures[1]);
    Serial.println(",");

    Serial.print("R pressure: ,");
    if (LURD_pressures[2] < 100) Serial.print("0");
    if (LURD_pressures[2] < 10) Serial.print("0");
    Serial.print(LURD_pressures[2]);
    Serial.println(",");

    Serial.print("D pressure: ,");
    if (LURD_pressures[3] < 100) Serial.print("0");
    if (LURD_pressures[3] < 10) Serial.print("0");
    Serial.print(LURD_pressures[3]);
    Serial.println(",");
    Serial.println("");
  }
}
  
void processIncomingByte (const byte inByte)
{
  static char input_line [MAX_INPUT];
  static unsigned int input_pos = 0;

  switch (inByte)
  {

    case '\n':   // end of text
      input_line [input_pos] = 0;  //null terminator
      
      process_data (input_line);
      
      input_pos = 0;  
      break;

    case '\r':   // discard carriage return
      break;

    default:
      if (input_pos < (MAX_INPUT - 1))
        input_line [input_pos++] = inByte;
      break;

  } 
} 

void updateAnalogValues() {
	//Serial.println(analogRead(1));
  for (int i = 0; i < 4; i++)
  {
    LURD_Values[i] = (LURD_Values[i] * oldValueWeight + analogRead(LURD_pins[i])) / (oldValueWeight + 1);
    int borderValue = (LURD_pressures[i] + BASE_PRESSURE);
    if (LURD_Values[i] > borderValue)
    {
      if (LURD_State[i] == 0)
      {
        Keyboard.press(LURD_Keys[i]);
        LURD_State[i] = 1;
      }
    }
    else
    {
      if (LURD_State[i] == 1 && LURD_Values[i] < borderValue * releaseMultiplier)
      {
        Keyboard.release(LURD_Keys[i]);
        LURD_State[i] = 0;
      }
    }
  }
}

int counter = 0;
void loop(void) {

  counter = (counter+1)%10;
  if (counter == 0 && Serial.available() > 0)
  {
    processIncomingByte(Serial.read());
  }

  updateAnalogValues();

 /* if(counter % 10 == 0) {
    int fsrReading = analogRead(0);
    Serial.print(fsrReading);
    Serial.print(";");
    fsrReading = analogRead(1);
    Serial.print(fsrReading);
    Serial.print(";");
  
    fsrReading = analogRead(2);
    Serial.print(fsrReading);
    Serial.print(";");
  
    fsrReading = analogRead(3);
    Serial.print(fsrReading);
    Serial.println(";");
  }*/
}
