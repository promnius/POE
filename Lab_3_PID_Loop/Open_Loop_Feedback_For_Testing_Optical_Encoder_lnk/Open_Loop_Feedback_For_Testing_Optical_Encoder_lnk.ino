#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *DC_gear_head_motor = AFMS.getMotor(1);

const unsigned short HALF = 125;
const unsigned short FULL = 255;
const unsigned int MAIN_DELAY = 5000;
  
void setup(){
  AFMS.begin();
  Serial.begin(9600);
}

void loop(){
  Serial.println("Spinning Forward, speed: 50");
  DC_gear_head_motor->run(FORWARD);
  DC_gear_head_motor->setSpeed(HALF); 
  delay(MAIN_DELAY);
  
  Serial.println("Spinning Forward, speed: 100");
  DC_gear_head_motor->run(FORWARD);
  DC_gear_head_motor->setSpeed(FULL); 
  delay(MAIN_DELAY);
  
  Serial.println("Spinning Forward, speed: 50");
  DC_gear_head_motor->run(FORWARD);
  DC_gear_head_motor->setSpeed(HALF); 
  delay(MAIN_DELAY);
  
  Serial.println("Spinning Stopped");
  DC_gear_head_motor->run(FORWARD);
  DC_gear_head_motor->setSpeed(0); 
  delay(MAIN_DELAY);
  
  Serial.println("Spinning Backward, speed: 50");
  DC_gear_head_motor->run(BACKWARD);
  DC_gear_head_motor->setSpeed(HALF); 
  delay(MAIN_DELAY);
  
  Serial.println("Spinning Backward, speed: 100");
  DC_gear_head_motor->run(BACKWARD);
  DC_gear_head_motor->setSpeed(FULL); 
  delay(MAIN_DELAY);
  
  Serial.println("Spinning Backward, speed: 50");
  DC_gear_head_motor->run(BACKWARD);
  DC_gear_head_motor->setSpeed(HALF); 
  delay(MAIN_DELAY);
  
  Serial.println("Spinning Stopped");
  DC_gear_head_motor->run(FORWARD);
  DC_gear_head_motor->setSpeed(0); 
  delay(MAIN_DELAY);
}

