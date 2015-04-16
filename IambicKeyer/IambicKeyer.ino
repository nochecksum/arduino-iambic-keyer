/*
 * Iambic Key Demo
 * https://github.com/xigco/arduino-iambic-keyer
 */


// First switch (usually left or "dit")
const int pinPaddleOne = 2;

// Second switch (usually right or "dah")
const int pinPaddleTwo = 3;

// LED for visual Morse code
const int pinLed = 13;

// 8ohm speaker or piezo buzzer for audio Morse code
const int pinSpeaker = 8;

// WPM control
const int pinPotentiometer = A0;

// Minimum/maximum WPM setting
const int minWpm = 6;
const int maxWpm = 50;

// This prevents a drifting pot from switching into menu mode mid-transmission
const int valPotentiometerTolerance = 4;

// How many times to repeat the demo CQ without speed change
const int modeTimeoutLimit = 2;


/**
 * Internal settings
 */
 
// Counts how many times demo CQ played so far without speed change
int modeTimeoutCounter;

// Used for comparing changes between loops
int valPotentiometerPrevious;

// Used for adding short gaps once between letters
int isLetterComplete = 0;

// Used for adding long gaps once between words
int isWordComplete = 0;

// Calculated WPM from user input
int actualWpm;

// Calculated dit/element length in milliseconds from WPM
int elementLengthMs;

// Current user mode (1=CW, 2=WPM)
int mode = 1;


/**
 * Program setup
 */
void setup()
{
  // Enable input switches
  pinMode(pinPaddleOne, INPUT);
  pinMode(pinPaddleTwo, INPUT);

  // Set current WPM without entering menu mode later
  valPotentiometerPrevious = analogRead(pinPotentiometer);
  updateWpm();

  // Flash the light and make a noise
  digitalWrite(pinLed, HIGH);  
  doSystemTone(60);
  digitalWrite(pinLed, LOW);
  
  // Text morse output and WPM setting to USB
  Serial.begin(9600);
}


/**
 * Main loop
 */
void loop()
{
  // Read the state of paddle and potentiometer
  int valPaddleOne = digitalRead(pinPaddleOne);
  int valPaddleTwo = digitalRead(pinPaddleTwo);
  int valPotentiometer = analogRead(pinPotentiometer);

  // Has potentiometer changed beyond threshold?
  if ((valPotentiometer > (valPotentiometerPrevious + valPotentiometerTolerance) ||
    valPotentiometer < (valPotentiometerPrevious - valPotentiometerTolerance))) {
    
    // Switch to WPM mode
    mode = 2;
  }
  
  // Execute CW mode
  if ( mode == 1 ) {
    
    // Act on user input
    if (valPaddleOne) {

      // Reset word + letter endings
      isLetterComplete = isWordComplete = 0;

      // Play dit
      doDit();

    } else if (valPaddleTwo) {

      // Reset word + letter endings
      isLetterComplete = isWordComplete = 0;

      // Play dah
      doDah();

    } else {

      // Nothing is pressed, check timings
      if (!isLetterComplete){

        // Ending a letter, set flag
        isLetterComplete = 1;

        // Play gap
        doShortGap();

      } else {

        // Still nothing is pressed, check timings
        if (!isWordComplete){

          // Ending a word, set flag
          isWordComplete = 1;

          // Play a gap
          doLongGap();

        } else {
          // Still nothing! Think about ending a transmission here..
        }
      }
    }
    
  // Execute WPM mode
  } else if (mode == 2) {
    
    // Has the user moved the potentiometer?
    if (valPotentiometer == valPotentiometerPrevious) {

      // No - increment a timeout counter
      modeTimeoutCounter++;

    } else {

      // Yes - update our WPM
      updateWpm();

      // Reset counter to give more time
      modeTimeoutCounter = 0;

      // Remember this new potentiometer value
      valPotentiometerPrevious = valPotentiometer;

    }
    
    // Play demo
    doDemo();

    // Are we timing out?
    if (modeTimeoutCounter >= modeTimeoutLimit) {

      // Yes - switch back to CW on next loop
      mode = 1;
      
      // Reset debug output ready for morse output
      Serial.println("/");

    }
  }
}



/**
 * Calculate WPM from potentiometer input
 */
void updateWpm()
{
  // Read current analog value from 0 to 1023
  int valPotentiometer = analogRead(pinPotentiometer);
  
  // Map that value into the range of WPMs configured at the top of the file
  actualWpm = map(valPotentiometer, 0,1023, minWpm, maxWpm);
  
  // Calculate millisecond bit length from new WPM
  updateElementLength();
  
  // Debug output new WPM
  Serial.println("/");
  Serial.print("Setting WPM to ");
  Serial.println(actualWpm);
}

/**
 * Calculate dit length in milliseconds from WPM
 */
void updateElementLength()
{
  elementLengthMs = 1200 / actualWpm;
}

/**
 * Play a system tone
 */
void doSystemTone(int length)
{
  tone(pinSpeaker, 1028);
  delay(elementLengthMs/100 * length);
  noTone(pinSpeaker);
}

/**
 * Play dit
 */
void doDit()
{
  Serial.print(".");
  digitalWrite(pinLed, HIGH);
  tone(pinSpeaker, 988);
  delay(elementLengthMs);
  noTone(pinSpeaker);
  digitalWrite(pinLed, LOW);
  delay(elementLengthMs);
}

/**
 * Play dah
 */
void doDah()
{
  Serial.print("-");
  digitalWrite(pinLed, HIGH);
  tone(pinSpeaker, 988);
  delay(elementLengthMs*3);
  noTone(pinSpeaker);
  digitalWrite(pinLed, LOW);
  delay(elementLengthMs);
}

/**
 * Short gap for between letters
 */
void doShortGap()
{
  Serial.print(" ");
  delay(elementLengthMs*2);
}

/**
 * Long gap for between words
 */
void doLongGap()
{
  Serial.print("/ ");
  delay(elementLengthMs*4);
}


/**
 * Demo CQ output
 */
void doDemo()
{
  doDah();doDit();doDah();doDit();
  doShortGap();
  doDah();doDah();doDit();doDah();
  doLongGap();
}
