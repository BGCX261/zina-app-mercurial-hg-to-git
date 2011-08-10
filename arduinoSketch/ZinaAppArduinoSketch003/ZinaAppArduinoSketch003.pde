/* 
 * @file ZinaAppArduinoSketch003: keypad & reed switch
 * 
 * == KEYPAD ==
 * keypad mapping order by arne boon @ 21-01-2011 for Zina
 * rows: 2, 7, 6, 4
 * cols: 3, 1, 5
 * 
 * than add number to correspond with pinouts on hardware (in this case pins 2-8 are used)
 * rows: 3, 8, 7, 5
 * cols: 4, 2, 6
 * 
 * @2011-02-02: in this case digital pins 3,4,5 and 8,9,10,11 are used
 * rows: 4, 11, 10, 8
 * cols: 5, 3, 9
 *
 * == REED SWITCH ==
 * digital pin pin: 2
 */

//-include---------------------------------------------------
#include <Keypad.h>

//-var---------------------------------------------------
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'  } ,
  {'4','5','6'  } ,
  {'7','8','9'  } ,
  {'*','0','#'  }
};

//-keypad---------------------------------------------------
byte rowPins[ROWS] = {4, 11, 10, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 3, 9}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//-reed switch--------------------------------------------
const int reedPin = 2;
const int ledPin = 13;
int reedState = 0;
int lastReedState = 0;


//-setup---------------------------------------------------
void setup(){
  //--pinmode
  pinMode(reedPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
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

  //--reed switch-------------------
  reedState = digitalRead(reedPin);

  if(reedState != lastReedState) {

    if(reedState == HIGH) {
      //bHornUp == false
      Serial.println("h0");
      
      //light for physical debugging: horn on desk, led on
      digitalWrite(ledPin, HIGH);
    } 
    else {
      //bHornUp = true
      Serial.println("h1");
      
      //light for physical debugging: horn in air, led off
      digitalWrite(ledPin, LOW);
    }
  }

  lastReedState = reedState;

}









