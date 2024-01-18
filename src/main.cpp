#include <Arduino.h>
#include <TFT_eSPI.h>
// #include <LittleFS.h>
#include "xfont.h"

// // 注意tft屏幕的借口配置在相关的文件中
TFT_eSPI tft = TFT_eSPI();
void setup()
{
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  // tft.drawLine(16,46,128,46,TFT_WHITE);
  // tft.drawLine(16,66,128,66,TFT_WHITE);
  //  tft.drawLine(16,46,16,66,TFT_WHITE);
  //  tft.drawLine(128,46,128,66,TFT_WHITE);
   tft.drawFastHLine(16,66,112,TFT_LIGHTGREY);
  DrawStr2(tft,10, 50, "清澈的爱只为中国", TFT_GREEN);
 
}

void loop()
{
  delay(1);
}

 