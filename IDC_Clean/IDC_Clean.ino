#include "IDC.h"
#define Rx 15 
#define Tx 14

IDC a = IDC();
int quality = 0;

void setup() {
  quality = a.lineFollow(); // Go around the track and scan the areas
}

void loop() {
   a.Receive(quality); // Did anyone send anything?
}
