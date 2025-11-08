#include <Wire.h>
#include <Servo.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

Servo ServoMotor;
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* password = "567";  
char input[4]; 
byte inputIndex = 0;

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};
byte rowPins[ROWS] = { 9, 8, 7, 6 }; 
byte colPins[COLS] = { 5, 4, 3 }; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int SensorPin = 10;
int BuzzerPin = 12;
bool isLocked = true;

void setup() {
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0); // column, row
  lcd.print("hello nakul bhai");
  lcd.setCursor(0, 1);
  lcd.print("kaise ho aap?");

  pinMode(SensorPin, INPUT);
  pinMode(BuzzerPin, OUTPUT);
  ServoMotor.attach(11);
  Serial.begin(9600); 
  lockDoor();
}

void loop() {
  // Serial.println("Hello, Arduino!"); 
  char key = keypad.getKey();
  if (key) {
    Serial.println(key);
  }
  if (key) {
    if (key == '*') {
      resetInput();
      lockDoor();
    } else {
      input[inputIndex] = key;
      inputIndex++;
    }
  }

  if (inputIndex == 3) {       
    input[inputIndex] = '\0'; 
    if (strcmp(input, password) == 0) {
      unlockDoor();
      resetInput();
    } else {
      digitalWrite(BuzzerPin, HIGH); delay(500);
      digitalWrite(BuzzerPin, LOW);  delay(50);
      resetInput();
      lockDoor();
    }
  }
  if (isLocked && digitalRead(SensorPin) == LOW) {
    digitalWrite(BuzzerPin, HIGH);
    delay(50);
    digitalWrite(BuzzerPin, LOW);
    delay(50);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Intruder");
    lcd.setCursor(0, 1);
    lcd.print("Detected!");
  }
}

void unlockDoor() {
  ServoMotor.write(0);
  isLocked = false;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Correct Password");
  lcd.setCursor(0, 1);
  lcd.print("Door unlocked!");
}

void lockDoor() {
  ServoMotor.write(180);
  isLocked = true;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Security System");
  lcd.setCursor(0, 1);
  lcd.print("Activated!");
}


void resetInput() {
  inputIndex = 0;
  memset(input, 0, sizeof(input));
}
