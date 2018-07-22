#include <pgmspace.h>
#include <uart.h>
#include <HardwareSerial.h>



class TElm327: public HardwareSerial {
  public:
    TElm327(int8_t uart_nr);

    enum EEvent : uint8_t { Idle, Message, Timeout, Send_Mess, Reciv_Mess };
    uint8_t Loop(void);
    uint8_t Send_Mess(char *Beep_com);
    uint8_t Send_Mess(const __FlashStringHelper *Beep_com);
    int8_t TX_Finish(void);


  private:
    char command[10];
    uint8_t pointer;
    EEvent Event;
    uint8_t state;
    int8_t Error;
    char Recive_Buf[10];
    uint8_t Recive_hex[10];
};

