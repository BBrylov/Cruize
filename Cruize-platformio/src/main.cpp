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
    Elm327.SetupElm(9600);
}

void EspAutoComp::loopExtra(){
    ESPWebBase::loopExtra();





}





EspAutoComp* app = new EspAutoComp();

void setup() {
  app->setup();
}

void loop() {
  app->loop();
}
