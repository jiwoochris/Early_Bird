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

#define C 262 // do   // make tones for a song
#define D 294 // re 
#define E 330 // mi
#define G 392 // sol
#define A 440 // la 
#define B 494 // ti

LiquidCrystal lcd(7,6,5,4,3,2); 

int notes[25] = { G, G, A, A, G, G, E, G, G, E, E, D, G, G, A, A, G, G, E, G, E, D, E, C }; // 학교종이 땡땡땡 song
int num = 0;
int i = 0;
int count1 = 0; //variables for counting the number that two stop buttons are pressed
int count2 = 0;
int hour_set; // hour of the alarm
int minute_set; // minute of the alarm

void set_alarm() { // set the alarm
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set your alarm"); // lcd display an announcement 'set your alarm'
  
  while(digitalRead(SET_BUTTON) == LOW){} // block until the set button is pressed
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Loading . . ."); // lcd displays an annuncement 'loading' 1 second.
  delay(1000);
  lcd.clear();

  while(digitalRead(SET_BUTTON) == LOW){ 
    hour_set = map(analogRead(resistorPin1),0,1023,23,0); // mapping the value of resistor to hour
    minute_set = map(analogRead(resistorPin2),0,1023,59,0);// mapping the value of resistor to minute

    print_time(0, 0, hour_set, minute_set); // lcd diplays alarm time
  }
}

void waiting_time() {// waiting time: before the alarm time
  int prev; // prev variable is like a lock. 
            //It makes lcd clear "current" time for displaying "set time" the time only that you press the set button
  lcd.clear();
  
  while (!(hour_set == hour() && minute_set == minute())) { // when current time is not alarm time 
                                                            // hour() returns current hour, minute() returns current minute
    if(digitalRead(SET_BUTTON) == LOW){ // lcd diplays current time basically
      if(prev == HIGH) 
        lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Current time ");
      
      print_time(0, 1, hour(), minute());// print current time in the lcd
      prev = LOW;
    }

    else{ // if you press the set button, lcd displays alarm time
      if(prev == LOW)
        lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Set time ");
      
      print_time(0, 1, hour_set, minute_set);// print alarm time in the lcd

      prev = HIGH;
    }
  }

}
void alarm_time(){// alarm time
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Time to Wake up!");
  lcd.setCursor(0, 1);
  lcd.print("It's ");
  
  print_time(5, 1, hour(), minute()); // prit alarm time in the lcd
  
  light_on(); // light comes on
  
  while (count2 < 3) { // when two stopp buttons are pressed 3 times each in rotation, the loop stop
    ringing();
    counting_stop_buttons();
  }
 }
void light_on(){// turn on the light
  digitalWrite(RELAY, HIGH);
}

void ringing() { // make buzzer sound song '학교종이 땡땡땡' and light blink
  if (hour() == hour_set && minute() == minute_set) {// when alarm time comes, buzzer sound the song '학교종이 땡땡땡' with normal speed
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

  else {// after 1 minute, the song faster 1.5 times and the light blinks
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

    digitalWrite(RELAY, i % 2); // light blinks
  }
}

void counting_stop_buttons() { // count the number that stop buttons are pressed
  
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

void reset(){// reset alarm 
  digitalWrite(RELAY, LOW); // turn off the light
  count1 = 0;
  count2 = 0;
}

void print_time(int init_x, int init_y, int hour, int minute){ // print time in the lcd
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

  setTime(23, 55, 0, 20, 11, 21); // set the start time //it's like a clock
}


void loop() {

  set_alarm();
  
  waiting_time();

  alarm_time();
  
  reset();
}
