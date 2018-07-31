#include "Elm327.h"
#include <uart.h>
#include <Arduino.h>
#include <HardwareSerial.h>
#include <Esp.h>
#include <String.h>
#include <pgmspace.h>


//Комманды для Elm327
char ate0[] PROGMEM = "ATE0";
char atz[] PROGMEM = "ATZ";
 char ath0[] PROGMEM = "ATH0";
 char atd[] PROGMEM = "ATD";
 char ati[] PROGMEM = "ATI";
 char atib10[] PROGMEM = "ATIB10";
 char atsh6410f5[] PROGMEM = "ATSH6410F5";
 char atl0[] PROGMEM = "ATL0";
 char atst40[] PROGMEM = "ATST40";
 char atsp3[] PROGMEM = "ATSP3";
 char odb_speed[] PROGMEM = "22174A1";

//Строка инициализации обмена с Elm327
 char *init_elm[] PROGMEM ={ atz, ate0, ath0, atl0, atsp3, atst40 , atib10, atsh6410f5,  NULL};





//Конструктор
TElm327::TElm327(int8_t uart_nr): HardwareSerial(uart_nr) {};

int8_t TElm327::Loop(void){
  
}

unsigned int TElm327::hexToDec(char *hexString) {
  unsigned int decValue = 0;
  int nextInt;
  
  for (int i = 0; i < 1; i++) {
    
    nextInt = int(hexString[i]);
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);
    decValue = (decValue * 16) + nextInt;
  }
  return decValue;
}



int8_t TElm327::SetupElm(unsigned long baud)
{
  this->begin(baud);
  this->println();
  //this->println("Init Elm327 ..." );
  delayMicroseconds(500);
  int8_t myError = 0;
  for (uint8_t i = 0; init_elm[i]; i++)  {
    Send_Mess(init_elm[i]);
      while (LoopSerial() == Elm_busy);
//       this->print("Error ");this->println(Elm_state());
      if (Elm_state() > 0 ){
//        this->print("Error found " );this->println(Elm_state() );
        myError++;
        break;
      }
  }
  
  if (myError)  {
    FErrorElm = 1;
    //this->println("Init Elm327 Error." );
  }else {//this->println("Init Elm327 finish." )
  ;}
  return (myError);
}

int8_t TElm327::TX_Finish(void) {
  if (_uart == NULL || !(uart_tx_enabled(_uart))) {
    return 0;
  }
  return  ((USS(_uart_nr) >> USTXC) & 0xff);
}

int8_t TElm327::LoopSerial(void) {
  static uint32_t lastime;
  //int32_t time_now;
  char *_pointer;
  switch (state) {
    case 0: //Ждем посылки
      if (this->available()) this->read();
      if (Event != Message) break;
      lastime = millis();
      this->println(command);
      state = 1;
      Error = -6;
      break;
    case 1://Ждем окончания передачи пакета
      if (millis() - lastime > Send_Timeout) {
        Error = Err_Send_Timeout;
        state = 0;
        break;
      }
      if (this->TX_Finish()) {
        break;
      }
      else {
        state = 2;
        pointer = 0;
        Recive_Buf[pointer] = '\0';
        break;
      }
      break;
    case 2:// получаем данные
    
    
      while (this->available()) {
        
        if (pointer > 9) {
          Error = Err_ManyReciv;
          state = 0;
          break;
        }
        Recive_Buf[pointer] = this->read();
        if ( Recive_Buf[pointer] == '>') { // посылка от Elm327 закончена
          state = 3; //переходим в анализ ответа от Elm327
          Recive_Buf[pointer + 1] = '\0';
        }
        if ( Recive_Buf[pointer] == '\r') { // посылка от авто закончена
          state = 4; // переходим в анализ данных от автомобиля
          Recive_Buf[pointer + 1] = '\0';
        }
        pointer++;
      }
      if (millis() - lastime > Reciv_Timeout) {
        Error = Err_Reciv_Timeout;
        state = 0;
        break;
      }
      break;
    case 3://разбор ответа от Elm327
    {
//      this->print("Recive ELM - " );this->println(Recive_Buf );
     char *_pointer=strchr( Recive_Buf,'O' ) ;
     if (!_pointer) {Error = Err_Reciv_Elm_Mess; state=0; break;}
      if (*(_pointer+1)=='K')  {Error=Elm_OK;state=0;break;}
    }
      break;
    case 4://разбор от автомобиля
     {
//     this->print("Recive Avto - " );this->println(Recive_Buf );
      char *_pointer=strchr( Recive_Buf,'\r' ) ;
      if (!_pointer) {Error = Err_Reciv_Avto_Mess; state=0; break;}
     Error = Elm_OK; state=0; break;
     pointer=0;
     for(uint8_t i=0; i< sizeof(Recive_Buf); i=i+2){
      Recive_hex[i/2]=hexToDec(Recive_Buf[i]);
     }
     }
     

      break;
    default:
      state = 0;
      Error = Elm_OK;
      break;
  }
  return Error;
}


void TElm327::Send_Mess(char *Beep_com)
{
  pointer = 0;
  strncpy(command, Beep_com, sizeof(command) - 1);
  command[sizeof(command) - 1] = '\0';
  Event = Message;

}
void TElm327::Send_Mess(const __FlashStringHelper * Beep_com)
{
  pointer = 0;
  strncpy_P(command, (PGM_P)Beep_com, sizeof(command) - 1);
  command[sizeof(command) - 1] = '\0';
  Event = Message;

}

int TElm327::Elm_state(void){
return Error;
}

char* TElm327::Recive_Messege(void){
  if (Error<0) return Recive_Buf;
  else return NULL;
}