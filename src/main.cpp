#include <Arduino.h>
#include "xfont.h"

XFont *_xFont;

void setup()
{

  Serial.begin(115200);
  Serial.println("Init.");
  int c = 0;
#ifdef ARDUINO_GFX
  c = GREEN;
#elif defined(TFT_ESPI)
  c = TFT_GREEN;
#endif

  // 注意，xFont的初始化必须在setup内
  unsigned long myTime = millis();
#ifdef ARDUINO_GFX
  Arduino_DataBus *_bus = create_default_Arduino_DataBus();
  Arduino_GFX *_gfxTft = new Arduino_ST7789(_bus, 14, 0, true, 240, 320, 0, 0, 0, 0);
  _xFont = new XFont(_gfxTft);
#elif defined(TFT_ESPI)
  _xFont = new XFont(true);
#endif
  Serial.printf("对象初始化耗时：%.3f \r\n", (millis() - myTime) / 1000.0);
  // 以下两个办法显示同样的文字都是0.53秒，但是由于视觉原因，会感觉DrawChineseEx更快，实际一样，不过DrawChineseEx显示的字数太多的时候会存在风险。
  _xFont->DrawChineseEx(10, 10, "业精于勤荒于嬉戏，行成于思毁于随。业精于勤荒于嬉戏，行成于思毁于随。业精于勤荒于嬉戏，行成于思毁于随。业精于勤荒于嬉戏，行成于思毁于随。", c);
  // _xFont->DrawChineseEx(0, 10, "业精于勤荒于嬉戏，\n行成于思毁于随。\r\n业精于勤荒于嬉戏，\n行成于思毁于随。\r\n业精于勤荒于嬉戏，\n行成于思毁于随。\r\n业精于勤荒于嬉戏，行成于思毁于随。", c);

  //  _xFont->DrawChineseEx(0, 10, " 我\r 爱\n 你\n ! ", c);

  // 下面代码切换字体并输出
  // _xFont->reInitZhiku("/x_simfang.ttf_24_b64.font");
 _xFont->tft->drawRect(0,164,320,71,WHITE);
 _xFont->tft->fillRect(0,164,320,71,DARKGREY);
 _xFont->tft->setTextSize(6);
 _xFont->tft->drawChar(1,170,'2',WHITE,DARKGREY);
 _xFont->tft->drawChar(40,170,'2',WHITE,DARKGREY);
  _xFont->tft->drawChar(80,170,':',WHITE,DARKGREY);
   _xFont->tft->drawChar(120,170,'2',WHITE,DARKGREY);
    _xFont->tft->drawChar(160,170,'0',WHITE,DARKGREY);
      _xFont->tft->drawChar(200,170,':',WHITE,DARKGREY);
         _xFont->tft->drawChar(240,170,'5',WHITE,DARKGREY);
    _xFont->tft->drawChar(280,170,'0',WHITE,DARKGREY);
  // _xFont->tft->en
//  _xFont->tft->drawEllipse(0,164,320,71,WHITE);
  // _xFont->DrawChineseEx(10, 169, "书山有路勤为径", c);
  // _xFont->DrawChineseEx(10, 206, "学海无涯苦作舟", c);

  // 下面两种方法都可以清除内存占用
  // _xFont->clear();
  // delete _xFont;

  Serial.printf("销毁后剩余内存: %.3f k \n", ESP.getFreeHeap() / 1024.0);
 
}

void loop()
{
  delay(200);
  // _xFont->tft.drawNumber(12,10,168);
}
