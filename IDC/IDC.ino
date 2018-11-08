#include "IDC.h"
#define Rx 15 
#define Tx 14

IDC a = IDC();

void setup() {
  // put your setup code here, to run once:
  a.setQTI(49, 51, 53, 52);
//  a.setServos(12, 11);
  a.setAxis(2, 3);
  a.initialize(5);  
  a.lineFollow(12, 11);
  a.Transmit();

}

void loop() {
  // put your main code here, to run repeatedly:
  // Make it so that the bot follows the line for a certain amount of time or whatever bound
  a.Recieve();

}
