#include <Time.h>
#include <TimeLib.h>

#define BUZZER 8
#define BUTTON1 6
#define BUTTON2 7
#define RELAY 3

#define C 262 // 도 
#define D 294 // 레 
#define E 330 // 미
#define G 392 // 솔 
#define A 440 // 라 
#define B 494 // 시

int notes[25] = { G, G, A, A, G, G, E, G, G, E, E, D, G, G, A, A, G, G, E, G, E, D, E, C };
int num = 0;

void setup() {
  Serial.begin(9600);
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);

  setTime(16, 0, 0, 17, 11, 21);
}

int i = 0;
int count1 = 0;
int count2 = 0;
int hour_set;
int minute_set;

bool set_alarm() {
  String s;

  Serial.println("Set your alarm (ex: 9:00)");

  if (Serial.available() > 0) {
    s = Serial.readString();
    if (s != NULL) {
      s.trim();
      int split = s.indexOf(":");
      hour_set =  s.substring(0, split).toInt();
      minute_set = s.substring(split + 1, s.length()).toInt();
    }
  }
  return true;
}

void wait_to_time() {
  while (hour_set != hour() && minute_set != minute()) {}

  Serial.print("It's ");
  Serial.print(hour());
  Serial.print(" : ");
  Serial.print(minute());
  Serial.println("\nTime to Wake up!!");
}

void ringing() {
  if (hour() == hour_set && minute() == minute_set) {
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
  }

  else {
    tone (BUZZER, notes[ i ], 100);
    
    if (i == 6 || i == 18)
      delay(200);
    else if (i == 11)
      delay(400);
    else if (i == 24){
      delay(400);
      i = 0;
    }
    else
      delay(100);
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

void loop() {

  while (!set_alarm()) {
    Serial.println("[Error] Usage Ex) 13:00. Try it again");
  }
  Serial.println("The alarm is set successfully");

  wait_to_time();

  digitalWrite(RELAY, HIGH);
  while (count2 < 3) {
    ringing();
    turn_off();
  }
  digitalWrite(RELAY, LOW);
}
