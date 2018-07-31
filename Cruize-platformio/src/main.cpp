#include <pgmspace.h>

#include "ESPWeb.h"
#include "Date.h"
#include "Schedule.h"
#include "RTCmem.h"
#include "beep.h"
#include "beep_cfg.h"
#include "Elm327.h"
#include "cruize-const.h"


TElm327 Elm327(UART0);
TBeep Beep ;






class EspAutoComp  : public ESPWebBase{
public:
EspAutoComp():ESPWebBase(){};


protected:
  void setupExtra();
  void loopExtra();

private:


};

void EspAutoComp::setupExtra(){
    ESPWebBase::setupExtra();
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
Beep.Loop();




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
