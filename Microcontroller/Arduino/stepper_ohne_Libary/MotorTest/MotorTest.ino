// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#include <AFMotor.h>

AF_DCMotor motor(1);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Motor test!");

  // turn on motor
  motor.setSpeed(0);
 
  motor.run(RELEASE);
}

void loop() {
  
  Serial.print("tick");
  
  motor.run(FORWARD);
  motor.setSpeed(200);

  delay(10000);
  motor.run(RELEASE);
  delay(3000); 
}
