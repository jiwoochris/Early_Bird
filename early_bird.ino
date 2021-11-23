#include <Time.h>
#include <TimeLib.h>
#include <LiquidCrystal.h> 

const int resistorPin1 = A0;  
const int resistorPin2 = A1;

#define SET_BUTTON 8 // define pin numbers as english names for code readbility
#define BUZZER 9
#define BUTTON1 10
#define BUTTON2 11
#define RELAY 12
#define LED 13

#define C 262 // 도   // make tones for a song
#define D 294 // 레 
#define E 330 // 미
#define G 392 // 솔 
#define A 440 // 라 
#define B 494 // 시

LiquidCrystal lcd(7,6,5,4,3,2); 

int notes[25] = { G, G, A, A, G, G, E, G, G, E, E, D, G, G, A, A, G, G, E, G, E, D, E, C }; // 학교종이 땡땡땡 song
int num = 0;
int i = 0;
int count1 = 0; //for counting pressed numbers of two stop buttons
int count2 = 0;
int hour_set; // hour of the alarm
int minute_set; // minute of the alarm

void set_alarm() { // setting the alarm
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set your alarm"); // lcd prints announcement 'set your alarm'
  
  while(digitalRead(SET_BUTTON) == LOW){} // when set button isn't pressed
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Loading . . ."); // lcd prints annuncement 'loading' 1 second.
  delay(1000);
  lcd.clear();

  while(digitalRead(SET_BUTTON) == LOW){ // when set button isn't pressed
    hour_set = map(analogRead(resistorPin1),0,1023,23,0); // mapping the value of resistor to hours
    minute_set = map(analogRead(resistorPin2),0,1023,59,0);

    print_time(0, 0, hour_set, minute_set);
  }
}

void waiting_time() {
  int prev;
  lcd.clear();
  
  while (!(hour_set == hour() && minute_set == minute())) {
    if(digitalRead(SET_BUTTON) == LOW){
      if(prev == HIGH) // 어떤 기능?
        lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Current time ");
      
      print_time(0, 1, hour(), minute()); // hour(), minute() 어떤 library 사용? 정확히 어떤 의미// hour(), minute() 어떤 library 사용? 정확히 어떤 의미
      prev = LOW;
    }

    else{
      if(prev == LOW)
        lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Set time ");
      
      print_time(0, 1, hour_set, minute_set);

      prev = HIGH;
    }
  }

}
void alarm_time(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Time to Wake up!");
  lcd.setCursor(0, 1);
  lcd.print("It's ");
  
  print_time(5, 1, hour(), minute()); 
  
  light_on();
  
  while (count2 < 3) {
    ringing();
    counting_stop_buttons();
  }
 }
void light_on(){
  digitalWrite(RELAY, HIGH);
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

void counting_stop_buttons() {
  
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

void print_time(int init_x, int init_y, int hour, int minute){
  lcd.setCursor(init_x, init_y);
  if(hour<10){
    lcd.print('0');
    lcd.setCursor(init_x+1, init_y);
  }
  lcd.print(hour);

  lcd.setCursor(init_x+2, init_y);
  lcd.print(":");

  lcd.setCursor(init_x+3, init_y);
  if(minute<10){
    lcd.print('0');
    lcd.setCursor(init_x+4, init_y);
  }
  lcd.print(minute);
}

void setup() {
  pinMode(RELAY, OUTPUT); // pin setting
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(resistorPin1, INPUT); 
  pinMode(resistorPin2, INPUT);
  pinMode(SET_BUTTON, INPUT);
  
  lcd.begin(16,2); 

  setTime(23, 55, 0, 20, 11, 21); // set the current time by yourself // 각 인자가 무엇을 의미하는 지 설명, library와 연결해서 함수 설명
}


void loop() {

  set_alarm();
  
  waiting_time();

  alarm_time(); // light, buzzer, stop button occurs
  
  reset();
}
