#include <Arduino.h>

String getStringFromChars(uint8_t * bs,int l);
String getStringFromChars2(uint8_t *bs, int l);
String getUnicodeFromUTF82(String s);
String getUnicodeFromUTF8(String s);
int *getBin(int Dec);
int *getBin2(uint8_t data);
String getPixDataFromHex(String s);
void DrawStr(int x,int y,String str);