/* @file HelloKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates the simplest use of the matrix Keypad library.
|| #

keypad mapping order by arne boon @ 21-01-2011 for Zina
rows: 2, 7, 6, 4
cols: 3, 1, 5

than add number to correspond with pinouts on hardware (in this case pins 2-8 are used)
rows: 3, 8, 7, 5
cols: 4, 2, 6

@2011-02-02
rows: 4, 11, 10, 8
cols: 5, 3, 9

*/
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
//byte rowPins[ROWS] = {3, 8, 7, 5}; //connect to the row pinouts of the keypad
//byte colPins[COLS] = {4, 2, 6}; //connect to the column pinouts of the keypad

byte rowPins[ROWS] = {4, 11, 10, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 3, 9}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(9600);
}
  
void loop(){
  char key = keypad.getKey();
  
  if (key != NO_KEY){
    Serial.print(key);
  }
}
