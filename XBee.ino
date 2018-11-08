#define Rx 15 
#define Tx 14

void setup() {
  // Begin Using the Serial Ports
  Serial.begin(9600);
  Serial2.begin(9600);
  delay(500);

  // Setup Pins
  pinMode(4, INPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  // Turn Pins Off
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  digitalWrite(4, LOW);
}

void loop() {

  // If button is pressed, send an "o" and turn on the yellow LED
  if (digitalRead(4) == HIGH) {
    char outgoing = 'Z';
    Serial2.println(outgoing);
    Serial.println("Button Pressed");
    digitalWrite(6, HIGH);
    delay(200);
  }

  // If something is available on Serial Port 2, turn on the green LED
  if(Serial2.available()) {
    char incoming = Serial2.read();
    digitalWrite(7, HIGH);
    Serial.println(incoming);
  }
}

