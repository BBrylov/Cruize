#include "Elm327.h"
#include <uart.h>
#include "Arduino.h"
#include "HardwareSerial.h"
#include "Esp.h"
#include <String.h>

#define Send_Timeout 100
#define Reciv_Timeout 100
#define Elm_OK -1
#define Err_Send_Timeout 1
#define Err_ManyReciv 2
#define Err_Reciv_Timeout 3
#define Err_Reciv_Elm_Mess 4
#define Err_Reciv_Avto_Mess 5






//Конструктор
TElm327::TElm327(int8_t uart_nr): HardwareSerial(uart_nr) {};

int8_t TElm327::TX_Finish(void) {
  if (_uart == NULL || !(uart_tx_enabled(_uart))) {
    return 0;
  }
  return  ((USS(_uart_nr) >> USTXC) & 0xff);
}

uint8_t TElm327::Loop(void) {
  static uint32_t lastime;
  int32_t time_now;
  char *_pointer;
  switch (state) {
    case 0: //Ждем посылки
      if (Elm327.available()) Elm327.read();
      if (Event != Message) break;
      lastime = millis();
      Elm327.print(command);
      state = 1;
      Error = 0;
      break;
    case 1://Ждем окончания передачи пакета
      if (millis() - lastime > Send_Timeout) {
        Error = Err_Send_Timeout;
        state = 0;
        break;
      }
      if (Elm327.TX_Finish()) {
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
      while (Elm327.available()) {
        pointer++;
        if (pointer > 9) {
          Error = Err_ManyReciv;
          state = 0;
          break;
        }
        Recive_Buf[pointer] = Elm327.read();
        if ( Recive_Buf[pointer] == '>') { // посылка от Elm327 закончена
          state = 3; //переходим в анализ ответа от Elm327
          Recive_Buf[pointer + 1] = '\0';
          break;
        }
        if ( Recive_Buf[pointer] == '\r') { // посылка от авто закончена
          state = 4; // переходим в анализ данных от автомобиля
          Recive_Buf[pointer + 1] = '\0';
          break;
        }
      }
      if (millis() - lastime > Reciv_Timeout) {
        Error = Err_Reciv_Timeout;
        state = 0;
        break;
      }
      break;
    case 3://разбор ответа от Elm327
    {
     char *_pointer=strchr( str1,'O' ) ;
     if (!_pointer) {Error = Err_Reciv_Elm_Mess; state=0; break;}
      if (*(_pointer+1)=='K')  {Error=Elm_OK;state=0;break;}
    }
      break;
    case 4://разбор от автомобиля
     {
      char *_pointer=strchr( str1,'\r' ) ;
      if (!_pointer) {Error = Err_Reciv_Avto_Mess; state=0; break;}
     }
     pointer=0;




     
      break;
    default:
      state = 0;
      break;
  }
}


uint8_t TElm327::Send_Mess(char *Beep_com)
{
  pointer = 0;
  strncpy(command, Beep_com, sizeof(command) - 1);
  command[sizeof(command) - 1] = '\0';
  Event = Message;

}
uint8_t TElm327::Send_Mess(const __FlashStringHelper * Beep_com)
{
  pointer = 0;
  strncpy_P(command, (PGM_P)Beep_com, sizeof(command) - 1);
  command[sizeof(command) - 1] = '\0';
  Event = Message;

}

