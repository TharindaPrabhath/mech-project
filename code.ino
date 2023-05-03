#include <LiquidCrystal.h>
#include <Key.h>
#include <Keypad.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);  //add pins conncted to PWM/digital

byte rowPins[4] = { 53, 51, 49, 47 };  //digital pins conncted to rows of keypad
byte colPins[4] = { 45, 43, 41, 39 };  //digital pins conncted to columns of keypad

int row = 0;
int column = 0;

byte curcorLoc[2] = { 0, 0 };

char keys[4][4] = {  //key identifying in 2D array
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

char welcomeMessage[16] = { '.', 'W', 'E', 'L', 'C', 'O', 'M', 'E', ' ', 'T', 'O', ' ', 'U', 'o', 'M', '.' };
char libraryBook[16] = { '.', '.', 'L', 'I', 'B', 'R', 'A', 'R', 'Y', ' ', 'B', 'O', 'O', 'K', '.', '.' };
char storer[16] = { '.', '.', '.', '.', '.', 'S', 'T', 'O', 'R', 'E', 'R', '.', '.', '.', '.', '.' };

Keypad inputs = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);  //keypad object making
// 2D array,Row pins, Column pins, No of rows, No of columns


void setup() {
  lcd.begin(16, 4);  //size of LCD
  starting();
}

void loop() {
  // put your main code here, to run repeatedly:
  char key = inputs.getKey();
  keyAction(key);
}


/*
Custom Functions
*/

// Startup Display
void starting() {
  char keySwitchOnStart = inputs.getKey();
  if (keySwitchOnStart != NO_KEY) {
    for (int a = 0; a < 16; a++) {
      lcd.setCursor(a, 0);
      lcd.print(welcomeMessage[a]);
      delay(100);
    }

    for (int a = 0; a < 16; a++) {
      lcd.setCursor(a, 1);
      lcd.print(libraryBook[a]);
      delay(100);
    }

    for (int a = 0; a < 16; a++) {
      lcd.setCursor(a, 2);
      lcd.print(storer[a]);
      delay(100);
    }

    for (int x = 0; x < 15; x++) {
      lcd.setCursor(x, 3);
      lcd.print("-");
      delay(50);
      if (x != 15) {
        lcd.print(">");
        delay(50);
      }
    }

    delay(2500);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Row: ");
    lcd.setCursor(0, 1);
    lcd.print("Enter Column: ");
    lcd.setCursor(11, 0);
    lcd.blink();
  }
}

// Getting user inputs and displaying

void keyAction(char key) {
  switch (key) {
    case 0:
      //do nothing
      break;
    case 'D':
      if (row != 0 && column == 0) {
        lcd.setCursor(8, 1);
        cursorLoc[0] = 8;
        cursorLoc[1] = 1;
      } else if (row != 0 && column != 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Adding Book to");
        lcd.setCursor(0, 1);
        lcd.print(row);
        lcd.print(" Row ");
        lcd.print(column);
        lcd.print(" Column..");
      }
      break;
    case 'C':
      startup();
      row = 0;
      column = 0;
      break;
    case 'B':
      if (cursorLoc[1] == 0) {
        startup();
        row = 0;
      } else {
        startup();
        column = 0;
        lcd.print(row);
        lcd.setCursor(8, 1);
        cursorLoc[0] = 8;
        cursorLoc[1] = 1;
      }
      break;
    default:
      lcd.print(key);
      if (cursorLoc[1] == 0) {
        row = key - '0';
      } else {
        column = key - '0';
      }
      //Serial.println(row+','+column);
      break;
  }
}
