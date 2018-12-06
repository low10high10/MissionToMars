#include <Arduino.h>
#include "IDC.h"
#include <Servo.h>
#include "HardwareSerial.h"
#include "print.h"

bool debug = 0;

// Set error bound sizes for x and y
int boundy = 100; 
int boundx = 100;
  
IDC::IDC() {
}

// Takes task, sets score, and begins serial monitor


/////////////////////// LINE FOLLOWING ///////////////////////

int IDC::lineFollow() {

  // Base case (all flat) is 1
  int score = 1; 

  // Start all serial ports and clear LCD
  Serial2.begin(9600);
  Serial3.begin(9600);
  Serial3.write(12); //Clears LCD Serial

  // Set pin LEDs
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  // Do we want to debug?
  if (debug == 0) {
    Serial3.end();
  }

  // Create the error bounds for the first hill
  int threshY = pulseIn(3, HIGH); //reads y Pulse (Sideways Tilt)
  int threshX = pulseIn(2, HIGH); //reads y Pulse (Forward & Backward Tilt)
  int UpperFlatY = threshY + boundy; 
  int LowerFlatY = threshY - boundy; 
  int UpperFlatX = threshX + boundx;
  int LowerFlatX = threshX - boundx; 
  
  Serial3.print(String(threshY) + " "+ String(threshX));
  delay(500);

  // Initialize Servos
  Servo servoRight;
  Servo servoLeft;

  // Begin the has counter
  int counter = 0;

  // Take a reading in the area before the hash
  score += sense(score);    

  // Initialize the QTI sensors 
  long thresh = 175;
  long left, middleleft, middleright, right;

  // Before the fifth has, line follow and scan the area
  while (counter < 5) {

    // Take a reading on the QTI     
    left = rcTime(49);
    middleleft = rcTime(51);
    middleright = rcTime(53);
    right = rcTime(52);

    // If QTI sensors see white, go forward
    if (left < thresh && middleleft < thresh && middleright < thresh && right < thresh) {
      forward();
    }
    
    // If QTI sensors see black, stop
    else if (left > thresh && middleleft > thresh && middleright > thresh && right > thresh) {
      brake();    
      delay(500);

      // Increase the hash counter
      counter += 1;

      // Course correct for the third hash
      if (counter == 3) {
        delay(500);
        rightTurn();
        delay(100);
      }

      // Scan all but the second and fourth hash and add it to our score
      if (counter != 2 && counter != 4) { 
        score += sense(score);
      }

      // Pass the hash before line following again
      forward();      
      delay(500);

    }

    // If anything is on the left, turn left
    else if (left > thresh || middleleft > thresh) {
      leftTurn();
    }

    // If anything is on the right, turn right
    else if (middleright > thresh || right > thresh){
      rightTurn();
    }
  }

  // Return a score to send to other groups
  return score;
}

// Find decay time values for the RC circuit to detect lines
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

// Function for going forward
void IDC::forward() {
  servoRight.attach(11);
  servoLeft.attach(12); 
  servoLeft.writeMicroseconds(1545);         // Left wheel counterclockwise
  servoRight.writeMicroseconds(1450);        // Right wheel clockwise
}

// Function for going backward
void IDC::backward() { 
  servoRight.attach(11);
  servoLeft.attach(12);
  servoLeft.writeMicroseconds(1450);         // Left wheel clockwise
  servoRight.writeMicroseconds(1550);        // Right wheel counterclockwise
}

// Function for turning left
void IDC::leftTurn() {
  servoRight.attach(11);
  servoLeft.attach(12);
  servoLeft.writeMicroseconds(1500);         // Left wheel counterclockwise
  servoRight.writeMicroseconds(1450);        // Right wheel counterclockwise
}

// Function for turning right
void IDC::rightTurn() {
  servoRight.attach(11);
  servoLeft.attach(12);
  servoLeft.writeMicroseconds(1550);         // Left wheel counterclockwise
  servoRight.writeMicroseconds(1500);        // Right wheel counterclockwise
}

// Function for stopping
void IDC::brake() {
  servoRight.attach(11);
  servoLeft.attach(12);
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
  servoRight.detach();
  servoLeft.detach();
}

