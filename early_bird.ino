#include <Time.h>
#include <TimeLib.h>
#include <LiquidCrystal.h>

const int resistorPin1 = A0;  
const int resistorPin2 = A1;

#define SET_BUTTON 8
#define BUZZER 9
#define BUTTON1 10
#define BUTTON2 11
#define RELAY 12
#define LED 13

#define C 262 // 도 
#define D 294 // 레 
#define E 330 // 미
#define G 392 // 솔 
#define A 440 // 라 
#define B 494 // 시

LiquidCrystal lcd(7,6,5,4,3,2);

int notes[25] = { G, G, A, A, G, G, E, G, G, E, E, D, G, G, A, A, G, G, E, G, E, D, E, C };
int num = 0;

void setup() {
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(resistorPin1, INPUT); 
  pinMode(resistorPin2, INPUT);
  pinMode(SET_BUTTON, INPUT);
  
  lcd.begin(16,2); 

  setTime(23, 55, 0, 20, 11, 21);
}

int i = 0;
int count1 = 0;
int count2 = 0;
int hour_set = NULL;
int minute_set = NULL;

void set_alarm() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set your alarm");
  
  while(digitalRead(SET_BUTTON) == LOW){}
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Loading . . .");
  delay(1000);
  lcd.clear();

  while(digitalRead(SET_BUTTON) == LOW){
    hour_set = analogRead(resistorPin1);
    hour_set = map(hour_set,0,1023,23,0);
    lcd.setCursor(0,0);
    if(hour_set<10){
      lcd.print('0');
      lcd.setCursor(1,0);
    }
    lcd.print(hour_set);

    lcd.setCursor(2,0);
    lcd.print(":");

    minute_set = analogRead(resistorPin2);  
    minute_set = map(minute_set,0,1023,59,0);
    lcd.setCursor(3,0);
    lcd.print(minute_set);
  }
}

void wait_to_time() {
  int prev;
  lcd.clear();
  
  while (!(hour_set == hour() && minute_set == minute())) {
    if(digitalRead(SET_BUTTON) == LOW){
      if(prev == HIGH)
        lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Current time ");
      lcd.setCursor(0,1);
  
      
      if(hour()<10){
        lcd.print('0');
        lcd.setCursor(1,1);
      }
      lcd.print(hour());
  
      lcd.setCursor(2,1);
      lcd.print(":");
      
      lcd.setCursor(3,1);
      lcd.print(minute());
      prev = LOW;
    }

    else{
      if(prev == LOW)
        lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Set time ");
      lcd.setCursor(0,1);
  
      
      if(hour()<10){
        lcd.print('0');
        lcd.setCursor(1,1);
      }
      lcd.print(hour_set);
  
      lcd.setCursor(2,1);
      lcd.print(":");
      
      lcd.setCursor(3,1);
      lcd.print(minute_set);

      prev = HIGH;
    }
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Time to Wake up!");
  lcd.setCursor(0, 1);
  lcd.print("It's ");
  lcd.setCursor(5, 1);
  if(hour()<10){
      lcd.print('0');
      lcd.setCursor(1,0);
    }
  lcd.print(hour());
  lcd.setCursor(7, 1);
  lcd.print(":");  
  lcd.setCursor(8, 1);
  lcd.print(minute());
}

void ringing() {
  if (hour() == hour_set && minute() == minute_set) {
    tone (BUZZER, notes[ i ], 400);
    
    if (i == 6 || i == 18)
      delay(800);
    else if (i == 11)
      delay(1600);
    else if (i == 24){
      delay(1600);
      i = 0;
    }
    else
      delay(400);
    i++;
  }

  else {
    tone (BUZZER, notes[ i ], 200);
    
    if (i == 6 || i == 18)
      delay(400);
    else if (i == 11)
      delay(800);
    else if (i == 24){
      delay(800);
      i = 0;
    }
    else
      delay(200);
    i++;

    digitalWrite(RELAY, i % 2);
  }
}

void turn_off() {
  
  if (digitalRead(BUTTON1) == HIGH) {
    count1 = 1;
    switch (count2) {
      case (1): count1 = 2; break;
      case (2): count1 = 3;
    }
  }

  if (digitalRead(BUTTON2) == HIGH) {
    switch (count1) {
      case (1): count2 = 1; break;
      case (2): count2 = 2; break;
      case (3): count2 = 3;
    }
  }
}

void reset(){
  digitalWrite(RELAY, LOW);
  count1 = 0;
  count2 = 0;
}

void loop() {

  set_alarm();
  
  wait_to_time();

  digitalWrite(RELAY, HIGH);
  while (count2 < 3) {
    ringing();
    turn_off();
  }

  reset();
}
