#include <Servo.h>                           // Include servo library

long thresh = 175;
 
Servo servoRight;
Servo servoLeft;
  
void setup() {
  // put your setup code here, to run once:
  pinMode(49, INPUT);
  pinMode(51, INPUT);
  pinMode(53, INPUT);
  pinMode(52, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  long left = rcTime(49);
  long middleleft = rcTime(51);
  long middleright = rcTime(53);
  long right = rcTime(52);

  if (left < thresh && middleleft < thresh && middleright < thresh && right < thresh) {
    forward();
  }
  else if (left > thresh && middleleft > thresh && middleright > thresh && right > thresh ){
    brake();
    delay(500);
    forward();
    delay(500);
  }
  else if (left > thresh && middleleft > thresh) {
    leftturn();
  }
  else if (middleright > thresh && right > thresh){
    rightturn();
  }
  else if(middleleft > thresh && middleright < thresh){
    leftturn();
  }
  else if(middleleft < thresh && middleright > thresh) {
    rightturn();
  }
}

long rcTime(int pin){
  pinMode(pin,OUTPUT);
  digitalWrite(pin,HIGH);
  delayMicroseconds(230);
  pinMode(pin,INPUT);
  digitalWrite(pin,LOW);
  long time= micros();
  while(digitalRead(pin));
  time= micros()-time;
  return time;
}

void forward() {
  // Full speed forward
  servoLeft.attach(12);                      // Attach left signal to pin 13
  servoRight.attach(11);                     // Attach right signal to pin 12

  servoLeft.writeMicroseconds(1550);         // Left wheel counterclockwise
  servoRight.writeMicroseconds(1450);        // Right wheel clockwise

}

void backward() {
  // Full speed backward
  servoLeft.attach(12);                      // Attach left signal to pin 13
  servoRight.attach(11); 
     
  servoLeft.writeMicroseconds(1450);         // Left wheel clockwise
  servoRight.writeMicroseconds(1550);        // Right wheel counterclockwise

}

void leftturn()
{

      servoLeft.attach(12);                      // Attach left signal to pin 13
      servoRight.attach(11);  
  
      servoLeft.writeMicroseconds(1500);         // Left wheel counterclockwise
      servoRight.writeMicroseconds(1450);        // Right wheel counterclockwise
 
 
}

void rightturn()
{
    servoLeft.attach(12);                      // Attach left signal to pin 13
    servoRight.attach(11);  
  
    servoLeft.writeMicroseconds(1550);         // Left wheel counterclockwise
    servoRight.writeMicroseconds(1500);        // Right wheel counterclockwise
 


}

void brake() {
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
}
