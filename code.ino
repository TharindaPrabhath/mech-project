#include <LiquidCrystal.h>
#include <Key.h>
#include <Keypad.h>

//constants
const byte ROW_CURSOR_LOC[2] = { 2, 5 };    //(row,col)
const byte COLUMN_CURSOR_LOC[2] = { 3, 8 };   //(row,col)

//different states programme goes through
const int OFF = 0;
const int WAITING_FOR_ROW_NO = 1;
const int WAITING_FOR_COL_NO = 2;
const int ADDING_BOOK_TO_SHELF = 3;

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);  //add pins conncted to PWM/digital

byte colPins[4] = { A3, A2, A1, A0 };  //digital pins conncted to rows of keypad
byte rowPins[4] = { 5, 4, 3, 2 };  //digital pins conncted to columns of keypad

//Store row and cloumn numbers users have entered
byte bookLoc[2] = { 0, 0 };

int cursorLoc[2] = { 0, 0 };  //store location of cursor {row,column}
int status = OFF;              //keep track of the program stage

char keys[4][4] = {  //key identifying in 2D array
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

/*
char welcomeMessage[16] = { '.', 'W', 'E', 'L', 'C', 'O', 'M', 'E', ' ', 'T', 'O', ' ', 'U', 'o', 'M', '.' };
char libraryBook[16] = { '.', '.', 'L', 'I', 'B', 'R', 'A', 'R', 'Y', ' ', 'B', 'O', 'O', 'K', '.', '.' };
char storer[16] = { '.', '.', '.', '.', '.', 'S', 'T', 'O', 'R', 'E', 'R', '.', '.', '.', '.', '.' };
*/

//Welcome MEssage
const String welcomeMessage_Line_1 = ".WELCOME TO UOM.";
const String welcomeMessage_Line_2 = "..LIBRARY BOOK..";
const String welcomeMessage_Line_3 = "....STOCKER.....";

Keypad inputs = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);  //keypad object making
// 2D array,Row pins, Column pins, No of rows, No of columns


void setup() {
	lcd.begin(20, 4);  //size of LCD
	Serial.begin(9600);
}

void loop() {
	char key = inputs.getKey();
	if (key != 0) {
		Serial.println(key);
	}
	keyAction(key);
}


/*
-------------------
	Functions
-------------------
*/

// Startup Display
void showWelcomeMessage() {
	for (int a = 0; a < 16; a++) {
		lcd.setCursor(a, 0);
		lcd.print(welcomeMessage_Line_1[a]);
		delay(100);
	}

	for (int a = 0; a < 16; a++) {
		lcd.setCursor(a, 1);
		lcd.print(welcomeMessage_Line_2[a]);
		delay(100);
	}

	for (int a = 0; a < 16; a++) {
		lcd.setCursor(a, 2);
		lcd.print(welcomeMessage_Line_3[a]);
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
	reset();
}

void reset() {
	lcd.clear();
	setCursorLoc(0, 0);
	lcd.print("Enter Location");
	setCursorLoc(1, 0);
	lcd.print("of the Book");
	setCursorLoc(ROW_CURSOR_LOC[0], 0);
	lcd.print("Row: ");
	setCursorLoc(COLUMN_CURSOR_LOC[0], 0);
	lcd.print("Column: ");
	setCursorLoc(ROW_CURSOR_LOC[0], ROW_CURSOR_LOC[1]);
	lcd.blink();

	status = WAITING_FOR_ROW_NO;
}

// Handling Keypad and LCD
void keyAction(char key) {
	switch (key) {
	case 0:
		//do nothing
		break;
	case 'D':
		if (status == WAITING_FOR_ROW_NO) {
			setCursorLoc(COLUMN_CURSOR_LOC[0], COLUMN_CURSOR_LOC[1]);
			bookLoc[0] == (key - '0');
			status = WAITING_FOR_COL_NO;
		}
		else if (status == WAITING_FOR_COL_NO) {
			lcd.clear();
			setCursorLoc(0, 0);
			lcd.print("Adding Book to");
			lcd.setCursor(0, 1);
			lcd.print(bookLoc[0]);
			lcd.print(" Row ");
			lcd.print(bookLoc[1]);
			lcd.print(" Column..");
			lcd.setCursor(0, 3);
			lcd.print("Press A to stop");
			status = ADDING_BOOK_TO_SHELF;

			//call the function to move book to the location
		}
		break;
	case 'C':
		if (status == WAITING_FOR_ROW_NO || status == WAITING_FOR_COL_NO) {
			reset();
			bookLoc[0] = 0;
			bookLoc[1] = 0;
		}
		break;
	case 'B':
		if (status == WAITING_FOR_ROW_NO) {
			reset();
			bookLoc[0] = 0;
		}
		else if (status == WAITING_FOR_COL_NO) {
			reset();
			bookLoc[1] = 0;
			lcd.print(bookLoc[0]);
			status = WAITING_FOR_COL_NO;
			setCursorLoc(COLUMN_CURSOR_LOC[0], COLUMN_CURSOR_LOC[1]);
		}
		break;
	case 'A':
		if (status == OFF) {
			//turn on the display
			showWelcomeMessage();
		}
		else if (status == ADDING_BOOK_TO_SHELF) {
			//cancel book adding process
			reset();
			bookLoc[0] = 0;
			bookLoc[1] = 0;
		}
		else {
			//turning off
			lcd.clear();
			lcd.noBlink();
			bookLoc[0] = 0;
			bookLoc[1] = 0;
			status = OFF;
		}
	default:
		if ((key - '0') > 3) {  //numbers larger than 3 won't be entered
			break;
		}
		else {
			if (status == WAITING_FOR_ROW_NO) {
				if (bookLoc[0] == 0) {
					bookLoc[0] = key - '0';
					lcd.print(key);
				}
			}
			else if (status == WAITING_FOR_COL_NO) {
				if (bookLoc[1] == 0) {
					bookLoc[1] = key - '0';
					lcd.print(key);
				}
			}
		}
		lcd.blink();
		break;
	}
}

//Function to set cursor location and save it on memoryvoid
void setCursorLoc(int row, int col) {
	lcd.setCursor(col, row);
	cursorLoc[0] = row;
	cursorLoc[1] = col;
}