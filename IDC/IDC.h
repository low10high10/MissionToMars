#include <Arduino.h>
#include <Servo.h>
#include "HardwareSerial.h"
#include "print.h"

class IDC {
    
  private:
    int score;
    int task;
    int qtiPin1;
    int qtiPin2;
    int qtiPin3;
    int qtiPin4;    
    int xIn;
    int yIn;
    int counter;   
    Servo servoLeft;
    Servo servoRight;
   
  public:
    void setQTI(int, int, int, int);    
    void setServos(int, int);
    void setAxis(int, int);
    void initialize(int);
    void lineFollow(int, int);
    void Recieve();
    void Transmit();
    long rcTime(int);
    void forward();
    void backward();
    void rightTurn();
    void leftTurn();
    void brake(); 
    void landingSite(); 
    void sense();   
    IDC();
};
  

