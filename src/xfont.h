#include <Arduino.h>

// #define ARDUINO_GFX
#define TFT_ESPI

#ifdef ARDUINO_GFX
#include "Arduino_GFX_Library.h"
#elif defined(TFT_ESPI) 
#include <TFT_eSPI.h>
#endif

#include <LittleFS.h>

// 转化字符数组为字符串
String getStringFromChars(uint8_t * bs,int l);
// 把utf8编码字符转unicode编码
String getUnicodeFromUTF8(String s);
// 从字符的像素16进制字符重新转成二进制字符串
String getPixDataFromHex(String s);

#ifdef ARDUINO_GFX
void DrawSingleStr(Arduino_GFX *tftOutput, int x, int y, String strBinData, int c, bool ansiChar);
#elif defined(TFT_ESPI) 
void DrawSingleStr(TFT_eSPI &tftOutput, int x, int y, String strBinData, int c, bool ansiChar);
#endif

// 在屏幕上输出字符，暂时保留这个方法

#ifdef ARDUINO_GFX
void DrawStr(Arduino_GFX *tftOutput, int x, int y, String str, int color);
#elif defined(TFT_ESPI) 
void DrawStr(TFT_eSPI &tftOutput, int x, int y, String str, int color);
#endif

// 在屏幕上输出字符，相对于drawstr这里修正了半角和数字的显示

#ifdef ARDUINO_GFX
void DrawStr2(Arduino_GFX *tftOutput, int x, int y, String str, int c);
#elif defined(TFT_ESPI) 
void DrawStr2(TFT_eSPI &tftOutput, int x, int y, String str, int c);
#endif

// 初始化字库
void initZhiku(String fontPath);
// 从字库文件获取字符对应的编码字符串
String getCodeDataFromFile(String strUnicode, String fontPath);
// 从字库文件获取字符对应的二进制编码字符串
String getPixBinStrFromString(String strUnicode, String fontPath);
// 依照字号和编码方式计算每个字符存储展位
int getFontPage(int font_size,int bin_type);