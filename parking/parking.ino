#include <Arduino_FreeRTOS.h>
const int trigger_pin = A2;
const int echo_pin = A3;
const int trigger_pin2 = A5;
const int echo_pin2 = A4;
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


int tn = 4000;
int duration;
int distance;

int duration2;
int distance2;

bool moveforward = false;
bool movebackward = false;
bool stopCar = false;
bool ultrasonic1range = false;
bool ultrasonic2range = false;



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

  Serial.begin(9600);
}

void methodfrominternet(){
  
      if (distance2 > -1 && distance2 < 10) {
      turnoffMotors();
      delay(1000);
    BothBack();
      delay(300);
      turnoffMotors();
      delay(1000);
  
//      if (random(0, 2) == 0) {
//        turnAForwardandBBackward();
//      } else {
        turnBForwardandABackward();
//      }
  
      delay(500);
      turnoffMotors();
      delay(700);
    } else {
      BothForward();
    }
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



    if (distance <= 7 && distance> 0)
    {
      tone(buzzer, tn);
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
  //  noTone(buzzer);
  Serial.print("u2 ");
  Serial.println(distance2);


    if (distance2 <= 7 && distance2> 0)
    {
      tone(buzzer, tn);
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
//if (count < 10) {
      turnBForwardandABackward();
      delay(240);
      stopNow();
      step = 2;
      delay(1000);

  }
  if (step == 2) {
    BothBack();
    delay(575);

   // if (distance2 <= 60 && distance2 > 0) {
      stopNow();
      delay(1000);
      step = 3;
   // }
  }
  //}
  
  if (step == 3) {
//    if (count1 < 10) {
      turnAForwardandBBackward();
      delay(210);
      stopNow();
      step=4;
  }
  if (step == 4) {
    BothForward();
    delay(200);
    stopNow();

    step=5;
  }



}




void loop() {
    ultrasonic1();
    ultrasonic2();
    parkCar();



}
