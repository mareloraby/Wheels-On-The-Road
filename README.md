
## Brief Description of the implementation

Implemented a RC with three modules: APA,FI and SS

 - The APA ( Active Parking Assist) involved parking the car between two static objects and preventing it from colliding with other static objects. If a car is about to collide, a warning sound is produced. The tone varies with the distance between the car and the static object.
We added two buttons to the module to control which direction (Rear or Front) the car should park.
	
-  The FI ( Fluid Indicators) feature’s role was to detect if it’s raining and turn on the wind shield’s action which is depicted using a servo motor. Furthermore, the car’s fuel level is read using a water level sensor and the corresponding reading is displayed on a 7segment display.  

-  In the SS (Sound System) feature we implemented a built-in car mp3 player where the user is able to control the sound being played through the speaker via an LCD touch screen on the mega board. The following operations can be achieved:
Next 
Previous 
Play/Pause.

-  We implemented an automatic light on/off circuit using an LDR module and LEDs as car headlights. The LDR sensor is used for detecting the presence of surrounding light so that during the daytime when the sun is bright, the headlight is switched off automatically. During the night time when there is no light, the LDR sends a signal to turn on the headlight.


## The components used in the project

- Arduino Uno Rev3 : used for the APA module ( car parking and obstacle detection)

- Arduino Mega ADK Rev3 : used for the rain sensor, fuel level measure, MP3 player and LCD display. 

- 4WD Complete Mini Plastic Robot Chassis Module HC-SR04 with 4 gear motor: used  to build the car

- H-Bridge: used to connect the gear motors with Arduino Uno Board .



- Active Parking Assistant (APA):

  - Small Buzzer 5~6V : initiates a sound when the car is about to collide.
  - Ultrasonic Sensor & Distance Measurement Module HC-SR04 (x2) : measures the distance between the car and the static object. 


- Fluid Indicator (FI): 

  - Raindrop Water Sensor Module : detects rain.
  - YL-83 - Control Board : connects the raindrop sensor to the Arduino Mega board 
  - Water Level-Detection Sensor: measures the car’s fuel level
  - Micro Servo SG90: acts as a windshield.
  - 7 Segment Display 2 Digit Black: display’s the car’s fuel level.

- Sound System (SS):

  - Speaker:generates sound from the mp3 player.
  - MP3 mini player for Arduino : stores sound playlists.
  - TFT-LCD-Display-2.4-2.8-Touch-Shield : controls the mp3 player. 
  - SD card: stores sound on the mp3 player.

- Automatic headlights on/off circuit:
  - LDR module
  - LEDs (x2)
 
 
## The full project circuit

	Circuit designed using fritzing.com
 
<img src="https://github.com/mareloraby/Wheels-On-The-Road/blob/main/MegaCode/mega_bb.jpg" />

<img src="https://github.com/mareloraby/Wheels-On-The-Road/blob/main/parking/Uno_bb.jpg" />


## The names of the libraries used and their functions
- Libraries for LCD display:
  SPFD5408_Adafruit_GFX.h
  SPFD5408_Adafruit_TFTLCD.h
  SPFD5408_TouchScreen.h

- SoftwareSerial.h:  enables serial communication with a digital pin other than the serial port. 

- DFRobotDFPlayerMini.h : library for DFPlayer Mini.

- Arduino_FreeRTOS.h : used for creating FreeRTOS tasks and scheduling them. 
Servo.h: This library allows an Arduino board to control RC (hobby) servo motors. 


## Prioritizing and dividing tasks using freeRTOS.

We divided the features between 2 microcontrollers; 3 on the Arduino Mega board (Rain detection and windshields, Fuel level, and sound system module  ) and 2 on the Arduino Uno board (DC motors control, obstacle detection and warning).

On the microcontroller on the uno board, we created 3 tasks: 1 continuous task for controlling the dc motors, and 2 periodic tasks for ultrasonic sensor readings, all having the same priority.
```

  xTaskCreate(
    taskOne,          /* Task function. */
    "Parking",        /* String with name of task. */
    128,              /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL);            /* Task handle. */
  xTaskCreate(
    taskTwo,          /* Task function. */
    "Ultrasonics",    /* String with name of task. */
    128,              /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL);            /* Task handle. */
  xTaskCreate(
    taskThree,        /* Task function. */
    "Ultrasonics2",   /* String with name of task. */
    128,              /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL);            /* Task handle. */
 ```
 ```
void taskOne() {
  while (1) {
    if (digitalRead(button2) == LOW) {
      Serial.println(" 2 pressed");
      buttonclicked2 = true;
      parkCar();
      step = 1;
    }
    if (digitalRead(button1) == LOW) {
      Serial.println(" 1 pressed");
      buttonclicked1 = true;
      parkCarOpposite();
      step = 1;
    }
  }
}
void taskTwo() {
  while (1) {
    ultrasonic1();
    delay(800);
  }
}
void taskThree() {
  while (1) {
    ultrasonic2();
    delay(800);
  }
}

```
On the microcontroller on the mega board, we created 3 tasks as well: 1 continuous task for controlling the lcd and mp3 player, and 2 periodic tasks: one for rain sensor reading and controlling the servo motor, and another for water level sensor reading and outputting to the 7 segment display, all having the same priority.
```
  xTaskCreate(
    taskOne,          /* Task function. */
    "MP3andLCD",      /* String with name of task. */
    128,              /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL);            /* Task handle. */
  xTaskCreate(
    taskTwo,          /* Task function. */
    "FuelLevel",      /* String with name of task. */
    128,              /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL);            /* Task handle. */
  xTaskCreate(
    taskThree,        /* Task function. */
    "RainSensor",     /* String with name of task. */
    128,              /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL);            /* Task handle. */
```
```
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

```

## The problems or limitations faced during the implementation of the project

- Not enough space was available on the car to integrate all the features, so we added another upper layer to the car. 
- Wires were loose and we resorted to welding, however that too went loose a few times. 
- Dealt with dead batteries at first so we bought a rechargeable battery instead. 

## Team members:
- Hadeer ElHussein
- Maryam ElOraby
- Rawan Reda
- Rowan Amgad



☺

