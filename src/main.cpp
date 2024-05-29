#include <Arduino.h>

#include "xfont.h"

XFont *_xFont;
void setup()
{
  Serial.begin(115200);
  Serial.println("Init.");
  // 注意，xFont的初始化必须在setup内
  unsigned long myTime=millis();;
  _xFont = new XFont(true);
  Serial.printf("对象初始化耗时：%3f \r\n",(millis()-myTime)/1000.0);
  int c=0;
  #ifdef ARDUINO_GFX
    c=GREEN;
  #elif defined(TFT_ESPI) 
    c=TFT_GREEN;
  #endif

  // 以下两个办法显示同样的文字都是0.53秒，但是由于视觉原因，会感觉DrawChineseEx更快，实际一样，不过DrawChineseEx显示的字数太多的时候会存在风险。
  // _xFont->DrawChinese(10, 10, "业精于勤荒于嬉戏，行成于思毁于随。业精于勤荒于嬉戏，行成于思毁于随。业精于勤荒于嬉戏，行成于思毁于随。业精于勤荒于嬉戏，行成于思毁于随。", c);
  _xFont->DrawChineseEx(10, 10, "业精于勤荒于嬉戏，行成于思毁于随。业精于勤荒于嬉戏，行成于思毁于随。业精于勤荒于嬉戏，行成于思毁于随。业精于勤荒于嬉戏，行成于思毁于随。", c);

  //部门改写已输出位置的文字，一定要加入backCOlor,否则不必。全部位置使用backColor会增加计算量。
  _xFont->DrawChineseEx(10, 10, "我们",c,BLACK); //注意，arduino_gfx的RGB可能是BGR，这点要注意


  //以下方法直接获取像素并在串口输出
  String _pixDatas=_xFont->GetPixDatasFromLib("我");
  int lenPixDatas=_pixDatas.length();
  for(int i=0;i<lenPixDatas;i++){
    if(i%12==0)Serial.println("");
    if(_pixDatas[i]=='1')Serial.print(_pixDatas[i]);
    else {
      Serial.print(" ");
    }
  }
  Serial.printf("\r\n系统总启动和显示耗时：%3f",(millis()-myTime)/1000.0);
}

void loop()
{
}
