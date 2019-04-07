/*For more information see www.ladyada.net/learn/sensors/fsr.html */

//Sides: left 0, right 1
#define SIDE '0'

#define BASE_PRESSURE  0

#define CALIBRATE_LOW_THRESHOLD 150
#define CALIBRATE_MEDIUM_THRESHOLD 350
#define CALIBRATE_HIGH_THRESHOLD 450

#define INITIAL_LEFT_PRESSURE  1000
#define INITIAL_DOWN_PRESSURE  1000
#define INITIAL_RIGHT_PRESSURE 1000
#define INITIAL_UP_PRESSURE    1000

// {1, 0, 2, 3} on FAT right side
// {0, 2, 3, 1} all other pads (FAT, PRO, Tennari)
int LED_pins[4] = {0, 2, 3, 1};
int LURD_pins[4] = {0, 2, 3, 1};
int LURD_values[4] = {0, 0, 0, 0};
int LURD_state[4] = {0, 0, 0, 0};
int LURD_pressures[4] = {INITIAL_LEFT_PRESSURE, INITIAL_UP_PRESSURE, INITIAL_RIGHT_PRESSURE, INITIAL_DOWN_PRESSURE};
int LURD_calibration_pressures[4] = {0, 0, 0, 0};
int startupCalibrationThreshold = CALIBRATE_MEDIUM_THRESHOLD;

//Vibration detection calibration mode: VDCM
bool VDCM_enabled = false;
int VDCM_vibrationMaxConsecutiveReads = 10;
int VDCM_LURD_consecutiveReads[4] = {0, 0, 0, 0};
int VDCM_LURD_maxPressureValues[4] = {0, 0, 0, 0};
int VDCM_vibrationPadding = 20;

int oldValueWeight = 1;
float releaseMultiplier = 0.9f;

char LURD_Keys[4] = {1, 2, 3, 4};
const unsigned int MAX_INPUT = 50;
void setup(void) {
  Serial.begin(9600);
  setupLedOutputs();
  calibrate();
}

void setupLedOutputs()
{
  for (int i = 0; i < 4; i++)
  {
    pinMode(LED_pins[i], OUTPUT);
  }
}

void initDataForCalibration() {
  for (int i = 0; i < 50; i++)
  {
    updateAnalogValues();
    delay(20);
  }
  memcpy(&LURD_calibration_pressures, &LURD_values, sizeof LURD_values) ;
}

void calibrate() {
  initDataForCalibration();
  setCalibrationThresholds(startupCalibrationThreshold);
  //printPressures();
}

void setCalibrationThresholds(int threshold)
{
  for (int i = 0; i < 4; i++)
  {
    LURD_pressures[i] = LURD_values[i] + threshold;
  }
}

// process data after null terminator is received
void process_data (char * data)
{

  //do some string parsing  
  data[4]=0;
  int index = data[0]-48;

  //pad side query (9) or set pressures (0-3)???
  if (index == 9) {
	  //I'm right or left? #Defined at the start of the file
    Serial.println(SIDE);
  } else if (index == 21) {
      //Given command char was 'E'eanble vibration detection calibration mode
      VDCM_enabled = true;
  } else if (index == 20) {
      //Given command char was 'D'isable vibration detection calibration mode
      VDCM_enabled = false;
  }
  else
  {
    if (index == 19) {
      //Given command char was 'C'alibrate (ASCII value 67 - 48 -> index 19). Wow, such amazing code.

      int threshold = atoi((const char *)&(data[1]));
      setCalibrationThresholds(threshold);
    }
    else if (index < 5)
    {
      LURD_pressures[index] = atoi((const char *)&(data[1]));
    }

    printPressures();
  }
}

void printPressures()
{
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
    LURD_values[i] = (LURD_values[i] * oldValueWeight + analogRead(LURD_pins[i])) / (oldValueWeight + 1);
    int borderValue = (LURD_pressures[i] + BASE_PRESSURE);
    if (LURD_values[i] > borderValue)
    {
      if (VDCM_enabled) VDCM_pressed(i);

      if (LURD_state[i] == 0)
      {
        Joystick.button(LURD_Keys[i], 1);
        digitalWrite(LED_pins[i], HIGH);
        LURD_state[i] = 1;
      }
    }
    else
    {
      if (VDCM_enabled) VDCM_pressReleased(i);
      if (LURD_state[i] == 1 && LURD_values[i] < borderValue * releaseMultiplier)
      {
        Joystick.button(LURD_Keys[i], 0);
        digitalWrite(LED_pins[i], LOW);
        LURD_state[i] = 0;
      }
    }
  }
}

void VDCM_pressed(int LURD_index)
{
  if (LURD_values[LURD_index] > VDCM_LURD_maxPressureValues[LURD_index])
  {
    VDCM_LURD_maxPressureValues[LURD_index] = LURD_values[LURD_index];
  }
  VDCM_LURD_consecutiveReads[LURD_index]++;
}

void VDCM_pressReleased(int LURD_index)
{
  if (VDCM_LURD_consecutiveReads[LURD_index] == 0) return;

  //Was the released panel press shorter than what is considered a vibration?
  if (VDCM_LURD_consecutiveReads[LURD_index] < VDCM_vibrationMaxConsecutiveReads)
  {
    //Set the threshold above the vibration max pressure value
    LURD_pressures[LURD_index] = VDCM_LURD_maxPressureValues[LURD_index] + VDCM_vibrationPadding;

    //Debug
    /*
    Serial.println("Vibration detected:");

    Serial.print("  LURD_index: ");
    Serial.println(LURD_index);

    Serial.print("  Consecutive reads before release: ");
    Serial.println(VDCM_LURD_consecutiveReads[LURD_index]);

    Serial.print("  Pressure value set to: ");
    Serial.println(LURD_pressures[LURD_index]);
    */
  }

  VDCM_LURD_maxPressureValues[LURD_index] = 0;
  VDCM_LURD_consecutiveReads[LURD_index] = 0;
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
