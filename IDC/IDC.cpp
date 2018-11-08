#include <Arduino.h>
#include "IDC.h"
#include <Servo.h>
#include "HardwareSerial.h"
#include "print.h"

// Takes pins from left to right
IDC::IDC() {
}

void IDC::setQTI(int QTI1, int QTI2, int QTI3, int QTI4) {
     
    int qtiPin1 = QTI1;
    int qtiPin2 = QTI2;
    int qtiPin3 = QTI3;
    int qtiPin4 = QTI4; 
    
}

// Takes servos from left to right 
void IDC::setServos(int left, int right) {

  Servo servoRight;
  Servo servoLeft;
  servoRight.attach(right);
  servoLeft.attach(left); 
  
}

// Sets axes pins x and y
void IDC::setAxis(int x, int y) {

  int xIn = x;
  int yIn = y;
  
}

// Takes task, sets score, and begins serial monitor
void IDC::initialize(int _task) {

  int score = 1;
  int task = _task;  
  int counter = 0;

  Serial.begin(9600);
  Serial2.begin(9600);

}


void IDC::lineFollow() {

  while (1) {

    long thresh = 175;
    long left = rcTime(qtiPin1);
    long middleleft = rcTime(qtiPin2);
    long middleright = rcTime(qtiPin3);
    long right = rcTime(qtiPin4);
  
    if (left < thresh && middleleft < thresh && middleright < thresh && right < thresh) {
      
      forward();
      
    }
    else if (left > thresh && middleleft > thresh && middleright > thresh && right > thresh ){
      brake();
      counter += 1;
      delay(500);
      if (counter == 5) {
        break;
      }    
      sense();

    }
    else if (left > thresh && middleleft > thresh) {
      leftTurn();
    }
    else if (middleright > thresh && right > thresh){
      rightTurn();
    }
    else if(middleleft > thresh && middleright < thresh){
      leftTurn();
    }
    else if(middleleft < thresh && middleright > thresh) {
      rightTurn();
    }
  }
}

long IDC::rcTime(int pin) {
  
  pinMode(pin,OUTPUT);
  digitalWrite(pin,HIGH);
  delayMicroseconds(230);
  pinMode(pin,INPUT);
  digitalWrite(pin,LOW);
  long time = micros();
  while(digitalRead(pin));
  time = micros()-time;
  return time;
  
}

void IDC::forward() {
  servoLeft.writeMicroseconds(1550);         // Left wheel counterclockwise
  servoRight.writeMicroseconds(1450);        // Right wheel clockwise
}

void IDC::backward() { 
  servoLeft.writeMicroseconds(1450);         // Left wheel clockwise
  servoRight.writeMicroseconds(1550);        // Right wheel counterclockwise
}

void IDC::leftTurn() {
  servoLeft.writeMicroseconds(1500);         // Left wheel counterclockwise
  servoRight.writeMicroseconds(1450);        // Right wheel counterclockwise
}

void IDC::rightTurn() {
  servoLeft.writeMicroseconds(1550);         // Left wheel counterclockwise
  servoRight.writeMicroseconds(1500);        // Right wheel counterclockwise
}

void IDC::brake() {
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
}

void IDC::sense() {
  if (task == 1) {
  }
  else if (task == 2) {
  }
  else if (task == 3) {
  }
  else if (task == 4) {
  }
  else if (task == 5) {
    forward();
    delay(500);
    brake();       
    landingSite();      
    delay(500);
    forward();
    delay(500); 
  }
}

// Measures what the robot is on
void IDC::landingSite(){
  //variables read pulse width

  int pulseX = pulseIn(xIn, HIGH); //reads x Pulse (Sideways Tilt)
  int pulseY = pulseIn(yIn, HIGH); //reads y Pulse (Forward & Backward Tilt)
 
  // Prints 'Flat' if in flat terrain range. 
  if((4920 < pulseX && pulseX< 5000)&& (4960 < pulseY && pulseY< 5000) ){ 
    Serial.println("Flat");
  } 
  
  // Prints 'Rocky' if in Rocky terrain range. 
  else if ((4880 <pulseX ) && (5010<pulseY) || ((4870 <pulseX) && (pulseY<4850)))  {
    Serial.println("Rocky");
    score += 1;
  }

  // Prints 'Hilly' if in Hilly terrain range. 
  else if(( pulseX < 4860) && (4960<pulseY && pulseY < 5030)){
    Serial.println("Hilly");

    if (score == 1) {
      score = 2;
    }
    else {
      score += 2;
    } 
  }
}

void IDC::Transmit() {

  Serial.println(score);
  
}

void IDC::Recieve() {
  if (Serial2.available()) {
     score += Serial2.read();
  }
}

