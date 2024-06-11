#include <Arduino.h>

#include "xfont.h"

XFont *_xFont;
void setup()
{
  Serial.begin(115200);
  Serial.println("Init.");
  int c=0;
  #ifdef ARDUINO_GFX
    c=GREEN;
  #elif defined(TFT_ESPI) 
    c=TFT_GREEN;
  #endif

  // 注意，xFont的初始化必须在setup内
  unsigned long myTime=millis();;
  #ifdef ARDUINO_GFX
    Arduino_DataBus *_bus = create_default_Arduino_DataBus();
    Arduino_GFX *_gfxTft = new Arduino_ST7735(_bus,-1,0,false,128,160,0,0,0,0,false);
    _xFont = new XFont(_gfxTft);
  #elif defined(TFT_ESPI) 
    _xFont = new XFont(true);
  #endif
  
  Serial.printf("对象初始化耗时：%.3f \r\n",(millis()-myTime)/1000.0);
  // 以下两个办法显示同样的文字都是0.53秒，但是由于视觉原因，会感觉DrawChineseEx更快，实际一样，不过DrawChineseEx显示的字数太多的时候会存在风险。
  // _xFont->DrawChinese(10, 10, "业精于勤荒于嬉戏，行成于思毁于随。业精于勤荒于嬉戏，行成于思毁于随。业精于勤荒于嬉戏，行成于思毁于随。业精于勤荒于嬉戏，行成于思毁于随。", c);
  // _xFont->DrawChineseEx(0, 10, "业精于勤荒于嬉戏，\n行成于思毁于随。\r\n业精于勤荒于嬉戏，\n行成于思毁于随。\r\n业精于勤荒于嬉戏，\n行成于思毁于随。\r\n业精于勤荒于嬉戏，行成于思毁于随。", c);
 _xFont->DrawChineseEx(0, 10, " 我\r爱\n你\n ! ", c);
  //部门改写已输出位置的文字，一定要加入backCOlor,否则不必。全部位置使用backColor会增加计算量。
  // _xFont->DrawChineseEx(10, 10, "我们",c,BLACK); //注意，arduino_gfx的RGB可能是BGR，这点要注意
// d000 \r
// a000 \n
// d280 \r\n
// 9000 \t

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
int cnt=222003;
void loop()
{
  // delay(20);
  // //  _xFont->DrawChineseEx(10, 10, "我们业精于勤荒于嬉戏，行成于思毁于随",GREEN,BLACK); //注意，arduino_gfx的RGB可能是BGR，这点要注意
  //  _xFont->DrawChineseEx(10, 100,"1中国2大财3",GREEN,BLACK); 
  //  _xFont->DrawChineseEx(100, 100,(String)cnt,GREEN,BLACK); 
  //  cnt++;
}
