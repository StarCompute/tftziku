#include <Arduino.h>
#include "xfont.h"

XFont *_xFont;
void setup()
{
  Serial.begin(115200);
  Serial.println("Init.");
  // 注意，xFont的初始化必须在setup内
  _xFont = new XFont();
  // unsigned long myTime=millis();;
  _xFont->DrawChinese(10, 10, "业精于勤荒于嬉戏，行成于思毁于随。业精于勤荒于嬉戏，行成于思毁于随。业精于勤荒于嬉戏，行成于思毁于随。业精于勤荒于嬉戏，行成于思毁于随。", GREEN);
  // Serial.println(millis()-myTime);
}

void loop()
{
}
