// draw mutiul size
// 使用不同字库显示不同的汉字

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
    ;
#ifdef ARDUINO_GFX
    Arduino_DataBus *_bus = create_default_Arduino_DataBus();
    Arduino_GFX *_gfxTft = new Arduino_ST7735(_bus, -1, 0, false, 128, 160, 0, 0, 0, 0, false);
    _xFont = new XFont(_gfxTft);
#elif defined(TFT_ESPI)
    _xFont = new XFont(true);
#endif

    Serial.printf("对象初始化耗时：%.3f \r\n", (millis() - myTime) / 1000.0);

    // 下面的代码展示了如何对于多字体的支持
    _xFont->reInitZhiku("/x_simkai.ttf_18_b64.font");
    _xFont->DrawChineseEx(0, 0, "书山有路", c);

    _xFont->reInitZhiku("/x_simsun.ttc_18_b64.font");
    _xFont->DrawChineseEx(0, 30, "书山有路", c);

    _xFont->reInitZhiku("/x_simhei.ttf_18_b64.font");
    _xFont->DrawChineseEx(0, 62, "书山有路", c);

    _xFont->reInitZhiku("/x_simfang.ttf_18_b64.font");
    _xFont->DrawChineseEx(0, 92, "书山有路", c);
    // 应该重新初始化回默认字库
    _xFont->reInitZhiku("/x.font");

    Serial.printf("\r\n系统总启动和显示耗时：%3f", (millis() - myTime) / 1000.0);
}
void loop()
{
}
