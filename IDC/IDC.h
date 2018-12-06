#include <Arduino.h>
#include <Servo.h>
#include "HardwareSerial.h"
#include "print.h"

class IDC {
    
  private: 
    int counter;   
   
  public:  
    int threshY;
    int threshX;
    String group1;
    String group2;
    String group3;
    String group4;
    String group5;
    String data;
    int xIn;
    int yIn;
    int score;
    void setServos();
    void initialize(int);
    int lineFollow();
    void Recieve(int);
    long rcTime(int);
    void forward();
    void backward();
    void rightTurn();
    void leftTurn();
    void brake(); 
    int landingSite(int, int, int, int, int); 
    int sense(int, int, int, int, int);   
    int quality;
    IDC();    
    Servo servoLeft;
    Servo servoRight;
};
  

