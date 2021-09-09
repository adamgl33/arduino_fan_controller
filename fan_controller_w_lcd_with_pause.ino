#include <LiquidCrystal.h>
#include <EEPROM.h>

// initialize the lcd
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
// LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);

int backLightPin = 10;

// constants
const int minSpeed = 255;
const int maxSpeed = 0;
const unsigned long timeToFade = 6000L;
const int speedIncrement = 20;
// globals
unsigned long lastbuttonPressed = millis();
int currentFan = 0;
bool inPauseState = false;

// main data structure
typedef struct fan
{
  int speed;
  int outPutPin;
};
// fan array - modify number of fans here by adding/removing elements. 
fan fans[] = {
    {255, 3},
    {255, 11}};
int numberOfFans = sizeof(fans) / sizeof(fan);

void setup()
{
  // read saved speeds from eeprom
  readFromEeprom();
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print fan state to the LCD, write pwm.
  afterUpdate();
}

void loop()
{
  int currentSpeed;
  // read button pressed
  int buttonVoltage = analogRead(0);
  //this removes least significant bits to get more stable reads.
  int buttonPressed = buttonVoltage >> 7; 
  switch (buttonPressed)
  {
  case 0:
    // right -> fan++
    currentFan++;
    if (currentFan > numberOfFans - 1)
    {
      currentFan = numberOfFans - 1;
    }
    afterClick();
    break;

  case 2:
    // up -> increase speed
    currentSpeed = fans[currentFan].speed;
    currentSpeed -= speedIncrement;
    if (currentSpeed < 0)
    {
      currentSpeed = 0;
    }
    fans[currentFan].speed = currentSpeed;
    afterUpdate();
    break;

  case 1:
    //down -> decrease
    currentSpeed = fans[currentFan].speed;
    currentSpeed += speedIncrement;
    if (currentSpeed > 255)
    {
      currentSpeed = 255;
    }
    fans[currentFan].speed = currentSpeed;
    afterUpdate();
    break;

  case 3:
    // left -> fan--
    currentFan--;
    if (currentFan < 0)
    {
      currentFan = 0;
    }
    afterClick();
    break;

  case 5:
    // Select - go to pause state
    handlePauseState(); 
    break;

  case 7:
    // nothing
    break;
  }

  checkSleep();
  delay(200);
}

// helper functions
void printFanNumber()
{
  lcd.setCursor(0, 0);
  String fanText = "Fan: ";
  fanText.concat(currentFan + 1);
  fanText.concat("/");
  fanText.concat(numberOfFans);
  lcd.print(fanText);
  lcd.setCursor(15, 0);
  if (inPauseState) {
    lcd.print("P");
  }
}

void printFanSpeed()
{
  lcd.setCursor(0, 1);
  String fanText = "Speed: ";
  fanText.concat(fans[currentFan].speed);
  lcd.print(fanText);
}

void writePwm()
{
  for (int i = 0; i < numberOfFans; i++)
  {
    analogWrite(fans[i].outPutPin, fans[i].speed);
  }
}

int calculateHumanReadableSpeed(int input)
{
  return (input - 255) * -1;
}
void checkSleep()
{
  unsigned long now = millis();
  if (now - lastbuttonPressed > timeToFade)
  {
    analogWrite(backLightPin, 0);
  }
  else
  {
    analogWrite(backLightPin, 255);
  }
}

void afterClick()
{
  lastbuttonPressed = millis();
  lcd.clear();
  printFanNumber();
  printFanSpeed();
}

void afterUpdate()
{
  afterClick();
  writePwm();
  writeToEeprom();
}

void readFromEeprom()
{
  for (int i = 0; i < numberOfFans; i++)
  {
    fans[i].speed = EEPROM.read(i);
  }
}

void writeToEeprom()
{
  EEPROM.write(currentFan, fans[currentFan].speed);
}

void handlePauseState()
{
  if (inPauseState)
  {
    // restore old fan speeds from struct
    inPauseState = false;
    readFromEeprom();
    afterClick();
    writePwm();
  }
  else
  {
    inPauseState = true;
    setAllSpeedsToZero();
    afterClick();
    writePwm();
  }
}

void setAllSpeedsToZero()
{
  for (int i = 0; i < numberOfFans; i++)
  {
    fans[i].speed = 0;
  }
}
