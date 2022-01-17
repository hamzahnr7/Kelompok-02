#include <Servo.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
// Deklarasi Servo
Servo myservo;
void unlock();

// Deklarasi Keypad
const byte Rows = 4;
const byte Cols = 4;

char keyMap[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[4] = {9, 8, 7, 6}; 
byte colPins[4] = {5, 4, 3, 2};

Keypad myKeypad = Keypad(makeKeymap(keyMap), rowPins, colPins, Rows, Cols); 

// Deklarasi LCD
LiquidCrystal lcd(A5, A4, A3, 13, 12, 11);
// void lcdInit(), lcdUnlocked(); lcdInputKey();

// Deklarasi Pin Buzzer dan Void(Fungsi)
byte buzzPin = A2; 
void Bip1(), Bip2();

// Deklarasi Button
byte button = A1;

// Deklarasi Sensor PIR
byte pirPin = A0;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(buzzPin, OUTPUT);
  pinMode(button, INPUT);
  pinMode(pirPin, INPUT);
  myservo.attach(10);
  myservo.write(0);
  
}

bool en = false;
int i = 0;
int stat = 0;
char key[6] = {'1', '1', '0', '3', '2', '1'};
char inputKey[6];

void loop()
{
  char keypressed = myKeypad.getKey();
  bool getPir = digitalRead(pirPin);

  if (digitalRead(button) == HIGH)
  {
    unlock();
    delay(1000);
  }
  
  if ( getPir == HIGH)
  {
    en = true;
    lcdInit();
  }
  else if (millis()/1000 % 5 == 0){
    // Serial.println(millis()/1000);
    lcd.clear(); 
    en = false;
    for (int k = 0; k < 6; k++) inputKey[k] = ' ';
  }
  
  if (keypressed != NO_KEY && en)
  {
    inputKey[i] = keypressed;
    lcdInputKey();
    //lcd.print(keypressed);
    if (i == 5)
    {
      for (int j = 0; j < 6; j++)
      {
        Serial.print(inputKey[j]);
        if (inputKey[j] == key[j])
        {
          stat++;
        }
        else
        {
          stat=0;
          break;
        }
      }
      if (stat == 6) 
      {
        lcdUnlocked();
        Bip1(); stat = 0; i = -1;
        unlock();
  		delay(2000);
        for (int k = 0; k < 6; k++) inputKey[k] = ' ';
  		lcd.clear(); // lcdInit();
      }
      else {
        for (int k = 0; k < 6; k++) inputKey[k] = ' ';
        Serial.print('Wrong Password');
        lcdFalseKey();
        Bip2(); stat = 0; i = -1;
  		delay(2000);
  		lcd.clear(); lcdInit();
      }
    }
    i++;
  }
}

void Bip1(){
  digitalWrite(buzzPin, HIGH);
  delay(500);
  digitalWrite(buzzPin, LOW);
//  delay(500);
}

void Bip2(){
  digitalWrite(buzzPin, HIGH);
  delay(500);
  digitalWrite(buzzPin, LOW);
  delay(250);
  digitalWrite(buzzPin, HIGH);
  delay(500);
  digitalWrite(buzzPin, LOW);
//  delay(250);
}

void lcdInit(){
//  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Input Key :");
  lcd.setCursor(0,1);
}
void lcdUnlocked(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Unlocking...");
  lcd.setCursor(0,1);
  lcd.print("Open the Door");
}
void lcdFalseKey(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Wrong Key");
  lcd.setCursor(0,1);
  lcd.print("Input Key Again!");
}
void lcdInputKey(){
  for (int i = 0; i < 6; i++){
    lcd.setCursor(i,1);
    if (inputKey[i] != ' '){
      lcd.print('*');
    }
    else lcd.print(' ');
  }
}

void unlock(){
  myservo.write(90);
  delay(3000);
  myservo.write(0);
}