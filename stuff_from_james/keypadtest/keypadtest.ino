#include <LiquidCrystal.h>

int x = 0;
int currx = 1023;
String btnStr = "None";

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Analog 0: ");
  lcd.print(currx);
  lcd.setCursor(0,1);
  lcd.print(btnStr);
}

void loop() {
  
  x = analogRead(A0); // the buttons are read from the analog0 pin
  
  // Check if x has changed
  if ((x != 1022) && (x != currx)){
  
    //update screen and change currx
    lcd.setCursor(10,0);
    lcd.print("     ");
    lcd.setCursor(10,0);
    lcd.print(x);
    currx = x;
    
    if (currx > 740 && currx < 745){
    
       btnStr="Select";
    
    } else if (currx > 400 && currx < 420){
    
      btnStr="Left";
      
    } else if (currx < 10){
    
      btnStr="Right";
      
    } else if (currx > 140 && currx < 150){
    
      btnStr="Up";
      
    } else if (currx > 320 && currx < 365){
    
      btnStr="Down";
      
    }
    
      //update button pressed
      lcd.setCursor(0,1);
      lcd.print("        ");
      lcd.setCursor(0,1);
      lcd.print(btnStr);
  }
  
  delay(10);
   
}
