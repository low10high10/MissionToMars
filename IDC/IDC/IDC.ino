#include "IDC.h"
#define Rx 15 
#define Tx 14

IDC a = IDC();
int quality = 5;

void setup() {
  //quality = a.lineFollow(); // goes around the track and does a quality check
  //a.Transmit(quality); // sends the information to the team
  //delay(1000);
  Serial3.begin(9600);
  Serial2.begin(9600);
  Serial.begin(9600);
  Serial3.write(12);
  Serial3.print("Starting Up...");
  Serial.println("Starting Up...");
  delay(2500);
  Serial3.write(12);
  a.Recieve(quality); // did anyone send anything?
}

void loop() {

}



