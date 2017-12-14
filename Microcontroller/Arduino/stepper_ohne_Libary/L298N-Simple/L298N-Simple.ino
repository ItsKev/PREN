#include <L298N.h>

//pin definition
#define EN 10
#define IN1 9
#define IN2 8

//create a motor instance
L298N motor(EN, IN1, IN2);

void setup() {

  //used for display information
  Serial.begin(9600);

  motor.setSpeed(0); // an integer between 0 and 255

}

void loop() {

  //tell the motor to go forward (may depend by your wiring)

 

  //change the initial speed
  
  
  motor.forward();  
  for(int i = 0; i <= 255; i+=10){
    if(i>255){
      i=255; 
    }
    motor.setSpeed(i);
  }

  for(int i = 255; i >= 0; i-=10){
    if(i<0){
      i=0; 
    }
    motor.setSpeed(i);
  }
  
}
