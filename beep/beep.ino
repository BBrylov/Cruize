
#include "beep.h"
#include "beep_cfg.h"

TBeep Beep ;
void setup() {

  //Serial.begin(115200);
  Beep.Start(BuZZer_pin);
  Beep.SetBeep(FPSTR(MWarning));
}

void loop() {

  Beep.Loop();
}
