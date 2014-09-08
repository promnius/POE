
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

void setup()
{
  // set lights as output
  
  // set pushbutton and potentiometer reading as inputs
}


void loop()
{
  // ---------------------------------------------------------------------------------------------------------------------------------------------
  // MEASUREING THE WORLD
  // check the pushbutton, if buttton is pressed, increment the number of times the button has been held for.
  // if that number == 2, change state, and set the entering new state flag. this debounces the button (1 reading won't do anything), and stops the loop from incrementing the 
  // state over and over when the button is held down.
  
  // if the number of times the button has been pressed is > large number (1000?), reset to small number (5?). This
  // prevents overflow if the button is held indefinetly, but also must be large enough that change state doesn't trigger.
  
  // this last section could have been accomplished with states (off, debouncing, or on), but this option allows you to easily pick
  // how long you want to debounce for, as a function of refresh rate.
  
  // read the potentiometer, store into array that keeps last X readings (where X ~10?? could work if
  // X == 1, just depends on how much math you want to do). This involves shifting out the oldest value, then
  // shifting in the new reading.
  
  // filter the potentiometer readings to reduce bounce (average last 10? minimum threshold for change?, gaussian distribution?
  // more advanced math?)
  
  
  // -------------------------------------------------------------------------------------------------------------------------------------------------
  // STATES: CONTROLLING THE WORLD  
  
  // now, use switch statement to select current state.
  // state 1: OFF. if initialization flag is set, turn off all lights. else, do nothing.
  
  // state 2: constanly ON: if initialization flag is set, turn on all lights. else, adjust brightness based on potentiometer reading (using PWM)
  
  // state 3: Blinking: if initialization flag is set, turn all lights on and initialize loop counter to 0. else, if counter has reached threshold,
  // invert the state of all the lights, then reset the counter. threshold is set by potentiometer value (ie, rate of blinking). Note: multiple states
  // use this value, so it could be calculated before entering the switch statement.
  
  // state 4: KNIGHT RIDER: if initialization flag is set, initialize loop counter to 0, turn off all but the first light, turn that one on. else, 
  // if counter has reached next threshold, turn on the next light and turn off the previous light. threshold is set by potentiometer 
  // value (ie, controls rate of blinking). if counter exceeds final threshold value, reset counter.
  // optionally, have the lights work back and forth with a double delay on both ends before counter is reset, like a true knight rider.
  
 // -------------------------------------------------------------------------------------------------------------------------------------------------
  
  
  // clear the entering new state flag (if any of the states needed to initialize on state change, they have already done so)
  
  // increment loop counter. no need to worry about overflow- indevidual states will reset the counter when needed for that state.
  
  // delay for 1/framerate, to approximately set loop speed. (this ignores calculation and code execution times, but for 
  // this project timing is non-critical, and the delay time will dwarf execution time for any reasonable frame rate, ie., 30-100)  
}
