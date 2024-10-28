#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <HX711.h>
#include <EEPROM.h>

#define DOUT 13
#define CLK 12
HX711 scale;
float weight = constrain(weight, 0.0, 5000.0);
String data = "";
String tempData = "";
int count = 0;
Servo Servo1;
Servo Servo2;

int angle = 90;

const byte ROWS = 4;
const byte COLS = 4;
LiquidCrystal_I2C lcd(0x27, 16, 2);
char hexaKeys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {0, 14, 2, 3};
byte colPins[COLS] = {4, 5, 6, 7};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

#define TRIG_PIN 10
#define ECHO_PIN 11
long duration;
float distance;
char customKey;
bool datataken =0; bool objectsorted=0;
const int EEPROM_ADDRESS = 0;     
const int MAX_DATA_LENGTH = 20;   
const int PRINT_CONFIRM_DELAY = 5000; 
const int AUTO_CLEAR_DELAY = 7000; 


//Save user input to EEPROM
void saveDataToEEPROM()
{ 
    for (int i = 0; i < data.length(); i++)
    {EEPROM.write(EEPROM_ADDRESS + i, data[i]);}}

void readDataFromEEPROM()
{  //Read user input from EEPROM
    data = "";
    for (int i = 0; i < MAX_DATA_LENGTH; i++)
    {  char charFromEEPROM = EEPROM.read(EEPROM_ADDRESS + i);
        if (charFromEEPROM == '\0')
        { break;
        } data += charFromEEPROM; }}

void calcdist()
{   digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration * 0.034 / 2;}

bool isfull()
{ calcdist();
  return (distance < 5);}

void initialisemean()
{ angle = 90;
  Servo1.write(angle);}

void tiltright()
{   angle += 90;
    Servo1.write(angle);
    delay(2000);
    angle -= 90;
    Servo1.write(angle);}

void tiltleft()
{   angle -= 90;
    Servo1.write(angle);
    delay(2000);
    angle += 90;
    Servo1.write(angle);}

void tilt180()
{   angle = 180;
    Servo2.write(angle);
    delay(2000);
    angle -= 90;
    Servo2.write(angle);}

void displaydat()
{   lcd.clear();
    delay(2000);

    lcd.setCursor(0, 0);
    lcd.print("Current User:");
    lcd.setCursor(0, 1);
    lcd.print(data.substring(1));
    delay(2000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Weight plastic:");
    lcd.setCursor(0, 1);
    lcd.print(weight);
    delay(2000);
    lcd.clear();

    Serial.print("Current User: ");
    Serial.println(data.substring(1));
    Serial.print("Weight plastic: ");
    Serial.println(weight);}

void ifidle()
{}    

void takedata()
{   data = tempData + customKey;
    lcd.print(customKey);}

void takeDataWithConfirmation()
{   lcd.clear();lcd.print("Enter user no");
    lcd.setCursor(0, 1);tempData = "";
    customKey = customKeypad.waitForKey();
    while (customKey != 'D')
    {   if (customKey == '*')
        {   lcd.clear();lcd.print("User no cleared");delay(2000);
            datataken=0; 
            takeDataWithConfirmation(); }
        else
        {   tempData = tempData + customKey;
            lcd.print(customKey);}
        customKey = customKeypad.waitForKey();}

    data=tempData;
    lcd.clear();lcd.print("Current user:");
    lcd.setCursor(0,1);lcd.print(data);delay(1500);lcd.clear();
    lcd.print("Press D: confirm");
    lcd.setCursor(0, 1);lcd.print("Press * : retry"); delay(2000);
    customKey = customKeypad.waitForKey();
    while (customKey != 'D' && customKey != '*')
    { customKey = customKeypad.waitForKey(); }
    if (customKey == 'D')
    {   data = tempData;
        saveDataToEEPROM();
datataken=1; }
    else if (customKey == '*')
    {  lcd.clear();lcd.print("User no cleared");
        delay(500); lcd.clear();
        datataken=0; 
        takeDataWithConfirmation();}    }

void handlenonplastic()
{       lcd.clear(); lcd.print("Not Plastic");delay(2500);
        tiltright();
         lcd.clear(); lcd.print("User name:");
        lcd.setCursor(0, 1);lcd.print(data.substring(1));delay(2000);
        lcd.clear();lcd.print("Transaction :");
        lcd.setCursor(0, 1);lcd.print("No plastic taken");delay(2000);
        lcd.clear();data = ""; 
        unsigned long previousMillis = millis();const long interval = 10000;
   unsigned long currentMillis = millis();
   while(currentMillis - previousMillis <= interval)
   {lcd.clear(); lcd.println("Collect object");
        lcd.setCursor(0, 1);lcd.print("From Behind");delay(2000);
        lcd.clear();currentMillis = millis(); } }

void askPlasticOrNonPlastic()
{ lcd.clear();lcd.print("Press B: plstic");
    lcd.setCursor(0, 1);lcd.print("Press C: !plstic");delay(2000);
    customKey = customKeypad.waitForKey();
    while (customKey != 'B' && customKey != 'C')
    {customKey = customKeypad.waitForKey();}
    if (customKey == 'B')
    {objectsorted=1;
        handlePlasticTaken();}
    else if (customKey == 'C')
    { objectsorted=1;
      handlenonplastic(); }}

void handlePlasticTaken()
{   lcd.clear();lcd.print("Plastic detected");delay(1500);
    tiltleft();
    measure();delay(2000);
    tilt180();
    lcd.clear();lcd.print("Plastic accepted");
    delay(2000);lcd.clear();
    lcd.print("Event outcomes:");lcd.clear();
    lcd.print("Transaction :");
    lcd.setCursor(0, 1);lcd.print("Plastic taken");
   unsigned long previousMillis = millis();const long interval = 10000;
   unsigned long currentMillis = millis();
   while(currentMillis - previousMillis <= interval)
   {lcd.clear();displaydat();currentMillis = millis(); }}

void measure()
{  if (scale.is_ready())
    { float weight1=0.0, weight2=0.0, weight3=0.0, weight4=0.0;
    lcd.clear();lcd.print("Measuring...");
        weight1 = scale.get_units(); weight1 *= 2.381;Serial.println(weight1); delay(2000);
        weight2 = scale.get_units(); weight2 *= 2.381;Serial.println(weight2); delay(2000);
        weight3 = scale.get_units(); weight3 *= 2.381;Serial.println(weight3); delay(2000);
        weight4 = scale.get_units(); weight4 *= 2.381;Serial.println(weight4); delay(2000);
        weight=(weight1+weight2+weight3+weight4)/4.0;
        lcd.clear();
        lcd.print("Average Weight: ");
        lcd.setCursor(0, 1);lcd.print(weight);
        Serial.print("Average Weight: ");
        Serial.println(weight); }}

void setup()
{   Serial.begin(115200);
    Servo1.attach(8);
    Servo2.attach(9);
    initialisemean();
    scale.begin(DOUT, CLK);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    lcd.begin(16, 2);
    lcd.backlight();
   
    readDataFromEEPROM();}

void loop()
{while (isfull())
    {   lcd.clear();lcd.print("Machine is full");
        lcd.setCursor(0, 1);lcd.print(distance);delay(500);lcd.clear();}
  lcd.print("Press A to start");
  customKey=customKeypad.getKey();delay(100);
if(customKey=='A')
{lcd.clear();                                        
while(!datataken)
  {takeDataWithConfirmation();}
  if(datataken)
  { lcd.clear();
    while(!objectsorted)
    {askPlasticOrNonPlastic();}}
    datataken=0;objectsorted=0;}
else{lcd.clear();}lcd.clear();}