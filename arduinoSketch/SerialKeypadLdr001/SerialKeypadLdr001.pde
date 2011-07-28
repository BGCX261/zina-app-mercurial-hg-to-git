//----------------------------------------------------
#include <Keypad.h>

//-keypad---------------------------------------------
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {3, 8, 7, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 2, 6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//-ldr-------------------------------------------------
int ldrPin = A0;
int ldrValue = 0;
int ldrThreshold = 50;

int valuePrevious = 500;
int valueThreshold = 20;

boolean ldrBoolNew = false;
boolean ldrBoolPrevious = false;

//-setup-----------------------------------------------
void setup(){
  Serial.begin(9600);
}

//-loop------------------------------------------------  
void loop(){
  
  //--keypad
  char key = keypad.getKey();
  
  if (key != NO_KEY){
    Serial.print("k");
    Serial.println(key);
  }
  
  //--
 
  ldrValue = analogRead(ldrPin);
  
  if(ldrValue < valuePrevious - valueThreshold) {
    Serial.print("minus @");
    Serial.println(ldrValue);
    
    startWaiting();
  }
  
  else if (ldrValue > valuePrevious + valueThreshold) {
    Serial.print("plus @");
  }
  
  else {
    Serial.println(ldrValue);
  }
  
  delay(100);

  /*
  Serial.print(valuePrevious);
  Serial.print(" prev / new ");
  Serial.println(ldrValue);*/
 
  
  valuePrevious = ldrValue;
  
  
  
  
  /*
  if (ldrValue <= ldrThreshold) {
    Serial.print("l0");
    //ldrBoolNew = false;
  } else {
    Serial.print("l1");
     //ldrBoolNew = true;
  }
  */
  
  /*
  Serial.print(" @");
  Serial.print(ldrValue);
  
  for(int i = 0; i < ldrValue; i+=10) {
    Serial.print(".");
  }
  Serial.println("|");*/
  
  //ldrBoolPrevious = ldrBoolNew;
  
  
  
}
  
  
