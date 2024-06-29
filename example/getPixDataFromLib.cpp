// getPixDataFromLib.cpp
// 本事例演示怎么直接从字库获取到像素数据并输出

#include <Arduino.h>

#include "xfont.h"

XFont *_xFont;
void setup()
{
    Serial.begin(115200);
    Serial.println("Init.");

    // 注意，xFont的初始化必须在setup内
    unsigned long myTime = millis();
    _xFont = new XFont(false);

    // 注意,由于ram的显示,下面示例中的汉字不要太多,可以多次获取
    String _pixDatas = _xFont->GetPixDatasFromLib("永不为奴");
    int lenPixDatas = _pixDatas.length();
    for (int i = 0; i < lenPixDatas; i++)
    {
        if (i % 24 == 0)
            Serial.println("");
        if (_pixDatas[i] == '1')
            Serial.print(_pixDatas[i]);
        else
        {
            Serial.print(" ");
        }
    }
    Serial.printf("\r\n系统总启动和显示耗时：%3f", (millis() - myTime) / 1000.0);
}

void loop()
{
}
