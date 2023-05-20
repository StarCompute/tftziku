#include <Arduino.h>
#include <painlessMesh.h>
#include <TFT_eSPI.h>
#include <LittleFS.h>
#include "common.h"

// 注意tft屏幕的借口配置在相关的文件中
TFT_eSPI tft = TFT_eSPI();


void DrawPixStr(int x, int y, String strUni, int fontsize)
{

  for (int m = 0; m < fontsize; m++)
  {
    for (int i = 0; i < fontsize; i++)
    {
      if ((String)strUni[m * fontsize + i] == "1")
        tft.drawPixel(y + i, x + m, TFT_GREEN);
    }
  }
}
// 下面代码在TFT屏幕输出文字
int pX = 16;
int pY = 0;
int fontsize = 16;      // 字号
int amountDisplay = 10; // 每行显示多少汉字，其实这个显示数量应该通过屏幕的宽度来计算字号
int screenWidth = 160;

int singleStrPixsAmount = fontsize * fontsize;

void setup()
{

  Serial.begin(115200);
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);

  // String strBinDisplay = getPixBinStrFromString("好软,这是一个自定义软字库的显示。欢迎你的使用！这个软字库有七千个汉字，基本囊括了日常使用的汉字内容。", "/x.font");

  DrawStr(10, 2, "湿度：",TFT_GREEN);
  DrawStr(10, 18, "温度：",TFT_RED);
  DrawStr(10, 34, "光照度：",TFT_DARKGREEN);
}

void loop()
{
  delay(1);
}

void DrawStr(int x = 0, int y = 0, String str = "星算",int color=TFT_GREEN)
{

  // 下面的代码显示对应的汉字在TFT屏幕上
  String strBinData=getPixBinStrFromString(str, "/x.font");
  amountDisplay = screenWidth / fontsize; // 如果不愿意动态计算显示数量可以注释调这一行
  for (int i = 0; i < strBinData.length(); i++)
  {

    pX = int(i % fontsize) + int(i / (singleStrPixsAmount)) * fontsize - int(i / (singleStrPixsAmount * amountDisplay)) * fontsize * amountDisplay;

    // 对于pY,每fontsize个像素后+1，每singleStrPixsAmount个像素后归0，同时每换一行，pY要加上fontsize个像素；
    pY = int((i - int(i / singleStrPixsAmount) * singleStrPixsAmount) / fontsize) + int(i / (singleStrPixsAmount * amountDisplay)) * fontsize;

    if (strBinData[i] == '1')
    {
      tft.drawPixel(pX+x , pY +y, color);
    }
  }
}