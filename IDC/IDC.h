#include <Arduino.h>
#include <Servo.h>
#include "HardwareSerial.h"
#include "print.h"

class IDC {
    
  private: 
    int counter;   
   
  public:
    int xIn;
    int yIn;
    int score;
    void setServos();
    void setAxis(int, int);
    void initialize(int);
    int lineFollow();
    void Recieve();
    void Transmit(int);
    long rcTime(int);
    void forward();
    void backward();
    void rightTurn();
    void leftTurn();
    void brake(); 
    int landingSite(int); 
    int sense(int);   
    int quality;
    IDC();    
    Servo servoLeft;
    Servo servoRight;
};
  

