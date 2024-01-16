#include <Arduino.h>
#include <TFT_eSPI.h>
#include <LittleFS.h>

// 转化字符数组为字符串
String getStringFromChars(uint8_t * bs,int l);
// 把utf8编码字符转unicode编码
String getUnicodeFromUTF8(String s);
// 从字符的像素16进制字符重新转成二进制字符串
String getPixDataFromHex(String s);

void DrawSingleStr(TFT_eSPI &tftOutput, int x, int y, String strBinData, int c, bool ansiChar);

// 在屏幕上输出字符，暂时保留这个方法
void DrawStr(TFT_eSPI &tftOutput,int x, int y, String str,int color);
// 在屏幕上输出字符，相对于drawstr这里修正了半角和数字的显示
void DrawStr2(TFT_eSPI &tftOutput,int x, int y, String str,int color);
// void DrawStr( TFT_eSPI &tftOutput,int x = 0, int y = 0, String str = "星算", int color = TFT_GREEN);

// 初始化字库
void initZhiku(String fontPath);
// 从字库文件获取字符对应的二进制编码字符串
String getPixBinStrFromString(String strUnicode, String fontPath);
// 依照字号和编码方式计算每个字符存储展位
int getFontPage(int font_size,int bin_type);