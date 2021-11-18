#define BUZZER 8
#define BUTTON1 6
#define BUTTON2 7
#define RELAY 3 

#define C 262 // 도 
#define D 294 // 레 
#define E 330 // 미 
#define F 349 // 파 
#define G 392 // 솔 
#define A 440 // 라 
#define B 494 // 시

int notes[25] = { G, G, A, A, G, G, E, G, G, E, E, D, G, G, A, A, G, G, E, G, E, D, E, C };
int num = 0;
void setup(){
  Serial.begin(9600);
  pinMode(RELAY,OUTPUT); 
  pinMode(BUZZER,OUTPUT);
  pinMode(BUTTON1,INPUT);
  pinMode(BUTTON2,INPUT); 
} 

int i =0;
int count1 =0;
int count2 =0;

void loop(){ 
  digitalWrite(RELAY,HIGH); 
  while(count2<3){
    tone (BUZZER, notes[ i ], 200);
    delay(800);
    i++;
    if(i == 12)
      delay(200);
    if(i==24)
      i=0;
      delay(200);

      
    if(digitalRead(BUTTON1)==HIGH){
      count1 = 1;
      switch(count2){
        case(1): count1 =2; break;
        case(2): count1 =3;
    }
  }
    if(digitalRead(BUTTON2)==HIGH){
       switch(count1){
         case(1): count2 =1; break;
         case(2): count2 =2; break;
         case(3): count2 =3;
       }
    }
    Serial.println(count1);
    Serial.println(count2);
  }
  digitalWrite(RELAY,LOW);
  exit(0);
}
