#include <TimedAction.h>
#include <Servo.h>

// Initializes a TimedAction class to run manual function every 50 ms 
TimedAction manualThread = TimedAction(40,manual);

TimedAction recordThread = TimedAction(50,record);

// Position for store array
int i = 0;

// Initialize all of the needed servo motors
Servo baseServo;
Servo shoulderServo;
Servo elbowServo;
Servo gripperServo;

// Initialize all potentiometers and their value holders
short basePot = A0;
short basePotVal;
short shoulderPot = A1;
short shoulderPotVal;
short elbowPot = A2;
short elbowPotVal;
short gripperPot = A3;
short gripperPotVal;

// Initialize the LEDs
const int LED1  = 22; // play mode indicator 
const int LED2  = 24; // record mode indicator

// Initialize button and their value holders
const int playButton    = 6;
const int recordButton  = 7;
boolean playButtonState = false;
boolean recordButtonState = false;

// Size of array
const short arraySize = 200;

// Initialize array to storage potentiometer values
short baseServoPosStorge[arraySize];
short shoulderServoPosStorge[arraySize];
short elbowServoPosStorge[arraySize];
short gripperServoPosStorge[arraySize];

void setup() {

  // Declared the pin of all servo motors
  baseServo.attach(8);
  shoulderServo.attach(9);
  elbowServo.attach(10);
  gripperServo.attach(11);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  pinMode(playButton, INPUT);
  pinMode(recordButton, INPUT);

  // Debug statement
  Serial.begin(9600);
}

void loop() {
  
  // Start direct control of the arm when record is not on
  if (playButtonState == false) { 
     
    // Manual mode start at the correct time
    manualThread.check();

    // Check for play mode
    if (digitalRead(playButton) == HIGH) {
      playButtonState = true;
      recordButtonState = false;

      digitalWrite(LED1, HIGH);

      // Stop the manual thread
      manualThread.disable();
    }
    
    // Check for record mode 
    if (digitalRead(recordButton) == HIGH) {
      recordButtonState = true;

      digitalWrite(LED2, HIGH);
    }
  }

  // Run the recording method
  if (recordButtonState) {
    recordThread.check();
  }

  // Run the playing method
  if (playButtonState) {
     
    for (int j = 0; j < arraySize; j++) {
      baseServo.write(baseServoPosStorge[j]);
      shoulderServo.write(shoulderServoPosStorge[j]);
      elbowServo.write(elbowServoPosStorge[j]);
      gripperServo.write(gripperServoPosStorge[j]);

      delay(50);
    }
    
  }
}

// This function will store pot value into an array
void record() {

  if (i >= arraySize) {

    // Stop the thread from running 
    recordThread.disable();

    digitalWrite(LED2, LOW);
    return;
  }
  
  // Record the position of potentiometer
  baseServoPosStorge[i]     = basePotVal;
  shoulderServoPosStorge[i] = shoulderPotVal;
  elbowServoPosStorge[i]    = elbowPotVal;
  gripperServoPosStorge[i]  = gripperPotVal; 

  Serial.println(i);

  // Increment the array position to next avaliable space within the array
  i = i + 1;
}

// This function will directly control the arm
void manual() {
  basePotVal = analogRead(basePot);  // Read the base potentiometer value between 0 and 1023
  basePotVal = map(basePotVal, 2, 600, 2, 178); // Map value of the base potentiometer from 0-600 to the value of the base servo 0-180
  baseServo.write(basePotVal); // Set servo position according to scaled value

  shoulderPotVal = analogRead(shoulderPot);
  shoulderPotVal = map(shoulderPotVal, 2, 600, 2, 178);
  shoulderServo.write(shoulderPotVal);

  elbowPotVal = analogRead(elbowPot);
  elbowPotVal = map(elbowPotVal, 2, 600, 2, 178);
  elbowServo.write(elbowPotVal);

  gripperPotVal = analogRead(gripperPot);
  gripperPotVal = map(gripperPotVal, 2, 600, 2, 178);
  gripperServo.write(gripperPotVal);
}


