#include <Arduino.h>
#include <LittleFS.h>
#include <TFT_eSPI.h>

String strUnicodes = "";
int unicode_begin_idx = 0;
int font_unicode_cnt = 0;
int total_font_cnt = 0;
int bin_type = 16;
int font_size = 0;
int font_page = 0;
bool isInit = false;
String fontFilePath = "/x.font";

int pX = 16;
int pY = 0;
// int fontsize = 12;      // 字号
int amountDisplay = 10; // 每行显示多少汉字，其实这个显示数量应该通过屏幕的宽度来计算字号
// 下面的屏幕宽和高可以自行设定，在本处由于使用了tft屏幕驱动中的参数
// int screenWidth = TFT_WIDTH;
int screenHeight = TFT_HEIGHT;

int screenWidth = 160;
// int screenHeight=128;
int singleStrPixsAmount = 0;

// 转化字符数组为字符串
String getStringFromChars(uint8_t *bs, int l)
{
    String ret;
    // ret=(char *)bs;
    // int l=(int)(strlen(*bs));
    // int l=sizeof(bs) ;
    // Serial.println(l);
    for (int i = 0; i < l; i++)
    {
        ret += (char)bs[i];
    }
    return ret;
}


// 把utf8编码字符转unicode编码
String getUnicodeFromUTF8(String s)
{
    // Serial.println(s.length());
    // 32-127
    char character[s.length()];
    String string_to_hex = "";
    int n = 0;
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] >= 32 and s[i] <= 127)
        {
            // Serial.print(s[i]+" :");
            // Serial.println(s[i],DEC);
            String ss1 = String(s[i], HEX);
            ss1 = ss1.length() == 1 ? "0" + ss1 : ss1;
            string_to_hex += "00" + ss1;
        }
        else
        {
            character[n + 1] = ((s[i + 1] & 0x3) << 6) + (s[i + 2] & 0x3F);
            character[n] = ((s[i] & 0xF) << 4) + ((s[i + 1] >> 2) & 0xF);
            String ss1 = String(character[n], HEX);
            String ss2 = String(character[n + 1], HEX);
            // 下面的代码用于处理出现x xx和xx x这种情况，补足前面的0为0x xx或者xx 0x
            string_to_hex += ss1.length() == 1 ? "0" + ss1 : ss1;
            string_to_hex += ss2.length() == 1 ? "0" + ss2 : ss2;
            n = n + 2;
            i = i + 2;
        }
    }
    // Serial.println(string_to_hex);
    return string_to_hex;
}

// 依照字号和编码方式计算每个字符存储展位
int getFontPage(int font_size, int bin_type)
{
    int total = font_size * font_size;
    int hexCount = 8;
    if (bin_type == 32)
        hexCount = 10;
        if (bin_type == 64)
        hexCount = 12;
    int hexAmount = int(total / hexCount);
    if (total % hexCount > 0)
    {
        hexAmount += 1;
        // total+=hexCount-total%hexCount;
    }
    //  print(hexAmount*2,total)
    return hexAmount * 2;
}


// 从字符的像素16进制字符重新转成二进制字符串
String getPixDataFromHex(String s)
{
    String ret = "";
    // Serial.println(s);
    int l = s.length();
    String s32="0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ@#*$";
    // 注意，这里是两个字符进行的处理
    int cc=3;
    if (bin_type==32)cc=4;
    if (bin_type==64)cc=5;
    for (int i = 0; i < l; i ++)
    {
        String ch = (String)s[i] ;
        int d = s32.indexOf(ch);
        // Serial.println(d);
        // 下面用了bitread来获取数字对应的二进制，bitread(value,k)是读取数字value中的二进制的第k位的值。
        // 使用bitread就没有使用getBin这种方式了，但是保留了两种getbin函数

        for (int k = cc; k >= 0; k--)
        {
            // Serial.print(sa[k]);
            ret += bitRead(d, k);
            // retNoReturn=retNoReturn+(String)sa[k];
        }
    }

    // Serial.println(ret);
    // Serial.println(retNoReturn);
    return ret.substring(0,font_size*font_size);
}

void initZhiku(String fontPath)
{
    if (isInit == true)
        return;
    LittleFS.begin();
    if (LittleFS.exists(fontPath))
    {
        File file = LittleFS.open(fontPath);

        static uint8_t buf_total_str[6];
        static uint8_t buf_font_size[2];
        static uint8_t buf_bin_type[2];
        // Serial.println(file.position());
        file.read(buf_total_str, 6);
        // Serial.println(file.position());
        file.read(buf_font_size, 2);
        file.read(buf_bin_type, 2);
        // 下面代码获取总字数和字号
        String s1 = getStringFromChars(buf_total_str, 6);
        String s2 = getStringFromChars(buf_font_size, 2);
        String s3 = getStringFromChars(buf_bin_type, 2);
        total_font_cnt = strtoll(s1.c_str(), NULL, 16);
        font_size = s2.toInt();
        bin_type = s3.toInt();
        font_page = getFontPage(font_size, bin_type);
        // Serial.println(s1);
        // Serial.println(s2);
        // Serial.println(s3);
        // Serial.println(font_page);
        // 待读取的总编码长度,每个字都对应着一个uxxxx，所以乘5
        font_unicode_cnt = total_font_cnt * 5;
        // String font_unicode = "";
        uint8_t *buf_total_str_unicode;
        buf_total_str_unicode = (uint8_t *)malloc(font_unicode_cnt);
        file.read(buf_total_str_unicode, font_unicode_cnt);
        strUnicodes = getStringFromChars(buf_total_str_unicode, font_unicode_cnt);
        // Serial.println(strUnicodes.length());
        free(buf_total_str_unicode);
        unicode_begin_idx = 6 + 2 + 2 + total_font_cnt * 5;
        file.close();
        isInit = true;
    }
    else{
        Serial.println("找不到字库文件。");
    }
    // LittleFS.end();
    
}

