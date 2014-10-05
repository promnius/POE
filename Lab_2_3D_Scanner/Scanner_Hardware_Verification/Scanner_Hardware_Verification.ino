
#include <Servo.h> 

Servo panServo;
Servo tiltServo;
const unsigned short panPotPin = 2; // analog pin
const unsigned short tiltPotPin = 3; // analog pin
const unsigned short panServoPin = 9;
const unsigned short tiltServoPin = 10;

void setup(){
  // to avoid needing breakout boards for power and ground (since we
  // only get 1 Vdd and 2 gnd pins), we use many of the digital IO
  // pins as power and ground for low power periferials. Since we have
  // no other use for these pins, it saves us extra hardware.
  pinMode(6, OUTPUT); // power for pan pot
  digitalWrite(6, HIGH);
  pinMode(4, OUTPUT); // ground for pan pot
  digitalWrite(4, LOW);
  
  pinMode(5, OUTPUT); // power for tilt pot
  digitalWrite(5, HIGH);
  pinMode(3, OUTPUT); // ground for tilt pot
  digitalWrite(3, LOW);
  
  pinMode(19, OUTPUT); // power for Xbee (re-purposed analog pin)
  digitalWrite(19, HIGH);
  pinMode(18, OUTPUT); // ground for Xbee (re-purposed analog pin)
  digitalWrite(18, LOW);
  
  panServo.attach(panServoPin);
  tiltServo.attach(tiltServoPin);
  
  Serial.begin(9600);
  
  // begin debugging section- verification that hardware is doing 
  // what we expect.
  delay(500);
  tiltServo.write(90);
}

void loop(){
  Serial.println("still moving!");
  delay(1000);
}


