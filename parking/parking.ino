#include <stdio.h>
#include <stdlib.h>
#include <Arduino_FreeRTOS.h>

//ultrasonic 1
const int trigger_pin = A2;
const int echo_pin = A3;
int duration;
int distance;

//ultrasonic 2
const int trigger_pin2 = A5;
const int echo_pin2 = A4;
int duration2;
int distance2;

int tn = 4000;

//buzzer pin
const int buzzer = 6;

// Motor A
int enA = 3;
int in1 = 2;
int in2 = 4;

// Motor B
int enB = 5;
int in3 = 7;
int in4 = 8;

int wheelspeed = 100;

bool moveforward = false;
bool movebackward = false;
bool stopCar = false;
bool ultrasonic1range = false;
bool ultrasonic2range = false;

//buttons to choose which way to park
const int button1 = 9;
const int button2 = 10;

bool buttonclicked1 = false;
bool buttonclicked2 = false;

void setup() {


  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(trigger_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  pinMode(trigger_pin2, OUTPUT);
  pinMode(echo_pin2, INPUT);


  pinMode(button1, INPUT); digitalWrite(button1, HIGH);
  pinMode(button2, INPUT); digitalWrite(button2, HIGH);



  xTaskCreate(
    taskOne,          /* Task function. */
    "Parking",        /* String with name of task. */
    128,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL);            /* Task handle. */

  xTaskCreate(
    taskTwo,          /* Task function. */
    "Ultrasonics",        /* String with name of task. */
    128,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL);            /* Task handle. */


  xTaskCreate(
    taskThree,          /* Task function. */
    "Ultrasonics2",        /* String with name of task. */
    128,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL);            /* Task handle. */

  Serial.begin(9600);
}


void turnBForwardandABackward()

{

  // Turn on motor A
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  // Set speed to 200 out of possible range 0~255
  analogWrite(enA, wheelspeed);


  // Turn on motor B
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  // Set speed to 200 out of possible range 0~255
  analogWrite(enB, wheelspeed);


}

void turnAForwardandBBackward()

{

  // Turn on motor A
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  // Set speed to 200 out of possible range 0~255
  analogWrite(enA, wheelspeed);

  // Turn on motor B
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  // Set speed to 200 out of possible range 0~255
  analogWrite(enB, wheelspeed);


}

void turnoffMotors() {
  analogWrite(enA, 0);
  analogWrite(enB, 0);

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void BothForward()
{
  // Turn on motor A
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  // Set speed to 200 out of possible range 0~255
  analogWrite(enA, wheelspeed);


  // Turn on motor B
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  // Set speed to 200 out of possible range 0~255
  analogWrite(enB, wheelspeed);


}

void BothBack()

{

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  // Set speed to 200 out of possible range 0~255
  analogWrite(enA, wheelspeed);


  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  // Set speed to 200 out of possible range 0~255
  analogWrite(enB, wheelspeed);


}


void ultrasonic1() {

  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigger_pin, LOW);
  duration = pulseIn(echo_pin, HIGH);
  distance = (duration / 2) / 28.5;
  noTone(buzzer);
  Serial.print("u1 ");
  Serial.println(distance);

  if (distance <= 7 && distance > 0)
  {
    tone(buzzer, tn);
    if (buttonclicked2 == true) stopNow();
  }

  else if (distance <= 13)
  {
    tone(buzzer, tn);
    delay(50);
    noTone(buzzer);
    delay(250);
  }

  else if (distance <= 20)
  {
    tone(buzzer, tn);
    delay(50);
    noTone(buzzer);
    delay(500);
  }
  else
  {
    noTone(buzzer);
  }
}


void ultrasonic2() {
  digitalWrite(trigger_pin2, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigger_pin2, LOW);
  duration2 = pulseIn(echo_pin2, HIGH);
  distance2 = (duration2 / 2) / 28.5;
  //noTone(buzzer);
  Serial.print("u2 ");
  Serial.println(distance2);


  if (distance2 <= 7 && distance2 > 0)
  {
    tone(buzzer, tn);
    if (buttonclicked1 == true) stopNow();
  }

  else if (distance2 <= 13)
  {
    tone(buzzer, tn);
    delay(50);
    noTone(buzzer);
    delay(250);
  }

  else if (distance2 <= 20)
  {
    tone(buzzer, tn);
    delay(50);
    noTone(buzzer);
    delay(500);
  }
  else
  {
    noTone(buzzer);
  }

}
int step = 1;
int count = 0;
int count1 = 0;
void stopNow() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void parkCar() {
  /* the car turns right first , and then keeps moving backwards until it gets close to a wall,
    then turns left until and moves forward until it gets close to a wall. */
  if (step == 1) {
    turnBForwardandABackward();
    delay(390);
    stopNow();
    step = 2;
    delay(1000);

  }
  if (step == 2) {
    BothBack();
    delay(530);
    stopNow();
    delay(1000);
    step = 3;
  }

  if (step == 3) {
    turnAForwardandBBackward();
    delay(380);
    stopNow();
    step = 4;
  }
  if (step == 4) {
    BothForward();
    delay(130);
    stopNow();

    step = 5;
  }

  if (step == 5)
  {

    turnBForwardandABackward();
    delay(220);
    stopNow();
    step = 6;
  }

}


void parkCarOpposite() {
  /* the car turns right first , and then keeps moving backwards until it gets close to a wall,
    then turns left until and moves forward until it gets close to a wall. */
  if (step == 1) {
    turnAForwardandBBackward();
    delay(300);
    stopNow();
    step = 2;
    delay(1000);

  }
  if (step == 2) {
    BothForward();
    delay(560);
    stopNow();
    delay(1000);
    step = 3;
  }

  if (step == 3) {
    turnBForwardandABackward();
    delay(395);
    stopNow();
    step = 4;
  }
  if (step == 4) {
    BothBack();
    delay(130);
    stopNow();

    step = 5;
  }

  if (step == 5)
  {

    turnAForwardandBBackward();
    delay(220);
    stopNow();
    step = 6;
  }


}



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


void loop() {

}
