#include <Arduino.h>
#include "xfont.h"

XFont *_xFont;
void setup()
{
  Serial.begin(115200);
  Serial.println("Arduino_GFX Hello World example");
   _xFont  = new XFont();
  _xFont->DrawChinese(10, 10, "业精于勤荒于嬉戏，行成于思毁于随。", GREEN);
  
}

void loop()
{
}
