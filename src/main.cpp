#include <Arduino.h>

#include "xfont.h"

XFont *_xFont;
void setup()
{
  Serial.begin(115200);
  Serial.println("Init.");
  // 注意，xFont的初始化必须在setup内
  unsigned long myTime=millis();;
  _xFont = new XFont();
  Serial.printf("对象初始化耗时：%3f \r\n",(millis()-myTime)/1000.0);
  int c=0;
  #ifdef ARDUINO_GFX
    c=GREEN;
  #elif defined(TFT_ESPI) 
    c=TFT_GREEN;
  #endif
  _xFont->DrawChinese(10, 10, "业精于勤荒于嬉戏，行成于思毁于随。业精于勤荒于嬉戏，行成于思毁于随。业精于勤荒于嬉戏，行成于思毁于随。业精于勤荒于嬉戏，行成于思毁于随。", c);
  Serial.printf("系统总启动和显示耗时：%3f",(millis()-myTime)/1000.0);

  //以下方法直接获取像素并在串口输出
  String _pixDatas=_xFont->GetPixDatasFromLib("我");
  for(int i=0;i<_pixDatas.length();i++){
    if(i%12==0)Serial.println("");
    if(_pixDatas[i]=='1')Serial.print(_pixDatas[i]);
    else {
      Serial.print(" ");
    }
  }
}

void loop()
{
}
