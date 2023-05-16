#include <Arduino.h>
// #include "simsun.h"
#include "painlessMesh.h"
#include <TFT_eSPI.h>
#include <LittleFS.h>
#include "fileo.h"
// #include "FS.h"
#include "common.h"

TFT_eSPI tft = TFT_eSPI();
long beginTime, endTime;

// #define MESH_PREFIX "kaka"
// #define MESH_PASSWORD "somethingSneaky"
// #define MESH_PORT 5557

// painlessMesh mesh;
// Scheduler userScheduler;
void DrawPixStr(int x, int y, String strUni, int fontsize)
{
  // strUni.replace()
  //  strUni.replace('\r\n','');
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
  Serial.println("kakka");
  Serial.println(esp_get_free_heap_size());
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);

  LittleFS.begin();

  // tft.loadFont("simsun12",LittleFS);
  // tft.loadFont(AA_FONT_SMALL, );
  // // tft.setTextSize(12);
  // // tft.setCursor(20, 20);
  // long x=millis();
  // tft.println(" i am xuankongi am xuankongi am xuankongi am xuankongi am xuankong");
  //   tft.println(" 这是一个自定义汉字软字库的显示，包括了7千个常用汉字，满足了日常的使用。");
  // Serial.print("TFT:");
  // Serial.println(millis()-x);
  // // tft.setCursor(20, 45);
  // tft.println("happy hahaha ");
  // tft.println("we care我们必须努力.");
  // tft.unloadFont();
  // // tft.setTextSize(2);
  // // Serial.println("begin");
  // getUnicodeFromUTF82("hello");
  // getUnicodeFromUTF8("你好");
  // getUnicodeFromUTF82("你好123");
  // getUnicodeFromUTF82("你好hello");
  // return;

  // bool b1 = LittleFS.exists("/1.txt");
  // bool b2 = LittleFS.exists("/kaka/kaka.txt");
  // bool b3 = LittleFS.exists("/asdf.txt");
  // tft.println(b1+b2+b3);
  // tft.println(b2);
  // tft.println(b3);
  // listDir(LittleFS,"/",2);
  // Serial.println("end");
  File file = LittleFS.open("/x.font");
  static uint8_t buf_total_str[6];
  static uint8_t buf_fontsize[2];
  Serial.println(file.position());
  file.read(buf_total_str, 6);
  Serial.println(file.position());
  file.read(buf_fontsize, 2);

  // Serial.println((String)*buf_total_str);

  // String s1=(char)buf_total_str;

  // String s=file.readString();
  // for (int i=0;i<6;i++){
  //   tft.println(buf_total_str[i]);
  //   Serial.println(buf_total_str[i]);
  //   s1+=(char)buf_total_str[i];
  // }
  String s1 = getStringFromChars(buf_total_str, 6);
  String s2 = getStringFromChars(buf_fontsize, 2);
  int total_font_cnt = strtoll(s1.c_str(), NULL, 16);
  int font_size = s2.toInt();

  Serial.println(s1);
  Serial.println(total_font_cnt);
  Serial.println(font_size);
  int font_unicode_cnt = total_font_cnt * 5;
  String font_unicode = "";
  uint8_t *buf_total_str_unicode;
  int font_page = int(font_size * font_size / 8 * 2);

  uint8_t buf_seek_pixdata[font_page];

  buf_total_str_unicode = (uint8_t *)malloc(font_unicode_cnt);

  long readtime = millis();

  file.read(buf_total_str_unicode, font_unicode_cnt);
  String strUnicodes = getStringFromChars2(buf_total_str_unicode, font_unicode_cnt);
  free(buf_total_str_unicode);
  // Serial.println(kk);
  // int ddd=kk.indexOf("u97af");
  // Serial.println(ddd/5);
  // Serial.println(millis()-readtime);
  // for(int i=0;i<font_unicode_cnt;i++){
  //   Serial.print((char)buf_total_str_unicode[i]);
  // }
  // String kk=getStringFromChars2(buf_total_str_unicode);
  // Serial.println((char *)buf_total_str_unicode);
  // return;
  // String teststr = "中华人民共和国中央人民政府";
  String teststr = "这是一个自定义汉字软字库的显示，包括了7千个常用汉字，满足了日常的使用。";

  //  teststr = "你好吗中国";
  // 人民共和国中央人民政府
  // teststr = "ABC";

  String strUnicode = getUnicodeFromUTF82(teststr);

  // String aa=getPixDataFromHex("04400e50784808480840fffe084008440a440c48183068220852088a2b061002");

  // return;

  // Serial.println( s3.c_str());
  // file.find()
  // font_unicode= getStringFromChars(buf_total_str_unicode, font_unicode_cnt);
  // Serial.print(font_unicode);
  // tft.println((String)*buf_total_str);
  //  tft.println((String)*buf_fontsize);
  Serial.println(strUnicode.length());
  int unicode_begin_idx = 6 + 2 + total_font_cnt * 5;
  Serial.println("begin");
  String ff = "";
  int pixX = 0;
  int pixY = 0;
  long ttime = millis();
  long totaltime = 0;

  for (int i = 0; i < strUnicode.length(); i = i + 4)
  {
    long dtime = millis();
    // Serial.println(i);
    String _str = "u" + strUnicode.substring(i, i + 4);
    // Serial.println(_str);

    file.seek(8);

    //     getPixDataFromHex("0100010001003ff8");
    // // 010001000100
    //     return;
    // file.find(_str.c_str());
    String asd = "";
    int readFileSize = 5 * 400;
    uint8_t buf_read_pixdata[readFileSize];

    // int cnt_page = (total_font_cnt + 6) / 6;'
    int cnt_page = total_font_cnt / 400 + 1;
    int uIdx = 0;
    beginTime = millis();

    int p = strUnicodes.indexOf(_str);
    uIdx = p / 5;

    // for (int m = 0; m < cnt_page; m++)
    // {
    //   file.read(buf_read_pixdata, readFileSize);
    //   asd = getStringFromChars2(buf_read_pixdata, readFileSize);
    //   // Serial.println(asd);
    //   int p = asd.indexOf(_str);
    //   if (p > 0)
    //   {
    //     // Serial.println("found");
    //     uIdx = (m * readFileSize + p) / 5;
    //     // ff += "1|" + String(m) + "|" + p + "|" + uIdx + ",\r\n";

    //     break;
    //   }
    // }
    // Serial.print("检索耗时:");
    // long seachtime = millis() - beginTime;
    // Serial.println(seachtime);
    beginTime = millis();
    int pixbeginidx = unicode_begin_idx + uIdx * font_page;
    file.seek(pixbeginidx);
    file.read(buf_seek_pixdata, font_page);
    // file.readString
    String su = getStringFromChars(buf_seek_pixdata, font_page);
    // Serial.println(file.position());

    // Serial.println(su);
    String aa = getPixDataFromHex(su);
    long seektime = millis() - beginTime;
    // Serial.print("定位耗时:");
    // Serial.println(seektime);
    pixX = int(i / 10 / 4) * font_size;
    pixY = font_size * (i / 4 % 10);
    // Serial.println(pixX);

    // beginTime = millis();
    DrawPixStr(pixX, pixY, aa, font_size);
    //  Serial.print("显示耗时:");
    // Serial.println(millis()-beginTime);
    // Serial.println(as);
    //  Serial.print("处理耗时:");
    // Serial.println(millis()-dtime);
    // long disptime = millis() - beginTime;
    long danyi = millis() - dtime;
    // Serial.print("单一汉字耗时:");
    // Serial.println(danyi);
    totaltime = totaltime + (millis() - dtime);
  }
  Serial.print("整体耗时:");
  Serial.println(millis() - ttime);
  Serial.println(totaltime);
  // tft.unloadFont();
  // Serial.println(ff);
  Serial.print("end");
  // if (!LittleFS.exists("/simsun12.vlw"))
  // {
  //   Serial.println("字体文件不存在");
  // }
  // else
  // {
  //   tft.loadFont("msyh12", LittleFS);
  //   tft.println("");
  //   tft.println("GB2312标准共收录6763个汉字，其中一级汉字3755个，二级汉字3008个；同时，GB 2312收录了包括拉丁字母、希腊字母、日文平假名及片假名字母、俄语西里尔字母在内的682个全角字符。整个字符集分成94个区，每区有94个位。");
  //   tft.println("《现代汉语通用字表》是中国大陆于1988年出版的字表，内收通用字7000个（含《现代汉语常用字表》中的3500字）。这个表亦代替了1965年出版的《印刷通用汉字字形表》（收6196字），规范了中国大陆的印刷字形标准，另外亦恢复了在《第一批异体字整理表》中被淘汰了的15个异体字为规范字。《现代汉语通用字表》是中国大陆现行规范汉字的依据之一。");
  //   tft.unloadFont();
  // }
  file.close();
  LittleFS.end();
  String tt=getPixBinStrFromString("这是一个自定义汉字软字库的显");
  Serial.println(tt.length());
  for(int i=0;i<tt.length();i++){
    // if(int(i%font_size)==0){
    //   Serial.print("\r\n");
    // }
    if(i%16==0)Serial.print("\r\n");

    if(tt[i]=='0')Serial.print(' ');
    if(tt[i]=='1')Serial.print(tt[i]);
    // Serial.print(tt[i]);
  }
}

void loop()
{
  delay(1);
}

// 关于字模字库文件格式的设想

// 000000 6个16进制，设定字库对应文字的个数。
// 00 设定字模像素值，例如12，16 使用16进制存储 ,由于实际使用范围有限，只考虑12，16，24号字体和之内的字体，每个对应用8的倍数字节来表示。例如12，13，14号字体都用XX来表示，不足的地方补0
// AAAABBBB 连续文字对应的unicode编码。
// 一个字符对应128个字符，32个两位16进制编码，每个字符对应的长度受制于点阵像素值。不同点阵数单个字符对应的16进制编码数量长度不一致
// 00080012002400480090012002400480024001200090004800240012000800000008001200240048009001200240048002400120009000480024001200080000
// 0x0008,0x0012,0x0024,0x0048,0x0090,0x0120,0x0240,0x0480,0x0240,0x0120,0x0090,0x0048,0x0024,0x0012,0x0008,0x0000,0x0008,0x0012,0x0024,0x0048,0x0090,0x0120,0x0240,0x0480,0x0240,0x0120,0x0090,0x0048,0x0024,0x0012,0x0008,0x0000,