// 从字库文件获取字符对应的二进制编码字符串
String getPixBinStrFromString(String strUnicode, String fontPath)
{

    String ret = "";
    // Serial.println(fontPath);
    if (LittleFS.exists(fontPath))
    {
        File file = LittleFS.open(fontPath);
        uint8_t buf_seek_pixdata[font_page];
        String ff = "";
        for (int i = 0; i < strUnicode.length(); i = i + 4)
        {
            String _str = "u" + strUnicode.substring(i, i + 4);

            // int cnt_page = (total_font_cnt + 6) / 6;'
            int cnt_page = total_font_cnt / 400 + 1;
            int uIdx = 0;
            int p = strUnicodes.indexOf(_str);
            uIdx = p / 5;
            // Serial.println(uIdx);
            int pixbeginidx = unicode_begin_idx + uIdx * font_page;
            file.seek(pixbeginidx);
            file.read(buf_seek_pixdata, font_page);
            String su = getStringFromChars(buf_seek_pixdata, font_page);
            // Serial.println(su);
            ret += getPixDataFromHex(su);
        }
        file.close();
    }
    return ret;
}

// 判断是否ansi字符
bool chkAnsi(unsigned char c)
{
    if (c >= 0 && c <= 127)
        return true;
    return false;
}
void DrawSingleStr(TFT_eSPI &tftOutput, int x, int y, String strBinData, int c, bool ansiChar)
{
    // 如果是ansi字符则只显示一半
    int lw = ansiChar == false ? font_size : font_size / 2;
    for (int i = 0; i < strBinData.length(); i++)
    {

        if (strBinData[i] == '1')
        {
            int pX1 = int(i % font_size);
            int pY1 = int(i / font_size);
            tftOutput.drawPixel(pX1 + x, pY1 + y, c);
        }
    }
}
// DrawStr2尝试处理半角英文问题，是对DrawStr的修正。
// 位置计算和字符显示分开
void DrawStr2(TFT_eSPI &tftOutput, int x, int y, String str, int c)
{
    initZhiku(fontFilePath);
    if(isInit==false){
        Serial.println("字库初始化失败");
        return;
    }
    // return;
    // Serial.println("Init end.");
    String strUnicode = getUnicodeFromUTF8(str);
    singleStrPixsAmount = font_size * font_size;
    String strBinData = getPixBinStrFromString(strUnicode, fontFilePath);
    // Serial.println(strBinData);
    int px = 0;
    int py = 0;
    // for (int i = 0; i < strBinData.length() / singleStrPixsAmount; i++)
    // {
    //     px = x + font_size * i;
    //     py = y;
    //     DrawSingleStr(tftOutput, px, py, strBinData.substring(singleStrPixsAmount * i, (singleStrPixsAmount) + singleStrPixsAmount * i), c, true);
    // }
    px = x;
    py = y;
    for (int l = 0; l < strUnicode.length() / 4; l++)
    {

        String childUnicode = strUnicode.substring(4 * l, (4) + 4 * l);
        int f = 0;
        sscanf(childUnicode.c_str(), "%x", &f);
        if (f <= 127)
        {
            if ((px + font_size / 2) > screenWidth)
            {
                // Serial.println(px + font_size / 2);
                px = 0;
                py += font_size;
            }
            DrawSingleStr(tftOutput, px, py, strBinData.substring(singleStrPixsAmount * l, (singleStrPixsAmount) + singleStrPixsAmount * l), c, true);
            px += font_size / 2;
        }
        else
        {
            if ((px + font_size) > screenWidth)
            {
                px = 0;
                py += font_size;
            }
            DrawSingleStr(tftOutput, px, py, strBinData.substring(singleStrPixsAmount * l, (singleStrPixsAmount) + singleStrPixsAmount * l), c, true);
            px += font_size;
        }
    }
}
void DrawStr(TFT_eSPI &tftOutput, int x, int y, String str, int color)
{
    initZhiku(fontFilePath);
    singleStrPixsAmount = font_size * font_size;
    // 下面的代码显示对应的汉字在TFT屏幕上

    String strBinData = getPixBinStrFromString(str, fontFilePath);
    // Serial.println(strBinData);
    amountDisplay = screenWidth / font_size; // 如果不愿意动态计算显示数量可以注释调这一行
    int l1 = singleStrPixsAmount * amountDisplay;
    int l2 = font_size * amountDisplay;
    for (int i = 0; i < strBinData.length(); i++)
    {

        if (strBinData[i] == '1')
        {
            pX = int(i % font_size) + int(i / singleStrPixsAmount) * font_size - int(i / l1) * l2;

            // 对于pY,每fontsize个像素后+1，每singleStrPixsAmount个像素后归0，同时每换一行，pY要加上fontsize个像素；
            pY = int((i - int(i / singleStrPixsAmount) * singleStrPixsAmount) / font_size) + int(i / l1) * font_size;

            tftOutput.drawPixel(pX + x, pY + y, color);
        }
        // else
        // {
        //   // tft.drawPixel(pX + x, pY + y, TFT_BLACK);
        // }
    }
}
