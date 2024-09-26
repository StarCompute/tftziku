#include <Arduino.h>
#include <LittleFS.h>

// 注意，使用不同的屏幕驱动应该对下面的代码进行不同的注释
#define ARDUINO_GFX
// #define TFT_ESPI

#ifdef ARDUINO_GFX
#include "Arduino_GFX_Library.h"
#elif defined(TFT_ESPI)
#include <TFT_eSPI.h>
#endif

using std::wstring;

class XFont
{

public:
    //默认初始化方法，不初始化TFT驱动
    XFont();
    //带参数初始化方法，isTFT=true初始化TFT驱动
    XFont(bool isTFT);

    File file;
    bool isInit = false;
#ifdef ARDUINO_GFX

    XFont(Arduino_GFX *gfx_tft);
    Arduino_DataBus *bus = create_default_Arduino_DataBus();
    // 注意，不同的tft屏幕驱动选择不同的对象
    Arduino_GFX *tft = new Arduino_ST7735(bus,-1,0,false,128,160,0,0,0,0,false);

#define GFX_BL DF_GFX_BL

#elif defined(TFT_ESPI)
    TFT_eSPI tft = TFT_eSPI();
#endif

    void DrawStr(int x, int y, String str, int fontColor);
    void DrawStr(int x, int y, String str, int fontColor,int backColor);
    void DrawStrEx(int x, int y, String str, int fontColor);
    void DrawStrEx(int x, int y, String str, int fontColor,int backColor);

    // 在指定位置输出中文，本方法边读字库边显示
    void DrawChinese(int x, int y, String str, int fontColor);
    void DrawChinese(int x, int y, String str, int fontColor,int backColor);

    // 在指定位置输出中文,本方法是字库读完后，统一显示，视觉上显示最快
    void DrawChineseEx(int x, int y, String str, int fontColor);
    void DrawChineseEx(int x, int y, String str, int fontColor,int backColor);

    //直接从字库获得指定字符的像素编码,方便用于像素屏，例如
    //GetPixDatasFromLib("我"),返回：“000110100000011100100100000100100010000100100000111111111110000100100000000100100100000110101000001100010000110100110010000101001010001100000110”
    String GetPixDatasFromLib(String displayStr);

    // 初始化字库
    void initZhiku(String fontPath);
    //重新初始化新的字库
    void reInitZhiku(String fontPath);
    //清除内存占用，释放40k的空间
    void clear(void);
     ~XFont(void);

#ifdef ARDUINO_GFX
    void InitTFT(Arduino_GFX *gfx_tft);
#endif  
protected:
    void InitTFT();
    // 转化字符数组为字符串
    String getStringFromChars(uint8_t *bs, int l);
    // 把utf8编码字符转unicode编码
    String getUnicodeFromUTF8(String s);
    // 从字符的像素16进制字符重新转成二进制字符串
    String getPixDataFromHex(String s);

    void DrawSingleStr(int x, int y, String strBinData, int fontColor,int backColor, bool ansiChar);
   void DrawSingleStr(int x, int y, String strBinData, int fontColor, bool ansiChar);
    // 在屏幕上输出字符，暂时保留这个方法
    // void DrawStr(int x, int y, String str, int color);


    // 从字库文件获取字符对应的编码字符串
    String getCodeDataFromFile(String strUnicode);

    // 依照字号和编码方式计算每个字符存储展位
    int getFontPage(int font_size, int bin_type);

    // 从字库文件获取字符对应的二进制编码字符串
    String getPixBinStrFromString(String displayStr);

    // 所有字符的unicode编码
    String strAllUnicodes = "";
    int unicode_begin_idx = 0;
    int font_unicode_cnt = 0;
    int total_font_cnt = 0;
    int bin_type = 64;
    int font_size = 0;
    int font_page = 0;
    
    unsigned long time_spent = 0;
    String fontFilePath = "/x.font";
    const char *s64 = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ@#*$";
    int pX = 16;
    int pY = 0;
    bool isTftInited=false;

    // 注意现在的显示字数是自动根据屏幕大下和字体大小计算的，以下暂时未生效，但是保留
    int amountDisplay = 10; // 每行显示多少汉字，其实这个显示数量应该通过屏幕的宽度来计算字号

    // 注意：如果屏幕设置了不同的旋转方式，以下代码应该对应调整
#ifdef ARDUINO_GFX
    int screenHeight = 240;
    int screenWidth = 320;
#elif defined(TFT_ESPI)
    int screenHeight = TFT_WIDTH;
    int screenWidth = TFT_HEIGHT;
#endif

    // int screenHeight=128;
    int singleStrPixsAmount = 0;

private:
    bool chkAnsi(unsigned char c);
};
