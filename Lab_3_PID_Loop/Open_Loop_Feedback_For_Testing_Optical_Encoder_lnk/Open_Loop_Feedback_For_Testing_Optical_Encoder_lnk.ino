#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *DC_gear_head_motor = AFMS.getMotor(1);

const unsigned short SensorPin = 2;

const unsigned short HALF = 125;
const unsigned short FULL = 255;
const unsigned int MAIN_DELAY = 5000;

int timer_prescaler;
int timer_preload;
int white_or_black;
short white_or_black_boolean = 0;
short last_state = 0;
unsigned long last_state_change = 0; // time in microseconds since the last state change
unsigned long num_state_changes = 0;


// debugging
long test_counter;
long start_time;
  
void setup(){
  
  int microsecond_delay = 400; // 2500 hz.
  
  // prescaler = 32
  TCCR1B = TCCR1B | B00000011; // set bits
  TCCR1B = TCCR1B & B11111011; // clear bits
  timer_prescaler = 64; // not sure why this works, but 32 was off
  // by a factor of 2. need to check datasheet again.
  
  long clock_frequency = 16000000 / timer_prescaler;
  long X = (long)((clock_frequency * microsecond_delay) / 1000000);
  timer_preload = 255 + 1 - (byte)X;
  
  AFMS.begin();
  Serial.begin(9600);
  
  Serial.print("timer_preload: ");
  Serial.println(timer_preload);
  Serial.print("clock_frequency: ");
  Serial.println(clock_frequency);
  
  /* Setup phase: configure and enable timer2 overflow interrupt */
  /* First disable the timer overflow interrupt while we're configuring */
  TIMSK2 &= ~(1<<TOIE2);
   
  /* Configure timer2 in normal mode (pure counting, no PWM etc.) */
  TCCR2A &= ~((1<<WGM21) | (1<<WGM20));
  TCCR2B &= ~(1<<WGM22);
   
  /* Select clock source: internal I/O clock */
  ASSR &= ~(1<<AS2);
   
  /* Disable Compare Match A interrupt enable (only want overflow) */
  TIMSK2 &= ~(1<<OCIE2A);
   
   
  /* Finally load end enable the timer */
  TCNT2 = timer_preload;
  TIMSK2 |= (1<<TOIE2);
  start_time = millis();
}

void loop(){
  // debugging
  Serial.println();
  Serial.print("Elapsed Time: ");
  Serial.print((millis() - start_time)/1000);
  Serial.println("s");
  Serial.print("Num Interrupt callbacks: ");
  Serial.println(test_counter);
  Serial.println();
  
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


ISR(TIMER2_OVF_vect) {
  /* Reload the timer */
  white_or_black = analogRead(SensorPin);
  if (white_or_black > 500){ white_or_black_boolean = 1; }
  else{ white_or_black_boolean = 0; }
  
  if (white_or_black_boolean != last_state) // oh my!! the color has changed!!
  {
    if (abs(micros() - last_state_change) > 2000) // .4ms, to provide a basic low pass filter to stop multiple reads 
    {
      // State has changed!!
      last_state = white_or_black_boolean;
      last_state_change = micros();
      num_state_changes ++;
    }
    
  }
  
  TCNT2 = timer_preload;
}

