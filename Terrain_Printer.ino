
int xIn=2;
int yIn=3;

void setup() {
 Serial.begin(9600);
}

void loop() {
  //variables read pulse width
  int pulseX;
  int pulseY;

  pulseX= pulseIn(xIn, HIGH); //reads x Pulse (Sideways Tilt)
  pulseY= pulseIn(yIn, HIGH); //reads y Pulse (Forward & Backward Tilt)
 
  // Prints 'Flat' if in flat terrain range. 
  if((4920 < pulseX && pulseX< 5000)&& (4960 < pulseY && pulseY< 5000) ){ 
    Serial.println("Flat");
  } 
  
  // Prints 'Rocky' if in Rocky terrain range. 
  else if ((4880 <pulseX ) && (5010<pulseY) || ((4870 <pulseX) && (pulseY<4850)))  {
    Serial.println("Rocky");
  }

  // Prints 'Hilly' if in Hilly terrain range. 
  else if(( pulseX < 4860) && (4960<pulseY && pulseY < 5030)){
    Serial.println("Hilly");
  }

  delay(1000); 
}
