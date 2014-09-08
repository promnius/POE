
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

// *note: precision timing could still be achieved using a game-loop style, but would require additional work that this project would not benifiet from.
// **note: an actual state machine would not need to continue returning to the main loop to check inputs, but would rather stay in a given state until 
// driven to change, typically by interrupts. This is why we refer to our code as a hybrid of a game loop and a state machine.

// possible future improvements:
// include the ability to disable the potentiometer, either via a firmware change of a single constant, or via an external button (pushbutton or SPST). This
// would involve having constants for all the 'standards' that are normally derived from the potentiometer, and every time the potentiometer value is used having an
// if statement to check which value to use.
// functionalize the main loop
// power saving options- power saving was not considered as a part of this project, since power savings would have to start with the electronics. (LED current
// is set using a simple current limiting resistor- very wasteful). For this to be feasable as an actual bike light (battery operated), many changes could be made
// to the code to reduce power consumption. (including moving to a true interrupt driven state machine, rather than a game loop).
// create a state machine object to make code more portable and to reduce the amount of code present in the main loop,
// since many similar projects use a state machine/ game loop** style of refresh.

//----------------------------------------------------------------------------------------------------------------------------------------------------
// CODE

// Constants
const short LED_red = 12;
const short LED_green = 11;
const short LED_yellow = 13;
const short PushButton = 8;
const short Potentiometer_Input = 9;
const unsigned short framerate = 100; // 100 for smooth operation, 2 for debugging

// variables. NEED TO ADD DESCRIPTIONS OF ALL VARIABLES.
unsigned int PushButtonCounter;
short State;
int Pot_raw;
boolean New_State_Flag;
unsigned int loop_counter;
unsigned int threshold = framerate; // default to 1 second blink.
boolean LED_State;
short Knight_Rider_LEDs[] = { LED_green, LED_red, LED_yellow}; // populate Knight_Rider_LED array with the appropriate order of lights
unsigned int delay_time;

void setup()
{
  // initialize serial communication for debugging purposes
  Serial.begin(9600);
  // set lights as output
  pinMode(LED_red, OUTPUT);
  pinMode(LED_green, OUTPUT);
  pinMode(LED_yellow, OUTPUT);  
  // set pushbutton and potentiometer reading as inputs
  pinMode(PushButton, INPUT);
  pinMode(Potentiometer_Input, INPUT);
}


