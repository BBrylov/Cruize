#define STEP_PIN 16 //D0
#define DIR_PIN 5 //D1
#define BUZZER 4 // D2
#define DSTOP 0 //D3
#define DIG 2 //D4
#define LEDR 15 // D8
#define LEDB 12 // D6
#define LEDG 13 // D7
#define IR 14 // D5
#define DSPEED 10 //SD3
//#define NOSERIAL 1

int Index;

void setup() {
  // put your setup code here, to run once:
 pinMode(BUZZER, OUTPUT);
delay(500);
 digitalWrite(BUZZER,HIGH);



  pinMode( STEP_PIN, OUTPUT); //Step
  pinMode(DIR_PIN, OUTPUT); //Direction


}


void loop(){


  digitalWrite(DIR_PIN,HIGH);
 for(Index = 0; Index < 2000; Index++)
  {
    digitalWrite(STEP_PIN,HIGH);
    delayMicroseconds(500);
    digitalWrite(STEP_PIN,LOW);
    delayMicroseconds(500);
  }
  delay(1000);

digitalWrite(BUZZER,LOW);
delay(500);
 digitalWrite(BUZZER,HIGH);

  digitalWrite(DIR_PIN,LOW);
 for(Index = 0; Index < 2000; Index++)
  {
    digitalWrite(STEP_PIN,HIGH);
    delayMicroseconds(500);
    digitalWrite(STEP_PIN,LOW);
    delayMicroseconds(500);
  }
  delay(1000);
digitalWrite(BUZZER,LOW);
delay(500);
 digitalWrite(BUZZER,HIGH);

  
}

