#include <Servo.h>
#include <Keypad.h>

Servo ServoMotor;

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
  }
}

void unlockDoor() {
  ServoMotor.write(180);
  isLocked = false;
}

void lockDoor() {
  ServoMotor.write(0);
  isLocked = true;
}


void resetInput() {
  inputIndex = 0;
  memset(input, 0, sizeof(input));
}
