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

   motor.setSpeed(10); 
   motor.forward(); 
   delay(2000); 

  motor.setSpeed(100); 
   motor.forward(); 
   delay(2000); 

   motor.setSpeed(150); 
   motor.forward(); 
   delay(2000);

   motor.setSpeed(200); 
   motor.forward(); 
   delay(2000);

   motor.setSpeed(255); 
   motor.forward(); 
   delay(5000);
}
