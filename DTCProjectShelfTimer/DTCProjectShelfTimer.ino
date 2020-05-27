#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define bUp 2
#define bDo 4
#define bGo 3
#define bBa 5
#define pLED 6
char *matList[] = {"Plastic", "Paper", "Cardboard", "Metal"};
int timeList[] = {168, 72, 24, 168};
int cOpt = 0;
int changeThing = 0;
int timerOn = 0;
unsigned long timeGoal = 0;
unsigned long timeNow = 0;
int ledState = 0;
int startTime = 0;
int stopTime = 0;
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  Serial.begin(9600);
  lcd.init();                      // initialize the lcd
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Welcome to");
  lcd.setCursor(0, 1);
  lcd.print("ShelfTime!");
  pinMode(bUp, INPUT);
  pinMode(bDo, INPUT);
  pinMode(bGo, INPUT);
  pinMode(bBa, INPUT);
  pinMode(pLED, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(bUp), goUp, FALLING);
  attachInterrupt(digitalPinToInterrupt(bDo), goDown, FALLING);
  attachInterrupt(digitalPinToInterrupt(bGo), goSelect, FALLING);
  attachInterrupt(digitalPinToInterrupt(bBa), goBack, FALLING);
  delay(500);
  lcd.clear();
}


void loop()
{
  if (cOpt < 0) {
    cOpt = 4 - cOpt;
  }
  else if (cOpt > 3) {
    cOpt = cOpt - 4;
  }
  digitalWrite(pLED, ledState);
  wait(1000);
}

void goUp() {
  //cOpt++;
  changeThing = 1;
}

void goDown() {
  //cOpt--;
  //changeThing = 1;
}

void goSelect() {
  startTime = 1;
}

void goBack() {
  //stopTime = 1;
}

void wait(int del) {
  int newTime = millis();
  showOption(cOpt);
  if (timerOn == 1) {
    lcd.setCursor(10, 1);
    lcd.print((int)((millis()-timeNow)/1000));
    //lcd.print("TEST");
    if (millis() - timeNow > timeGoal) {
      ledState = 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Done!!!");
      delay(1000);
      timerOn = 0;
    }
  }
  while (millis() - newTime < del) {
    if (timerOn == 1) {
      Serial.println("waiting...");
    }
  }
}

void showOption(int opt) {
  if (changeThing == 1) {
    ledState = 0;
    cOpt++;
    opt++;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(matList[opt]);
    lcd.setCursor(0, 1);
    lcd.print(timeList[opt]);
    changeThing = 0;
  }
  if (startTime == 1) {
    timeGoal = timeList[opt] * 1000;
    timerOn = 1;
    timeNow = millis();
    startTime = 0;
  }
}
