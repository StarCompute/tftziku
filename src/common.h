#include <Arduino.h>

// 使用说明
// 1，包含本文件；
// 2，调用initZhiku 初始化；
// 3，调用getPixBinStrFromString2 就可以获取对应的字符取模数据；
// 注意3，调用getPixBinStrFromString2 在本例中是在DrawStr中使用的

void initZhiku(String fontPath);
String getStringFromChars(uint8_t * bs,int l);
String getStringFromChars2(uint8_t *bs, int l);
String getUnicodeFromUTF82(String s);
String getUnicodeFromUTF8(String s);
int *getBin(int Dec);
int *getBin2(uint8_t data);
String getPixDataFromHex(String s);
void DrawStr(int, int, String,int color);


String getPixBinStrFromString2(String displayString, String fontPath);

int getStrCountFromFontFile(uint8_t * bs);
int getFontSizeFromFontFile(uint8_t * bs);

String getPixBinStrFromString(String displayString,String fontPath);