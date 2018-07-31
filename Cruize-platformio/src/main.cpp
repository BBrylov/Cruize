#include <pgmspace.h>
#include "ESPWeb.h"
#include "Date.h"
#include "Schedule.h"
#include "RTCmem.h"
#include "beep.h"
#include "beep_cfg.h"
#include "Elm327.h"
#include "cruize-const.h"
//#define PROGMEM_T __attribute__((section(".irom.text.template")))
 const char ate0[] PROGMEM = "ATE0";
  const char atz[] PROGMEM = "ATZ";
  const char ath0[] PROGMEM = "ATH0";
  const char atd[] PROGMEM = "ATD";
  const char ati[] PROGMEM = "ATI";
  const char atib10[] PROGMEM = "ATIB10";
  const char atsh6410f5[] PROGMEM = "ATSH6410F5";
  const char atl0[] PROGMEM = "ATL0";
  const char atst40[] PROGMEM = "ATST40";
  const char atsp3[] PROGMEM = "ATSP3";
  const char odb_speed[] PROGMEM = "22174A1";



TElm327 Elm327(UART0);
TBeep Beep ;






class EspAutoComp  : public ESPWebBase{
public:
EspAutoComp():ESPWebBase(){};


protected:
  void setupExtra();
  void loopExtra();
  int8_t A_ReadData();
  int8_t A_ControlMotor();
private:
uint8_t Speed_cur;

};

int8_t EspAutoComp::A_ReadData(){
    //enum EEvent : uint8_t { Elm_ok=0, Elm_error=1, TimerEvent=-1, Elm_Busy=2 };
    int8_t event;
    static int8_t state=1;
    static int8_t Error_count=0;
    uint8_t *hexout;

    event=Elm327.Elm_state();

    switch(state){
        case 1://Состояние ожидание готовности ELM
            switch(event) {   //обработка входного состояния Elm
            case Elm_OK:
            Elm327.Send_Mess(FPSTR(odb_speed));
            state=2;
            break;
            case Elm_busy:
            state=1;
            break;
            default:
            state=3;
            break;
            }

        break;
        case 2://Состояние ожидание ответа ELM
            switch(event) {   //обработка входного состояния Elm
            case Elm_OK:
            hexout=Elm327.Recive_Hex();
            Speed_cur=hexout[3];
            Error_count=0;
            state=1;
            break;
            case Elm_busy:
            state=2;
            break;
            default:
            state=3;
            break;
            }
        break;
        case 3://Состояние обработки ошибок
            if (Error_count>MaxErrorELM_count) {
              _log->println(F("ELM327 too many Errors."));   
              Error_count=0;
              }
        Elm327.Send_Mess(FPSTR(ati));
        state=1;
        break;
        case 5://Состояние другие автоматы запросов
        state=1;
        break;
        default://Состояние
        state=1;
        break;

    }



return state;
}
int8_t EspAutoComp::A_ControlMotor(){

}


void EspAutoComp::setupExtra(){
    ESPWebBase::setupExtra();
    Speed_cur=0;// обнулили текущую сторость
    Beep.Start(BuZZer_pin);
    _log->println(F("ELM327 init started."));
    if (!Elm327.SetupElm(9600)) {
        _log->println(F("ELM327 init Finish."));
        Beep.SetBeep(FPSTR(BStart));
    }
    else {
        _log->println(F("ELM327 init Error."));
        Beep.SetBeep(FPSTR(BFatalError));
    }



}


void EspAutoComp::loopExtra(){
    ESPWebBase::loopExtra();
Beep.Loop();//Автомат звуковых сигналов
A_ReadData();//автомат верхеуровнего чтения с ELM
Elm327.LoopSerial();// Автомат работы с ELM




}





EspAutoComp* app = new EspAutoComp();

void setup() {
//    delayMicroseconds(500);
 //Elm327.begin(9600);
 //Elm327.println("Hi sdfjksdkfgbsk");
 //Elm327.println("Hi dlskfndklfjsjkdgkjg");

 app->setup();
}

void loop() {
 //Elm327.println("Hi");
  app->loop();
}
