#include <Arduino.h>
#include "ESPWeb.h"
#include "Date.h"
#include "Schedule.h"
#include "RTCmem.h"
#include "beep.h"
#include "beep_cfg.h"
#include "Elm327.h"

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
}

void EspAutoComp::loopExtra(){
    ESPWebBase::loopExtra();

}





EspAutoComp* app = new EspAutoComp();

void setup() {
#ifndef NOSERIAL
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  Serial.println();
#endif

  app->setup();
}

void loop() {
  app->loop();
}
