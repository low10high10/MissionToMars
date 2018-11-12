#include "IDC.h"
#define Rx 15 
#define Tx 14

IDC a = IDC();

void setup() {
  a.setAxis(2, 3); 
  a.lineFollow();
  a.Transmit();

}

void loop() {
  a.Recieve();

}
