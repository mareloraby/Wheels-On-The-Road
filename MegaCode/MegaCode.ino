#include <Arduino_FreeRTOS.h>
#include <SPFD5408_Adafruit_GFX.h>
#include <SPFD5408_Adafruit_TFTLCD.h>
#include <SPFD5408_TouchScreen.h>

#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

static const uint8_t PIN_MP3_TX = 53; // Connects to module's RX
static const uint8_t PIN_MP3_RX = 52; // Connects to module's TX
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);

// Create the Player object
DFRobotDFPlayerMini player;


void mp3Setup() {

  // Init serial port for DFPlayer Mini
  softwareSerial.begin(9600);

  // Start communication with DFPlayer Mini
  if (player.begin(softwareSerial)) {
    Serial.println("OK");

    // Set volume to maximum (0 to 30).
    player.volume(20);
    // Play the first MP3 file on the SD card
    //player.play(1);
  } else {
    Serial.println("Connecting to DFPlayer Mini failed!");
  }


}

# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
# define End_Byte 0xEF
# define Acknowledge 0x00


#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif


#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

//Don't forget if your touch function doesn't work check the values above it may be (A1 A2 7 6) resp

// Calibrate values you may want to run the calibration code first and set those points
#define TS_MINX 125
#define TS_MINY 85
#define TS_MAXX 965
#define TS_MAXY 905

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

#define MINPRESSURE 10
#define MAXPRESSURE 1000

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void tftSetup() {


  tft.reset();                         //Shield launching
  tft.begin(0x9341);
  tft.setRotation(4);                  //Rotation 0-3
  tft.fillScreen(BLACK);               //Black background
  tft.fillRect(10, 40, 80, 80, RED);   //Draws 2 rectangles Green and Red
  tft.fillCircle(150, 80, 40, GREEN);
  tft.fillRect(210, 40, 80, 80, RED);


  tft.setCursor(25, 70);
  tft.setTextColor(WHITE);  tft.setTextSize(2);
  tft.print("Prev");

  tft.setCursor(125, 60);
  tft.setTextColor(WHITE);  tft.setTextSize(2);
  tft.print("Play/");
  tft.setCursor(125, 80);
  tft.print("Pause");

  tft.setCursor(230, 70);
  tft.setTextColor(WHITE);  tft.setTextSize(2);
  tft.print("Next");
}


/* Change these values based on your calibration values */
int lowerThreshold = 420;
int upperThreshold = 520;

#include <SevSeg.h>
#include <Arduino_FreeRTOS.h>
#include "SevSeg.h"
SevSeg sevseg;

// Sensor pins
#define sensorPower 49
#define sensorPin A8

// Value for storing water level
int val = 0;

void sevenSegmentSetup() {
  byte numDigits = 1;
  byte digitPins[] = {};
  byte segmentPins[] = {22, 23, 24, 25, 26, 27, 28, 29};
  bool resistorsOnSegments = true;

  byte hardwareConfig = COMMON_CATHODE;
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);
}

//rainsensor and servo
#include <Servo.h>
Servo servo;
int angle = 42;
int rainPin = A9;
int thresholdValue = 800;

void servoSetup() {

  //servo and rain
  pinMode(rainPin, INPUT);
  servo.attach(13);
  servo.write(angle);
}

void setup(void) {


  // Init USB serial port for debugging
  Serial.begin(9600);


  pinMode(sensorPower, OUTPUT);
  digitalWrite(sensorPower, LOW);

  mp3Setup();
  tftSetup();
  sevenSegmentSetup(); //level sensor and seven segments
  servoSetup(); //servo and rain

  xTaskCreate(
    taskOne,          /* Task function. */
    "MP3andLCD",        /* String with name of task. */
    128,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL);            /* Task handle. */

  xTaskCreate(
    taskTwo,          /* Task function. */
    "FuelLevel",        /* String with name of task. */
    128,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL);            /* Task handle. */

  xTaskCreate(
    taskThree,          /* Task function. */
    "RainSensor",        /* String with name of task. */
    128,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL);            /* Task handle. */

}

