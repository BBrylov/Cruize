
#include "beep.h"
#include "beep_cfg.h"

uint8_t TBeep::Start(uint8_t Pin_num)
{
  if (Pin_num>20) {
    PinN=-1;
    return 0;
  }
  PinN= Pin_num;
  Event=Idle;
  state=0;
  pinMode(PinN, OUTPUT);
  digitalWrite(PinN,HIGH); 
};

uint8_t  TBeep::SetBeep(char *Beep_com)
{
  if (PinN<0) return 0;
  pointer=0;
  strncpy(command, Beep_com,sizeof(command)-1);
  command[sizeof(command)-1]='\0';
  Event=Message;
};

uint8_t  TBeep::SetBeep(const __FlashStringHelper *Beep_com)
{
  if (PinN<0) return 0;
  pointer=0;
  strncpy_P(command, (PGM_P)Beep_com,sizeof(command)-1);
  command[sizeof(command)-1]='\0';
  Event=Message;
  
};
uint8_t  TBeep::Loop(void)
{
  static uint32_t lastime;
 
  uint8_t old_sate=state;
  int32_t time_now;
  if (PinN<0) return 0; 
 switch (state){
  case 0: //Ждем посылки
  if (Event != Message) break;
  //if (sizeof(command)) break;
  if (command[pointer]!='S' and command[pointer]!='L') break;
  lastime=millis();
  digitalWrite(PinN,LOW);
  state=1; 

  break;
  case 1: //включили звук ждем таймоут
  time_now=millis()-lastime;
    switch (command[pointer])
      {
      case 'L':
      time_now-=TimeOut_Long;
      break;
      default: 
      time_now-=TimeOut_Shot;
      break;
      } 
    if (time_now>0) 
      {
        Event=Timeout;
        state=2;
        lastime=millis();
        digitalWrite(PinN,HIGH);
      }
    break;  
    case 2://выключили звук ждем паузу
      time_now=millis()-lastime-TimeOut_Loop;
      if (time_now>0)
        {
          pointer++;
/*          if (sizeof(command[pointer])==pointer) 
            {
              state=0;
              break;
            } 
*/
           if (command[pointer]!='S' and command[pointer]!='L') 
            {
              state=0;
              break;
            }  
            lastime=millis();
            digitalWrite(PinN,LOW);
            state=1;   
        }
    break;
  default:
   state=0;
  break;  
 }
  
};
