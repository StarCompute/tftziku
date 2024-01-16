#include <Arduino.h>
#include <TFT_eSPI.h>
// #include <LittleFS.h>
#include "common.h"

// // 注意tft屏幕的借口配置在相关的文件中
TFT_eSPI tft = TFT_eSPI();
void setup()
{
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  DrawStr2(tft,20, 50, "清澈的爱只为中国", TFT_GREEN);
}

void loop()
{
  delay(1);
}

 