#include <pgmspace.h>
#include <uart.h>
#include <HardwareSerial.h>







//Параметры для настройки
#define Send_Timeout 100
#define Reciv_Timeout 100
//Коды ошибок
#define Elm_OK -1
#define Err_Send_Timeout 1
#define Err_ManyReciv 2
#define Err_Reciv_Timeout 3
#define Err_Reciv_Elm_Mess -2
#define Err_Reciv_Avto_Mess -3
#define Elm_busy 6


class TElm327: public HardwareSerial {
  public:
    TElm327(int8_t uart_nr);

    enum EEvent : uint8_t { Idle, Message, Timeout, Send_Mes, Reciv_Mes };
    int8_t LoopSerial(void); // основной автомат низкокго уровня передачи
    void Send_Mess(char *Beep_com);//посылает посылку из озу
    void Send_Mess(const __FlashStringHelper *Beep_com);//посылает посылку из eeprom
    int8_t TX_Finish(void);// проверка закончена ли передача
    int Elm_state(void);// опрос состояния автомата по работе с ELM. возвращается код ошибки.
    char* Recive_Messege(void); //выдать указатель на символьное сообщение принятое от Elm или автомобиля
    int8_t SetupElm(unsigned long baud);// установка параметра скорости порта и иницализации ELM из массива init_elm


  private:
    char command[10];
    uint8_t pointer;
    EEvent Event;
    uint8_t state;
    int8_t Error;
    char Recive_Buf[10];
    uint8_t Recive_hex[10];
    int8_t FErrorElm=0;//общая ошибка инициализации ELM327
};

