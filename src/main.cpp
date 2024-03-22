#include <Arduino.h>
#include "xfont.h"


#ifdef ARDUINO_GFX
#include "Arduino_GFX_Library.h"

Arduino_DataBus *bus = create_default_Arduino_DataBus();
Arduino_GFX *tft = new Arduino_ST7735(bus);

#define GFX_BL DF_GFX_BL
#elif defined(TFT_ESPI) 
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
#endif




// put function declarations here:

void setup()
{
  // put your setup code here, to run once:

  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  Serial.println("Arduino_GFX Hello World example");
#ifdef ARDUINO_GFX
  if (!tft->begin())
  {
    Serial.println("gfx->begin() failed!");
  }
  tft->fillScreen(BLACK);

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif
  tft->setCursor(10, 10);
  tft->setTextColor(RED);
  tft->println("Hello World!");
  //
  delay(5000); // 5 seconds
  tft->fillScreen(BLACK);
  tft->setTextColor(GREEN);
  // gfx->drawPixel()
  DrawStr2(tft, 32, 1, "清澈的爱只为中国", GREEN);
#elif defined(TFT_ESPI)
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  // tft.drawLine(16,46,128,46,TFT_WHITE);
  // tft.drawLine(16,66,128,66,TFT_WHITE);
  //  tft.drawLine(16,46,16,66,TFT_WHITE);
  //  tft.drawLine(128,46,128,66,TFT_WHITE);
   tft.drawFastHLine(16,66,112,TFT_LIGHTGREY);
  DrawStr2(tft,10, 50, "清澈的爱只为中国 ", TFT_GREEN);

#endif
}

void loop()
{
}
