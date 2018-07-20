#define STEP_PIN 16 //D0
#define DIR_PIN 5 //D1
#define BUZZER 4 // D2
#define DSTOP 0 //D3
#define DIG 2 //D4
#define LEDR 15 // D8
#define LEDB 12 // D6
#define LEDG 13 // D7
#define IR 14 // D5
#define DSPEED 10 //SD3
//#define NOSERIAL 1


#include "ESPWeb.h"


const char* const jsonSpeed PROGMEM = "Speed";
const char* const jsonRPM PROGMEM = "RPM";

class ESPCruiz : public ESPWebBase {
public:
  ESPCruiz() : ESPWebBase() {}
protected:
  void setupExtra();
  void loopExtra();
  void handleRootPage();
  String jsonData();

private:
/*#if defined(BMP085)
  Adafruit_BMP085* bm;
#elif defined(BMP280)
  Adafruit_BMP280* bm;
#else
  Adafruit_BME280* bm;
#endif*/
  float Speed;
  uint16_t RPM;

};

ESPCruiz* app = new ESPCruiz();

void setup() {
//  Serial.begin(115200);
//  Serial.println();
  app->setup();
}

void loop() {
  app->loop();
}
