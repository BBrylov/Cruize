#include <pgmspace.h>
#include "Elm327.h"
#include <Arduino.h>


TElm327 Elm327(UART0);

void setup() {
  // put your setup code here, to run once:
 // pinMode(LED_BUILTIN, OUTPUT);
  //digitalWrite(LED_BUILTIN, LOW);
  Elm327.begin(1200);

}

void loop() {
  // put your main code here, to run repeatedly:
//digitalWrite(LED_BUILTIN, HIGH);
  
}


