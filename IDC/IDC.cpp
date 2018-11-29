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

  score += sense(score);

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
        delay(150);
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
  servoLeft.detach();
  servoRight.detach();
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
  servoLeft.writeMicroseconds(1542.66);         // Left wheel counterclockwise
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
  if((4850 < pulseX && pulseX < 5100)&& (4800 < pulseY && pulseY < 4950) ){ 
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
    //delay(100);
    forward();
    delay(750);
    Serial.print(pulseX);
    Serial.println(pulseY);
  }
  else if ((4780 <pulseX) && (pulseY<4800)) {
    Serial.println("Rocky"); // checks ig left side is rocky
    Serial3.print("R "); //prints onto LCD Display
    count = 1;
    rightTurn();
    //delay(100);
    forward();
    delay(750);
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
    delay(1000);
  }
    Serial.print("Count is: ");
    Serial.println(count);
    return count; // what will be added to score
}

void IDC::Transmit(int quality) {

  
}


void IDC::Recieve(int quality) {

  
  Serial2.begin(9600);
  Serial.print("Quality of Terrain: "); // send our info
  Serial.println(quality);
  Serial3.write(13);
  Serial3.println(quality);

  delay(1000);

  char transmit;
  
 if (quality == 1) {
   transmit = 'u';
 }
 else if (quality == 2) {
   transmit = 'v';
 }
 else if (quality == 3) {
   transmit = 'w';
 }
 else if (quality == 4) {
   transmit = 'x';
 }
 else if (quality == 5) {
   transmit = 'y';
 }
 String group5 = String(quality);
 String group1 = "0";
 String group2 = "0";
 String group3 = "0";
 String group4 = "0";

 while(1) {
  
  if (Serial2.available()) {
     char data = Serial2.read();
     Serial.print("Found:");
     Serial.println(data);
     
     if (data == 'a') {
        group1 = "1";
     }
     else if (data == 'b') {
        group1 = "2";
        Serial.println("Hey, that's pretty good");
        Serial.println(group1);
     }
     else if (data == 'c') {
         group1 = "3";
     }
     else if (data == 'd') {
         group1 = "4";
     }
     else if (data == 'e') {
         group1 = "5";
     }
     else if (data == 'f') {
         group2 = "1";
     }
     else if (data == 'g') {
         group2 = "2";
     }
     else if (data == 'h') {
         group2 = "3";
     }
     else if (data == 'i') {
         group2 = "4";
     }
     else if (data == 'j') {
         group2 = "5";
     }
     else if (data == 'k') {
         group3 = "1";
     }
     else if (data == 'l') {
         group3 = "2";
     }
     else if (data == 'm') {
         group3 = "3";
     }
     else if (data == 'n') {
         group3 = "4";
     }
     else if (data == 'o') {
         group3 = "5";
     }
     else if (data == 'p') {
         group4 = "1";
     }
     else if (data == 'q') {
         group4 = "2";
     }
     else if (data == 'r') {
         group4 = "3";
     }
     else if (data == 's') {
         group4 = "4";
     }
     else if (data == 't') {
         group4 = "5";
     }
     else if (data == 'u') {
         group5 = "1";
     }
     else if (data == 'v') {
         group5 = "2";
     }
     else if (data == 'w') {
         group5 = "3";
     }
     else if (data == 'x') {
         group5 = "4";
     }
     else if (data == 'y') {
         group5 = "5";
     }
  }
  // Gathering the data from the other teams and combining it into one string.
  String Mission = (group1 + " " + group2 + " " + group3 + " " + group4 + " " + group5);
  Serial3.write(12);
  Serial3.print(Mission);
  Serial2.print(transmit);
  delay(3000);
 }
}

