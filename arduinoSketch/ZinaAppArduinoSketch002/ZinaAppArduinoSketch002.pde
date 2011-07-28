/* @file HelloKeypad.pde
 * || @version 1.0
 * || @author Alexander Brevig
 * || @contact alexanderbrevig@gmail.com
 * ||
 * || @description
 * || | Demonstrates the simplest use of the matrix Keypad library.
 * || #
 * 
 * keypad mapping order by arne boon @ 21-01-2011 for Zina
 * rows: 2, 7, 6, 4
 * cols: 3, 1, 5
 * 
 * than add number to correspond with pinouts on hardware (in this case pins 2-8 are used)
 * rows: 3, 8, 7, 5
 * cols: 4, 2, 6
 * 
 * @2011-02-02
 * rows: 4, 11, 10, 8
 * cols: 5, 3, 9
 *
 */

//-include---------------------------------------------------
#include <Keypad.h>

//-var---------------------------------------------------
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

//-keypad---------------------------------------------------
byte rowPins[ROWS] = {4, 11, 10, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 3, 9}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//-ldr---------------------------------------------------
int ldrPin = A0;
int ldrValue = 0;
int valuePrevious = 1024;     
int thresholdFactor = 3;         // <<<<<<------------------- THIS VAR FOR VALUE DIFFERENCE FACTOR IN THRESHOLD * as big or / as small
int thresholdMinus = 100;
int thresholdPlus = 100;

//--ldr readings: build in a break to get more extreme values between changes
boolean bReading = true;
int readingCounter = 0;
int numReadingRounds = 1200;   // <<<<<<------------------- THIS VAR FOR DELAY BETWEEN READINGS: BIGGER DELAY = EXTREMER CHANGES

//--skip loop when reading a change of valueThreshold, otherwise you get 'multiple changes' in a big change (bv. 20 fits 5x 100)
boolean bPrintValue = true;
int printCounter = 0;
int numPrintRounds = 3 * numReadingRounds;

//-setup---------------------------------------------------
void setup(){
  //-open serial port
  Serial.begin(9600);
}

//-loop---------------------------------------------------  
void loop(){
  
  //--keypad-------------------------
  char key = keypad.getKey();
  
  if (key != NO_KEY){
    Serial.print("k");
    Serial.println(key);
  }
  
  //--ldr--------------------------
  if(bReading) {
    ldrValue = analogRead(ldrPin); 
    bReading = false;
   
    
    //--print values
    Serial.print(ldrValue);
    Serial.print(" @ minus: ");
    Serial.print(thresholdMinus);
    Serial.print(" @ plus: ");
    Serial.print(thresholdPlus);
    for(int i=0; i < ldrValue; i+=10) {
      Serial.print(".");
    }
    Serial.println("|");
    
    
  } else {
     readingCounter++;
    
    if(readingCounter > numReadingRounds) {
      readingCounter = 0;
      bReading = true;
    }   
  }
 
  //--print out only first occurance of changing value (build in pause for next value)
  if(bPrintValue) {
    
    //if the value goes down
    if(ldrValue < valuePrevious - thresholdMinus) {
      Serial.println("l0");
      bPrintValue = false;
    }
    
    //if the value goes up
    if(ldrValue > valuePrevious + thresholdPlus) {
      Serial.println("l1");
      bPrintValue = false;
    }
  } else {
    printCounter++;
    
    if(printCounter > numPrintRounds) {
      printCounter = 0;
      bPrintValue = true;
      //Serial.println("PRINT COUNTER RESET");
    } 
  }
  
  //--set value for next measurement
  valuePrevious = ldrValue;
  thresholdMinus = valuePrevious / thresholdFactor;
  thresholdPlus = valuePrevious * thresholdFactor;

}








