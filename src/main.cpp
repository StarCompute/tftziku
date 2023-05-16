#include <Arduino.h>
#include <TFT_eSPI.h>
#include <LittleFS.h>
#include "common.h"

TFT_eSPI tft = TFT_eSPI();
long beginTime, endTime;

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

void setup()
{

  Serial.begin(115200);
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);


  String strBinDisplay = getPixBinStrFromString("好软");

  // 下面代码在终端输出文字点阵。
  Serial.println(strBinDisplay.length());
  for (int i = 0; i < strBinDisplay.length(); i++)
  {
    if (i % 16 == 0)
      Serial.print("\r\n");
    if (strBinDisplay[i] == '0')
      Serial.print(' ');
    if (strBinDisplay[i] == '1')
      Serial.print(strBinDisplay[i]);
  }

  // 下面代码在TFT屏幕输出文字
  int pX = 16;
  int pY = 0;
  int fontsize=16; //字号
  int amountDisplay=10; //每行显示多少汉字
  int singleStrPixsAmount=fontsize*fontsize;
  for (int i = 0; i < strBinDisplay.length(); i++)
  {
    // 这里必须有特别的说明，每个字符的像素点总数是singleStrPixsAmount=fontsize*fontsize,如果是16号字体就是256个；
    // 每行显示10个字，那么他们到一点阶段就必须换行，x坐标归0，y坐标必须加上字体对应的像素
    // 对于pX,每显示fontsize个像素后就必须字体归到起始点，注意每显示n字符后，这个起始点就必须加上fontsize*n这个起始值
    // 同时对于换行也必须处理。

    pX=int(i%fontsize)+int(i/(singleStrPixsAmount))*fontsize-int(i/(singleStrPixsAmount*amountDisplay))*fontsize*amountDisplay;

    // 对于pY,每fontsize个像素后+1，每singleStrPixsAmount个像素后归0，同时每换一行，pY要加上fontsize个像素；
    pY =int((i-int(i/singleStrPixsAmount)*singleStrPixsAmount)/fontsize)+int(i/(singleStrPixsAmount*amountDisplay))*fontsize;

    if (strBinDisplay[i] == '1')
    {
      tft.drawPixel(pX, pY, TFT_GREEN);
    }
  }
}

void loop()
{
  delay(1);
}
  