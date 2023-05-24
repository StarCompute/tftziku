#include <Arduino.h>
#include <LittleFS.h>

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

// 转化字符数组为字符串，下面这种方法看起来最简单，但是末尾多了一个尾巴。
String getStringFromChars2(uint8_t *bs, int l)
{
    String ret;
    ret = (char *)bs;
    // int l=(int)(strlen(*bs));
    // int l=sizeof(bs) ;
    // Serial.println(l);
    // for (int i = 0; i < l; i++)
    // {
    //     ret += (char)bs[i];
    // }
    // Serial.println(ret);
    return ret;
}

// 从字体文件中获取字符的总数量
int getStrCountFromFontFile(uint8_t *bufs)
{
    static uint8_t buf_total_str[6];
    String strCount = getStringFromChars(bufs, 6);
    // 因为使用的是16进制存储，所以必须加一次转换
    return strtoll(strCount.c_str(), NULL, 16);
}
// 从字体文件中获取字符字号
int getFontSizeFromFontFile(uint8_t *bufs)
{
    String strFontSize = getStringFromChars(bufs, 2);
    return strFontSize.toInt();
}

// 把utf8编码字符转unicode编码
String getUnicodeFromUTF82(String s)
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

// String getUnicodeFromUTF8(String s)
// {

//     char buffer[s.length()];
//     strcpy(buffer, s.c_str());
//     int BSIZE = (sizeof(buffer) / 3);
//     // int BSIZE = s.length();
//     char character[BSIZE * 2];
//     // Serial.println(BSIZE);
//     String string_to_hex = "";

//     for (int i = 0; i < BSIZE; i = i + 1)
//     {
//         character[i * 2 + 1] = ((buffer[i * 3 + 1] & 0x3) << 6) + (buffer[i * 3 + 2] & 0x3F);
//         character[i * 2] = ((buffer[i * 3] & 0xF) << 4) + ((buffer[i * 3 + 1] >> 2) & 0xF);
//         String ss1 = String(character[i * 2], HEX);
//         String ss2 = String(character[i * 2 + 1], HEX);
//         // 下面的代码用于处理出现x xx和xx x这种情况，补足前面的0为0x xx或者xx 0x
//         string_to_hex += ss1.length() == 1 ? "0" + ss1 : ss1;
//         string_to_hex += ss2.length() == 1 ? "0" + ss2 : ss2;
//         // string_to_hex += String(character[i * 2 + 1], HEX);
//     }
//     // int i0 = (int)character[0];
//     // int i1 = (int)character[1];

//     // string_to_hex += String(character[0], HEX);
//     // string_to_hex += String(character[1], HEX);
//     // Serial.println(character[0]);
//     // Serial.println(character[1]);
//     // Serial.println(string_to_hex);
//     return string_to_hex;
// }

// 把16进制字符转化成二进制
int *getBin2(uint8_t data)
{
    static int a[8];
    for (int i = 0; i < 8; i++)
    {
        if (data & 0x01)
            a[i] = 1; // 1
        else
            a[i] = 0; // 0
        data >>= 1;
    }
    return a;
}

// 十进制转二进制字符
int *getBin(int Dec)
{
    //  Dec=128;
    static int a[8];
    for (int i = 7; i >= 0; i--)
    {
        if (pow(2, i) <= Dec)
        {
            Dec = Dec - pow(2, i);
            a[7 - i] = 1;
        }
        else
        {
            a[7 - i] = 0;
        }
    }
    return a;
};

// 从字符的像素16进制字符重新转成二进制字符串
String getPixDataFromHex(String s)
{
    // String ret = "";
    String retNoReturn;
    // Serial.println(s);
    int l = s.length();
    // 注意，这里是两个字符进行的处理
    for (int i = 0; i < l; i = i + 2)
    {
        String ch = (String)s[i] + (String)s[i + 1];
        int d = 0;
        sscanf(ch.c_str(), "%x", &d);
        // 下面用了bitread来获取数字对应的二进制，bitread(value,k)是读取数字value中的二进制的第k位的值。
        // 使用bitread就没有使用getBin这种方式了，但是保留了两种getbin函数
        for (int k = 7; k >= 0; k--)
        {
            // Serial.print(sa[k]);
            retNoReturn += bitRead(d, k);
            // retNoReturn=retNoReturn+(String)sa[k];
        }
    }

    // Serial.println(ret);
    // Serial.println(retNoReturn);
    return retNoReturn;
}

// 从字库文件获取字符对应的二进制编码字符串
String getPixBinStrFromString(String displayString,String fontPath)
{

    LittleFS.begin();

    File file = LittleFS.open(fontPath);
    static uint8_t buf_total_str[6];
    static uint8_t buf_fontsize[2];
    // Serial.println(file.position());
    file.read(buf_total_str, 6);
    // Serial.println(file.position());
    file.read(buf_fontsize, 2);

    String s1 = getStringFromChars(buf_total_str, 6);
    String s2 = getStringFromChars(buf_fontsize, 2);
    int total_font_cnt = strtoll(s1.c_str(), NULL, 16);
    int font_size = s2.toInt();


    // Serial.println(s1);
    // Serial.println(total_font_cnt);
    // Serial.println(font_size);
    int font_unicode_cnt = total_font_cnt * 5;
    String font_unicode = "";
    uint8_t *buf_total_str_unicode;
    int font_page = int(font_size * font_size / 8 * 2);
   
    uint8_t buf_seek_pixdata[font_page];

    buf_total_str_unicode = (uint8_t *)malloc(font_unicode_cnt);
    file.read(buf_total_str_unicode, font_unicode_cnt);
    String strUnicodes = getStringFromChars2(buf_total_str_unicode, font_unicode_cnt);
    free(buf_total_str_unicode);
    String strUnicode = getUnicodeFromUTF82(displayString);
    // Serial.println(strUnicode.length());
    int unicode_begin_idx = 6 + 2 + total_font_cnt * 5;
    // Serial.println("begin");
    //  Serial.println(font_page);
    String ff = "";
    String ret = "";
    for (int i = 0; i < strUnicode.length(); i = i + 4)
    {
        String _str = "u" + strUnicode.substring(i, i + 4);
        file.seek(8);
        String asd = "";
        int readFileSize = 5 * 400;
        uint8_t buf_read_pixdata[readFileSize];

        // int cnt_page = (total_font_cnt + 6) / 6;'
        int cnt_page = total_font_cnt / 400 + 1;
        int uIdx = 0;
        int p = strUnicodes.indexOf(_str);
        uIdx = p / 5;
        int pixbeginidx = unicode_begin_idx + uIdx * font_page;
        file.seek(pixbeginidx);
        file.read(buf_seek_pixdata, font_page);
        String su = getStringFromChars(buf_seek_pixdata, font_page);

        ret += getPixDataFromHex(su);
    }
    file.close();
    LittleFS.end();
    return ret;
}
