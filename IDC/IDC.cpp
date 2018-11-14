#include <Arduino.h>
#include "IDC.h"
#include <Servo.h>
#include "HardwareSerial.h"
#include "print.h"

// Takes pins from left to right
IDC::IDC() {
}

// Sets axes pins x and y
void IDC::setAxis(int x, int y) {

  int xIn = x;
  int yIn = y;
  
}

// Takes task, sets score, and begins serial monitor


int IDC::lineFollow() {

  int score = 1; 

  Servo servoRight;
  Servo servoLeft;
  int counter = 0;

  Serial.begin(9600);
  Serial2.begin(9600);

  while (1) {
        
    long thresh = 175;
    long left = rcTime(49);
    long middleleft = rcTime(51);
    long middleright = rcTime(53);
    long right = rcTime(52);
  
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

      if (counter == 3) {
        rightTurn();
        delay(100);
      }

      if (counter != 2) {
        int add = sense(score);
        Serial.println(add);
        score += add;
        Serial.println(score);
      }

    forward();
    delay(500);

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
  return score;
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
  servoRight.attach(11);
  servoLeft.attach(12); 
  servoLeft.writeMicroseconds(1550);         // Left wheel counterclockwise
  servoRight.writeMicroseconds(1450);        // Right wheel clockwise
}

void IDC::backward() { 
  servoRight.attach(11);
  servoLeft.attach(12);
  servoLeft.writeMicroseconds(1450);         // Left wheel clockwise
  servoRight.writeMicroseconds(1550);        // Right wheel counterclockwise
}

void IDC::leftTurn() {
  servoRight.attach(11);
  servoLeft.attach(12);
  servoLeft.writeMicroseconds(1500);         // Left wheel counterclockwise
  servoRight.writeMicroseconds(1450);        // Right wheel counterclockwise
}

void IDC::rightTurn() {
  servoRight.attach(11);
  servoLeft.attach(12);
  servoLeft.writeMicroseconds(1550);         // Left wheel counterclockwise
  servoRight.writeMicroseconds(1500);        // Right wheel counterclockwise
}

void IDC::brake() {
  servoRight.attach(11);
  servoLeft.attach(12);
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
}

int IDC::sense(int score) {
    forward();
    delay(1500);
    brake();    
    delay(500);    
    int count = landingSite(score);  
    return count;
}

// Measures what the robot is on
int IDC::landingSite(int score){
  //variables read pulse width
  int pulseX = pulseIn(2, HIGH); //reads x Pulse (Sideways Tilt)
  int pulseY = pulseIn(3, HIGH); //reads y Pulse (Forward & Backward Tilt)
  int count = 0;
  Serial.print(pulseX);
  Serial.print("  ");
  Serial.println(pulseY);
  // Prints 'Flat' if in flat terrain range. 
  if((4875 < pulseX && pulseX < 5100)&& (4800 < pulseY && pulseY < 4950) ){ 
    Serial.println("Flat");
  } 
  
  // Prints 'Rocky' if in Rocky terrain range. 
  else if ((4780 <pulseX ) && (4950<pulseY))  {
    Serial.println("Rocky");
    count = 1;
    leftTurn();
    delay(75);
    forward();
    delay(75);
  }

  else if ((4780 <pulseX) && (pulseY<4850)) {
    Serial.println("Rocky");
    count = 1;
    rightTurn();
    delay(75);
    forward();
    delay(75);
  }

  // Prints 'Hilly' if in Hilly terrain range. 
  else if(( pulseX < 4875) && (4870<pulseY && pulseY < 5030)){
    Serial.println("Hilly");
    Serial.print("Score is: ");
    Serial.println(score);
    if (score == 1) {
      count = 2;
    }
    else {
      count = 1;
    }
    forward();
    delay(500);
  }
    Serial.print("Count is: ");
    Serial.println(count);
    return count;
}

void IDC::Transmit(int quality) {

  Serial2.begin(9600);
  Serial2.println(quality);
  Serial.print("Quality of Terrain: ");
  Serial.println(quality);
  
}

void IDC::Recieve() {
  if (Serial2.available()) {
     score += Serial2.read();
  }
}

