
// HEADER
// Project: POE Lab 1: Bike Light
// Description: use basic input devices to control the reaction of LEDs, designed to mimic the various modes of a bike light.
// Authors: Kyle Mayer and Ryan Loue
// Date: September 7, 2014
// Interesting Fact: The great Pyramid of Giza currently measures 455 feet tall, although it is estimated that at the time of construction
// it stood 480 feet or taller- the current pyramid no longer has its capstone. It was the tallest man-made structure until the Lincoln's
// cathedral was constructed in 1300 A.D.
//----------------------------------------------------------------------------------------------------------------------------------------------------
// NOTES

// about the code:
// this uses a game loop style (periodic polling of the inputs) to monitor the world and update the displays. advantages: very straight forward, few hardware dependancies 
// (can move to other platforms effortlessly), can add additional states with little work. Disadvantages: proccessor intensive (must continue
// to run through game loop at refresh rate even when system is stagnant), no precision timing*, and there will always be an unneccessary minor delay between
// input polling. This will be too fast for the human eye, but a button could theoretically be pressed and released so fast that the system does not register.

// possible future improvements:
// many possible optimizations for knight rider mode, including better support for arbitrary numbers of lights, computational efficiency,
// and code clarity.
// functionalize the main loop, for readability and reusability
// power saving options- power saving was not considered as a part of this project, since power savings would have to start with the electronics. (for example, LED current
// is set using a simple current limiting resistor- very wasteful). For this to be feasable as an actual bike light (battery operated), many changes could be made
// to the code to reduce power consumption. (including moving to a true interrupt driven state machine, rather than a game loop).
// create a state machine object to make code more portable and to reduce the amount of code present in the main loop,
// since many similar projects use a state machine/ game loop** style of refresh.

// *note: precision timing could still be achieved using a game-loop style, but would require additional work that this project would not benifiet from.
// **note: an actual state machine would not need to continue returning to the main loop to check inputs, but would rather stay in a given state until 
// driven to change, typically by interrupts. This is why we refer to our code as a hybrid of a game loop and a state machine.

//----------------------------------------------------------------------------------------------------------------------------------------------------
// START CODE

// CONSTANTS
const short LED_red = 10; // LED pin numbers (digital pins)
const short LED_green = 9;
const short LED_yellow = 11;
const short PushButton = 8; // pushbutton pin number
const short Potentiometer_Input = 0; // pot pin number (analog input pin)
const unsigned short framerate = 100; // 100 for smooth operation, 2 for debugging. sets the refresh rate.
const boolean using_Pot = true; // determines if the potentiometer is used or not. Currently, only true is supported.
const unsigned int delay_time = 1000/(framerate);   // delay for 1/framerate, to approximately set loop speed. (this ignores calculation and code execution times, but for 
// this project timing is non-critical, and the delay time will dwarf execution time for any reasonable frame rate, ie., 30-100)
// --------------------------------------------------------------------------------------------------------------------------------------------------

// VARIABLES
unsigned int PushButtonCounter; // how many loops has the button been held for? Used for debouncing.
short State; // keeps track of which state the lights are in, off, blinking, flashing, or knight rider.
int Pot_raw; // raw reading from the potentiometer, unfiltered
int Pot_old = -500; // will be overwritten immediately, initial value is unimportant. stores the filtered potentiometer reading
boolean New_State_Flag; // set true when we first change state. allows the state to know when it was first entered, in case initialization is neccessary.
unsigned int loop_counter; // counts the loops, used by any state that needs to continue operations from one iteration to the next.
unsigned int threshold_blink = framerate; // How fast do the lights blink during blink mode. default to 1Hz. may be reset by pot.
boolean LED_State; // With more elegant code, this variable should not be neccessary. Keeps track of whether the lights are on or off during the blink mode.
short Knight_Rider_LEDs[] = { LED_green, LED_red, LED_yellow}; // populate Knight_Rider_LED array with the appropriate order of lights
unsigned int threshold_knight_rider = (framerate * 1) / 12; // default to 12 Hz. sets the speed of the knight rider mode

// ----------------------------------------------------------------------------------------------------------------------------------------------------

void setup()
{
  // initialize serial communication for debugging purposes
  // Serial.begin(19200);
  // set lights as output
  pinMode(LED_red, OUTPUT);
  pinMode(LED_green, OUTPUT);
  pinMode(LED_yellow, OUTPUT);  
  // set pushbutton and potentiometer reading as inputs
  pinMode(PushButton, INPUT);
  pinMode(Potentiometer_Input, INPUT); // this may actually set digital pin 0 to input,
  // not really important since digital pin 0 is not used and pins default to input state anyway.
}


