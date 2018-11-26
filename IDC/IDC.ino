#include "IDC.h"
#define Rx 15 
#define Tx 14

IDC a = IDC();

void setup() {
  int quality = a.lineFollow(); // goes around the track and does a quality check
  a.Transmit(quality); // sends the information to the team

}

void loop() {
  a.Recieve(); // did anyone send anything?

}
