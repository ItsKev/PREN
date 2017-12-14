#include <Adafruit_MotorShield.h>
AF_DCMotor motor(1);

// DC motor on M2
AF_DCMotor motor2(2);

void setup() {

// Turn ON DC MOTOR #1
motor.setSpeed(200);
motor.run(RELEASE);

// TURN ON DC MOTOR #2
motor2.setSpeed(200);
motor2.run(RELEASE);
}


void loop() {
motor.run(FORWARD);
motor2.run(FORWARD);

for (i=0; i<255; i++) {
motor.setSpeed(i);
motor2.setSpeed(i); 
delay(10);
}

motor.run(BACKWARD);
motor2.run(BACKWARD);
for (i=0; i<255; i++) {
motor.setSpeed(i); 
motor2.setSpeed(i);
delay(10);

}
}
