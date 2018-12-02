
int xIn=2;
int yIn=3;

void setup() {
 Serial.begin(9600);
 Serial3.begin(9600);
 Serial3.write(12);
}

void loop() {
  //variables read pulse width
  int pulseX;
  int pulseY;

  pulseX= pulseIn(xIn, HIGH); //reads x Pulse (Sideways Tilt)
  pulseY= pulseIn(yIn, HIGH); //reads y Pulse (Forward & Backward Tilt)

  //Displays Pulse Widths
 
  Serial3.print(pulseX); //Tilts Left & Right
  Serial3.print("\t");
  Serial3.print(pulseY);
  Serial3.print(" ");


 
  // Prints 'Flat' if in flat terrain range. 
  if((4850 < pulseX && pulseX < 5100)&& (4750 < pulseY && pulseY < 4850)  ){ 
    Serial3.println("Flat");
  } 
  
  // Prints 'Rocky' if in Rocky terrain range. 
  else if (((4780 <pulseX ) && (5050<pulseY)))  {
    Serial3.println("Rocky right");
  }
  else if ((4750 <pulseX) && (pulseY<4950))  {
    Serial3.println("Rocky left");
  }
  

  // Prints 'Hilly' if in Hilly terrain range. 
  else {
    Serial3.println("Hilly");
  }

  delay(1000);
  Serial3.write(12);
}
