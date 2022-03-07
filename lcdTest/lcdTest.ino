// include the library code:
#include <LiquidCrystal.h>

//RS --> GPIO33
//E --> GPIO32
//D7 --> GPIO25
//D6 --> GPIO26
//D5 --> GPIO27
//D4 --> GPIO19

//LED1 --> GPIO15
//LED2 --> GPIO4
//LED3 --> GPIO16
//LED4 --> GPIO17
//LED5 --> GPIO18

// initialize the library with the numbers of the interface pins
// Create An LCD Object. Signals: [ RS, EN, D4, D5, D6, D7 ]
LiquidCrystal lcd(33, 32, 19, 27, 26, 25);
 
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("circuitschools.");
}
 
void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
}
