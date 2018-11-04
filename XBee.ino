#define Rx 15 
#define Tx 14

void setup() {
  // Begin Using the Serial Ports
  Serial.begin(9600);
  Serial2.begin(9600);
  delay(500);

  // Setup Pins
  pinMode(8, INPUT);
  pinMode(9, OUTPUT);
  pinMode(4, OUTPUT);

  // Turn Pins Off
  digitalWrite(9, LOW);
  digitalWrite(4, LOW);
}

void loop() {

  // If button is pressed, send an "o" and turn on the yellow LED
  if (digitalRead(8) == HIGH) {
    char outgoing = 'o';
    Serial2.println(outgoing);
    Serial.println("Button Pressed");
    digitalWrite(4, HIGH);
    delay(200);
  }

  // If something is available on Serial Port 2, turn on the green LED
  if(Serial2.available()) {
    char incoming = Serial2.read();
    digitalWrite(9, HIGH);
    Serial.println(incoming);
  }
}

