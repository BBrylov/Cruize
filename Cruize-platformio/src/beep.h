#include <pgmspace.h>
#include <ESP8266WiFi.h>






class TBeep {
  public:
    enum EEvent : uint8_t { Idle, Message, Timeout};
    uint8_t Start(uint8_t Pin_num);
    uint8_t Loop(void);
    uint8_t SetBeep(char *Beep_com);
    uint8_t SetBeep(const __FlashStringHelper *Beep_com);
  protected:

  private:
    int8_t PinN;
    char command[10];
    uint8_t pointer;
    EEvent Event;
    uint8_t state;
};
