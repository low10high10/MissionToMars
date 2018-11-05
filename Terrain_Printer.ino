
int xIn=2;
int yIn=3;

void setup() {
 Serial.begin(9600);
}

void loop() {
  //variables read pulse width
  int pulseX;
  int pulseY;

  pulseX= pulseIn(xIn, HIGH); //read x Pulse
  pulseY= pulseIn(yIn, HIGH); //read y Pulse

  //Displays Pulse Widths
  
  //Serial.print(pulseX); //Tilts Left & Right
  //Serial.print("\t");
  //Serial.println(pulseY);


 

  if((4920 < pulseX && pulseX< 5000)&& (4960 < pulseY && pulseY< 5000) ){
    Serial.println("Flat");
  } 
  else if ((4880 <pulseX ) && (5010<pulseY) || ((4870 <pulseX) && (pulseY<4850)))  {
    Serial.println("Rocky");
  }
  else if(( pulseX < 4860) && (4960<pulseY && pulseY < 5030)){
    Serial.println("Hilly");
  }

  delay(1000);
  
  
}

