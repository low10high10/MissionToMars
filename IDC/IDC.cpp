#include <Arduino.h>
#include "IDC.h"
#include <Servo.h>
#include "HardwareSerial.h"
#include "print.h"

// Takes pins from left to right
IDC::IDC() {
}

// Takes task, sets score, and begins serial monitor


int IDC::lineFollow() {

  int score = 1; 

  Servo servoRight;
  Servo servoLeft;
  int counter = 0;

  Serial.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600); // LCD Serial
  Serial3.write(12); //Clears LCD Serial
  Serial3.write(17); //Turns backlight on

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
      if (counter == 5) { // after five hashes youre done!
        break;
      }    

      if (counter == 3) { // course correct for the third hash
        rightTurn();
        delay(100);
      }

      if (counter != 2) { // dont check the second hash... ever
        int add = sense(score);
        Serial.println(add);
        score += add;
        Serial.println(score);
      }

    forward();
    delay(500);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
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
  servoRight.writeMicroseconds(1447);        // Right wheel clockwise
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
    servoRight.attach(11);
    servoLeft.attach(12); 
    servoLeft.writeMicroseconds(1535);         // Left wheel counterclockwise
    servoRight.writeMicroseconds(1463);
    delay(2000);
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
    Serial3.print("F "); //prints onto LCD Display
    pinMode(7, OUTPUT);
    digitalWrite(7, HIGH);
    Serial.print(pulseX);
    Serial.println(pulseY);
  } 
  // Prints 'Rocky' if in Rocky terrain range. 
  else if ((4780 <pulseX ) && (4950<pulseY))  {
    Serial.println("Rocky"); // checks if right side is rocky
    Serial3.print("R "); //prints onto LCD Display
    count = 1;
    leftTurn();
    delay(75);
    forward();
    delay(75);
    Serial.print(pulseX);
    Serial.println(pulseY);
  }
  else if ((4780 <pulseX) && (pulseY<4800)) {
    Serial.println("Rocky"); // checks ig left side is rocky
    Serial3.print("R "); //prints onto LCD Display
    count = 1;
    rightTurn();
    delay(75);
    forward();
    delay(75);
    Serial.print(pulseX);
    Serial.println(pulseY);
  }
  // Prints 'Hilly' if in Hilly terrain range. 
  else { //if(( pulseX < 4895) && (4870<pulseY && pulseY < 5030))
    Serial.println("Hilly"); // is it nothing else? its hilly!!
    Serial3.print("H "); //prints onto LCD Display
    Serial.print("Score is: ");
    Serial.println(score);
    pinMode(6, OUTPUT);
    digitalWrite(6, HIGH);
    Serial.print(pulseX);
    Serial.println(pulseY);
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
    return count; // what will be added to score
}

void IDC::Transmit(int quality) {

  Serial2.begin(9600);
  Serial2.println(quality);
  Serial.print("Quality of Terrain: "); // send our info
  Serial.println(quality);
  Serial3.write(13);
  Serial3.println(quality);
  
}

void IDC::Recieve() {
  if (Serial2.available()) {
     score += Serial2.read();
  }
}