void loop()
{
  // ---------------------------------------------------------------------------------------------------------------------------------------------
  // MEASUREING THE WORLD
  // check the pushbutton, if buttton is pressed, increment the number of times the button has been held for.
  // if that number == 2, change state, and set the entering new state flag. this debounces the button (1 reading won't do anything), and stops the loop from incrementing the 
  // state over and over when the button is held down.
  //Serial.print("Push Button Counter: "); // debugging
  //Serial.println(PushButtonCounter);
  if (digitalRead(PushButton)) 
  {
    PushButtonCounter ++;
  }
  else {PushButtonCounter = 0;}
  if (PushButtonCounter == 2)
  {
    State ++; // 0 == OFF, 1 == ON, 2 == BLINKING, 3 == KNIGHT RIDER
    New_State_Flag = true;
  }
  if (State > 3) {State = 0;}
  
  // if the number of times the button has been pressed is > large number (1000?), reset to small number (5?). This
  // prevents overflow if the button is held indefinetly, but also must be large enough that change state doesn't trigger.
  if (PushButtonCounter > 60000) {PushButtonCounter = 20;}
  
  // this last section could have been accomplished with states (off, debouncing, or on), but this option allows you to easily pick
  // how long you want to debounce for, as a function of refresh rate.
  
  // read the potentiometer, store into array that keeps last X readings (where X ~10?? could work if
  // X == 1, just depends on how much math you want to do). This involves shifting out the oldest value, then
  // shifting in the new reading.
  Pot_raw = analogRead(Potentiometer_Input);
  
  // filter the potentiometer readings to reduce bounce (average last 10? minimum threshold for change?, gaussian distribution?
  // more advanced math?). Also calculate threshold value for blink speed and brightness.
  // IGNORED FOR NOW.
  
  
  // -------------------------------------------------------------------------------------------------------------------------------------------------
  // STATES: CONTROLLING THE WORLD  
  
  // now, use switch statement to select current state.
  // state 0: OFF. if initialization flag is set, turn off all lights. else, do nothing.
  
  // state 1: constanly ON: if initialization flag is set, turn on all lights. else, adjust brightness based on potentiometer reading (using PWM)
  
  // state 2: Blinking: if initialization flag is set, turn all lights on and initialize loop counter to 0. else, if counter has reached threshold,
  // invert the state of all the lights, then reset the counter. threshold is set by potentiometer value (ie, rate of blinking). Note: multiple states
  // use this value, so it could be calculated before entering the switch statement.
  
  // state 3: KNIGHT RIDER: if initialization flag is set, initialize loop counter to 0, turn off all but the first light, turn that one on. else, 
  // if counter has reached next threshold, turn on the next light and turn off the previous light. threshold is set by potentiometer 
  // value (ie, controls rate of blinking). if counter exceeds final threshold value, reset counter.
  // optionally, have the lights work back and forth with a double delay on both ends before counter is reset, like a true knight rider.
  
  if (New_State_Flag) {Serial.print("Current State: "); Serial.println(State);}
  switch(State)
  {
    case 0:
      if (New_State_Flag) {
        allLow();
      }
      break;
    case 1:
      if (New_State_Flag) {
        allHigh();
      }
      else
      {
        // ADJUST BRIGHTNESS, WILL DO LATER.
      }
      break;
    case 2:
      if (New_State_Flag) {
        loop_counter = 1;
        allLow();
        //Serial.print("ALL LOW FIRST TIME");
      }
      else
      {
        if (loop_counter >= threshold) {
          // Very un-elegant way of doing this. would be much better to just invert the states
          //Serial.print("LED State: ");
          //Serial.println(LED_State); // Debugging
          if (LED_State == true){
            allLow();
            //Serial.print("ALL LOW");
          }
          else{
            allHigh();
            //Serial.print("ALL HIGH");
          }
          LED_State = !LED_State;

          loop_counter = 0;
        }
        loop_counter ++;        
      }
      break;
    case 3:
      if (New_State_Flag) {
        loop_counter = 0;
        // could use an array that represents the actuall outputs, then just shift left and right. This would
        // be computationally much faster, and the code would be shorter, though more complicated to write.
        oneLightOn(0);
      }
      else {
        // VERY UGLY. HARDCODED IN. SHOULD BE FIXED. This would be solved by the above suggestion of shifting an array back and forth.
        // ADDITIONAL NOTE OF BADDNESS: Order of if statements is important, since the final if statement will evaluate true even when the
        // first one does, although we only want one to execute.
        //Serial.print("Threshold Value: "); // debugging
        //Serial.println(threshold);
        //Serial.print("Multiplied Threshold Value: ");
        //Serial.println((threshold * 4));
        //Serial.print("Loop Counter: ");
        //Serial.println(loop_counter);
        if (loop_counter >= threshold * 4) { // first light on
          oneLightOn(0);
          loop_counter = 0;
          //Serial.print("Color: Green");
        }
        else if (loop_counter >= threshold * 3) { // second light on
          oneLightOn(1);
          //Serial.print("Color: Red");
        }
        else if (loop_counter >= threshold * 2) { // third light on
          oneLightOn(2);
          //Serial.print("Color: Yellow");
        }
        else if (loop_counter >= threshold * 1) { // second light on
          oneLightOn(1);
          //Serial.print("Color: Red");
        }
        loop_counter ++;
      }    
        
      break;
    default:
      Serial.println("ERROR: Unknown state accessed.");
      break;
  }
  
  
 // -------------------------------------------------------------------------------------------------------------------------------------------------
  
  
  // clear the entering new state flag (if any of the states needed to initialize on state change, they have already done so)
  New_State_Flag = false;
  
  // increment loop counter. no need to worry about overflow- indevidual states will reset the counter when needed for that state.
  // UPDATE: Done in the indevidual states that need it.
  
  // delay for 1/framerate, to approximately set loop speed. (this ignores calculation and code execution times, but for 
  // this project timing is non-critical, and the delay time will dwarf execution time for any reasonable frame rate, ie., 30-100)
  delay_time = 1000/(framerate);
  //Serial.print("Delay Time: ");
  //Serial.println(delay_time); // debugging
  delay(delay_time);  
}


void allHigh() {
  digitalWrite(LED_red, HIGH);
  digitalWrite(LED_green, HIGH);
  digitalWrite(LED_yellow, HIGH);
}

void allLow() {
  digitalWrite(LED_red, LOW);
  digitalWrite(LED_green, LOW);
  digitalWrite(LED_yellow, LOW);
}

void oneLightOn(short light) { // light must be 0,1, or 2. No error checking provided. function not called by humans.
  digitalWrite(Knight_Rider_LEDs[0], LOW);
  digitalWrite(Knight_Rider_LEDs[1], LOW);
  digitalWrite(Knight_Rider_LEDs[2], LOW);
  digitalWrite(Knight_Rider_LEDs[light], HIGH);
}
