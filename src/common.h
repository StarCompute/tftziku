#include <Arduino.h>
#include <TFT_eSPI.h>
#include <LittleFS.h>


String getStringFromChars(uint8_t * bs,int l);
// String getStringFromChars2(uint8_t *bs, int l);
String getUnicodeFromUTF8(String s);
String getPixDataFromHex(String s);
void DrawSingleStr(TFT_eSPI &tftOutput, int x, int y, String strBinData, int c, bool ansiChar);

void DrawStr(TFT_eSPI &tftOutput,int x, int y, String str,int color);
void DrawStr2(TFT_eSPI &tftOutput,int x, int y, String str,int color);
// void DrawStr( TFT_eSPI &tftOutput,int x = 0, int y = 0, String str = "星算", int color = TFT_GREEN);
void initZhiku(String fontPath);
String getPixBinStrFromString(String strUnicode, String fontPath);

int getStrCountFromFontFile(uint8_t * bs);
int getFontSizeFromFontFile(uint8_t * bs);
int getFontPage(int font_size,int bin_type);