// Function to get to the site to scan
int IDC::sense(int score) {

  // This will be added to the score
  int add = 0;

  // Create new thresholds
  int threshY = pulseIn(3, HIGH); //reads y Pulse (Sideways Tilt)
  int threshX = pulseIn(2, HIGH); //reads y Pulse (Forward & Backward Tilt)
  int UpperFlatY = threshY + boundy; 
  int LowerFlatY = threshY - boundy; 
  int UpperFlatX = threshX + boundx;
  int LowerFlatX = threshX - boundx; 
  Serial3.print(String(threshY) + " "+ String(threshX));
  delay(500);

  // Go to the area and stop
  forward();
  delay(1500);
  brake();    
  delay(500);    

  // Take the average reading to compare with the threshold
  int pulseX = pulseIn(2, HIGH); 
  int pulseY = pulseIn(3, HIGH); 
  delay(100);
  pulseX += pulseIn(2, HIGH);
  pulseY += pulseIn(3, HIGH);
  delay(100);
  pulseX += pulseIn(2, HIGH);
  pulseY += pulseIn(3, HIGH);

  pulseX = pulseX/3;
  pulseY = pulseY/3;
  
  delay(500);
  Serial3.write(12);
  
  // Prints 'F' if in flat terrain range. 
  if((LowerFlatX < pulseX && pulseX < UpperFlatX) && (LowerFlatY < pulseY && pulseY < UpperFlatY) ){ 
    Serial3.print("F "); 
    Serial3.write(12);
    Serial3.println(String(UpperFlatY) + " " + String(LowerFlatY) + " " + String(pulseY));
    Serial3.write(13);
    Serial3.println(String(UpperFlatX) + " " + String(LowerFlatX) + " " + String(pulseX));

    // Turn on red LED 
    digitalWrite(7, HIGH);
  } 
  
  // Prints 'R' if in Rocky terrain range. 
  else if (UpperFlatY < pulseY || pulseY < LowerFlatY)  {
    Serial3.print("R "); //prints onto LCD Display
    Serial3.write(12);
    Serial3.println(String(UpperFlatY) + " " + String(LowerFlatY) + " " + String(pulseY));
    Serial3.write(13);
    Serial3.println(String(UpperFlatX) + " " + String(LowerFlatX) + " " + String(pulseX));

    // Add 1 to the score and leave the area 
    add = 1;
    forward();
    delay(750);
    digitalWrite(6,LOW);
    digitalWrite(7,LOW);
  }

  // Prints 'H' if not in Flat or Rocky terrain range. 
  else {
    Serial.println("Hilly"); // is it nothing else? its hilly!!
    Serial3.print("H "); //prints onto LCD Display
    Serial3.write(12);
    Serial3.println(String(UpperFlatY) + " " + String(LowerFlatY) + " " + String(pulseY));
    Serial3.write(13);
    Serial3.println(String(UpperFlatX) + " " + String(LowerFlatX) + " " + String(pulseX));

    // Turn on green LED and if the score is 1, add 2 instead of 1
    digitalWrite(6, HIGH);
    if (score == 1) {
      add = 2;
    }
    else {
      add = 1;
    }

    // Leave area
    forward();
    delay(1000);
    digitalWrite(6,LOW);
    digitalWrite(7,LOW);
  }
  return add; // what will be added to score
}

void IDC::Receive(int quality) {
  
 // Convert value into a unique character to send
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

 // Initalize the other scores
 String group1 = "0";
 String group2 = "0";
 String group3 = "0";
 String group4 = "0";
 String group5 = String(quality);

 Serial2.print(transmit);
 
 // Loop forever to get the other scores
 while(1) {  
  if (Serial2.available()) {
     char data = Serial2.read();
     if (data == 'a') {
        group1 = "1";
     }
     else if (data == 'b') {
        group1 = "2";
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
  }
  
  // Combining the data into one string and displaying it
  String Mission = (group1 + " " + group2 + " " + group3 + " " + group4 + " " + group5);
  Serial3.begin(9600);
  Serial3.write(12);
  Serial3.print(Mission);
  Serial3.write(13);
  Serial3.print(group5);
  delay(500);
 }
}