void taskOne( void * pvParameter )
{
  while (1) {
    //Serial.println("Task 1");
    mp3andlcd();
  }
}

void taskTwo( void * parameter)
{
  while (1) {
    // Serial.println("Task 2");
    levelsensor();
    delay(1000);
  }
}

void taskThree( void * parameter)
{
  while (1) {
    // Serial.println("Task 3");
    rainSensor();
    delay(1000);
  }

}



int btnclick = 0; // 1 play  2 pause 3 next 4 prev
bool nextclick = false;

bool isPlaying = false;

void mp3andlcd() {
  TSPoint p = ts.getPoint();       //checking if the user touched the screen

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) { //p.z means the pressure value so if the touch wants to be detected
    // it pressure should be in this range (it's enough)
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());    //x and y positions of the touch so the program know the postion where the user has pressed
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());;


    if (p.y > 20 && p.y < 100 && p.x > 40 && p.x < 110) { //This concerns the red rectangle we draw before if the "y" position is between (20-100) and "x" position is between (40-120)
      //tft.fillRect(0, 180, 300, 300, BLACK);          //The program will show red pressed, and that's how we create a touch button, the positions are from the rectangle values with a little addition
      //tft.setCursor(20, 180);
      //tft.setTextColor(WHITE);  tft.setTextSize(3);
      // tft.print("Prev");

      if (isPlaying == true) {
        player.previous();
        Serial.println();
        Serial.println(player.readCurrentFileNumber());
      }
    }

    else if (p.x > 130 && p.x < 205 && p.y > 40 && p.y < 120) {

      if (isPlaying == true ) {
        player.pause();
        isPlaying = false;
      }
      else {
        if (isPlaying == false ) {
          player.start();
          isPlaying = true;
        }
      }


    }
    else if (p.x > 220 &&  p.x < 300 && p.y > 40 && p.y < 120) {

      // tft.fillRect(0, 180, 300, 300, BLACK);          //We fill a little area so it's a partial cleaning of the screen
      // tft.setCursor(0, 180);
      //  tft.setTextColor(WHITE);  tft.setTextSize(3);
      //  tft.print("Next");
      if (isPlaying == true) {
        player.next();
        Serial.println();
        Serial.println(player.readCurrentFileNumber());
      }

    }
  }
}


void levelsensor() {
  int level = readSensor();
  if (level == 0) {
  }
  else if (level > 0 && level <= lowerThreshold) {
    // Serial.println("Water Level: Low");
    sevseg.setNumber(0);
    sevseg.refreshDisplay();
  }
  else if (level > lowerThreshold && level <= upperThreshold) {
    // Serial.println("Water Level: Medium");

    sevseg.setNumber(1);
    sevseg.refreshDisplay();
  }
  else if (level > upperThreshold) {
    //   Serial.println("Water Level: High");
    sevseg.setNumber(2);
    sevseg.refreshDisplay();
  }
  delay(1000);
}

//This is a function used to get the reading
int readSensor() {
  digitalWrite(sensorPower, HIGH);
  delay(10);
  val = analogRead(sensorPin);
  digitalWrite(sensorPower, LOW);
  return val;
}

void moveServo () {
  // scan from 0 to 180 degrees
  for (angle = 10; angle < 180; angle += 2)
  {
    servo.write(angle);
    delay(15);
  }
  // now scan back from 180 to 0 degrees
  for (angle = 180; angle > 10; angle -= 2)
  {
    servo.write(angle);
    delay(15);
  }
}

void rainSensor() {

  int sensorValue = analogRead(rainPin);
  Serial.print(sensorValue);
  if (sensorValue < thresholdValue) {
    // Serial.println(" - It's wet");
    moveServo();
  }
  else {
    //  Serial.println(" - It's dry");
  }
  delay(500);
}

void loop()
{


}