void loop()
{
  // ---------------------------------------------------------------------------------------------------------------------------------------------
  // MEASUREING THE WORLD
  // check the pushbutton, if buttton is pressed, increment the number of times the button has been held for.
  if (digitalRead(PushButton)) 
  {
    PushButtonCounter ++;
  }
  else {PushButtonCounter = 0;}
  // if that number == 2, change state, and set the entering new state flag. this debounces the button (1 reading won't do anything), and stops the loop from incrementing the 
  // state over and over when the button is held down.
  if (PushButtonCounter == 2)
  {
    State ++; // 0 == OFF, 1 == ON, 2 == BLINKING, 3 == KNIGHT RIDER
    New_State_Flag = true;
  }
  if (State > 3) {State = 0;}
  
  // if the number of times the button has been pressed is > large number (1000?), reset to small number (5?). This
  // prevents overflow if the button is held indefinetly, but the reset value must be large enough that change state doesn't trigger.
  if (PushButtonCounter > 60000) {PushButtonCounter = 20;}
  
  // read the potentiometer
  Pot_raw = analogRead(Potentiometer_Input);
  // filter the reading to avoid errotic bouncing.
  if (abs(Pot_raw - Pot_old) > 10)
  {
    Pot_old = Pot_raw;
  }
  
  // calculate the appropriate thresholds for the blink modes. magic numbers based off of hard coded frequency ranges
  // determined experimentally.
  if (using_Pot == true) {
    threshold_blink = ((long)framerate * (Pot_old + 150)) / 1624; // can get too large for integers during intermediate math, thats why we cast to long
    // Maximum: 11 Hz, Minimum: 1.4 Hz
    threshold_knight_rider = ((long)framerate * (Pot_old + 1024)) / 20480; // maximum: 20 Hz, Minimum: 10 Hz
  }
  
  // END MEASUREING THE WORLD
  // -------------------------------------------------------------------------------------------------------------------------------------------------
  // STATES: CONTROLLING THE WORLD  
  
  // state 0: OFF. state 1: constanly ON. state 2: Blinking. state 3: KNIGHT RIDER
  
  // if (New_State_Flag) {Serial.print("Current State: "); Serial.println(State);} // debugging
  switch(State)
  {
    case 0: // Case: OFF. if initialization flag is set, turn off all lights. else, do nothing.
      if (New_State_Flag) {
        allLow();
      }
      break;
      
    case 1: // Case: ON. Adjust brightness based on potentiometer reading (using PWM)
      // Adjust Brightness
      if (using_Pot == true)
      {
        analogWrite(LED_red, (((long)Pot_old + 100) * 255) / 1124); // magic numbers determined experimentally,
        // designed to make sure that the LEDs are never completely off, even when fully dimmed.
        analogWrite(LED_green, (((long)Pot_old + 100) * 255) / 1124);
        analogWrite(LED_yellow, (((long)Pot_old + 100) * 255) / 1124);
      }
      else
      {
        digitalWrite(LED_red, HIGH);
        digitalWrite(LED_green, HIGH);
        digitalWrite(LED_yellow, HIGH);
      }
      break;
    case 2: // Case: Blinking. if initialization flag is set, reset loop counter.
      if (New_State_Flag) {
        loop_counter = 1; // correcting for off by 1 error.
        allLow();
      }
      else
      {
        if (loop_counter >= threshold_blink) { // counter has reached threshold. invert the light states. reset loop counter for next blink.
          // Very un-elegant way of doing this. would be much better to just invert the states
          if (LED_State == true){// lights are on, turn them off
            allLow();
          }
          else{ // lights are off, turn them on
            allHigh();
          }
          LED_State = !LED_State;

          loop_counter = 0; // reset the counter for the next blink
        }
        loop_counter ++;        
      }
      break;
    case 3: // Case: Knight Rider. initialize loop counter to 0
      if (New_State_Flag) {
        loop_counter = 0;
        // could use an array that represents the actual outputs, then just shift left and right. This would
        // be computationally much faster, and the code would be shorter, though more complicated to write.
        // (direct port access to the PORTB register)
        oneLightOn(0);
      }
      else {
        // Note: Order of if statements is important, since the final if statement will evaluate true even when the
        // first one does, although we only want one to execute. magic numbers are based on the number of lights we have,
        // so adding more lights would require rewriting some of this code. this section is listed for re-write under the
        // list of future improvements.
        if (loop_counter >= threshold_knight_rider * 6 ) { // first light on
          oneLightOn(0);
          loop_counter = 0; // the full sequence has been completed.
        }
        else if (loop_counter >= threshold_knight_rider * 5) { // second light on
          oneLightOn(1);
        }
        else if (loop_counter >= threshold_knight_rider * 3) { // third light on
          oneLightOn(2);
        }
        else if (loop_counter >= threshold_knight_rider * 2) { // second light on again
          oneLightOn(1);
        }
        loop_counter ++;
      }    
        
      break;
    default:
      // Serial.println("ERROR: Unknown state accessed.");
      break;
  }
  
 // END STATES: CONTROLLING THE WORLD 
 // -------------------------------------------------------------------------------------------------------------------------------------------------
 // COMPLETEING LOOP LOGISTICS 
  
  // clear the entering new state flag (if any of the states needed to initialize on state change, they have already done so)
  New_State_Flag = false;

  delay(delay_time); // delay to set frame rate.
} // END MAIN LOOP

// END MAIN LOOP: COMPLETEING LOOP LOGISTICS
// -------------------------------------------------------------------------------------------------------------------------------------------------------
// SUPPORT FUNCTIONS

// Turn on all lights
void allHigh() {
  digitalWrite(LED_red, HIGH);
  digitalWrite(LED_green, HIGH);
  digitalWrite(LED_yellow, HIGH);
}

// turn off all lights
void allLow() {
  digitalWrite(LED_red, LOW);
  digitalWrite(LED_green, LOW);
  digitalWrite(LED_yellow, LOW);
}


// turns on one of the three lights, turn off the other two.
void oneLightOn(short light) { // light must be 0,1, or 2. No error checking provided.
  digitalWrite(Knight_Rider_LEDs[0], LOW);
  digitalWrite(Knight_Rider_LEDs[1], LOW);
  digitalWrite(Knight_Rider_LEDs[2], LOW);
  digitalWrite(Knight_Rider_LEDs[light], HIGH);
}

// END SUPPORT FUNCTIONS
// ----------------------------------------------------------------------------------------------------------------------------------------------------------
// END PROGRAM


