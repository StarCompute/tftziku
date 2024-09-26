#include <Arduino.h>
#include <xfont.h>
#ifdef ARDUINO_GFX
    #include "Arduino_GFX_Library.h"
#elif defined(TFT_ESPI)
    #include <TFT_eSPI.h>   
#endif

XFont::XFont()
{
    // Serial.println("Init.");
    // InitTFT();
    XFont(false);
}
#ifdef ARDUINO_GFX
    XFont::XFont(Arduino_GFX *gfx_tft)
    {
        unsigned long beginTime = millis();
        InitTFT(gfx_tft);
        //初始化字库，获得字库中的所有字符集
        Serial.printf("     TFT初始化耗时:%2f 秒.\r\n",(millis() - beginTime)/1000.0);
        beginTime = millis();
        initZhiku(fontFilePath);
        Serial.printf("     装载字符集耗时:%2f 秒.\r\n",(millis() - beginTime)/1000.0);
    }
#endif
XFont::XFont(bool isTFT)
{
    unsigned long beginTime = millis();
    if(isTFT==true)
    {
        InitTFT();
        //初始化字库，获得字库中的所有字符集

        Serial.printf("     TFT初始化耗时:%2f 秒.\r\n",(millis() - beginTime)/1000.0);
    }
    beginTime = millis();
    initZhiku(fontFilePath);
    Serial.printf("     装载字符集耗时:%2f 秒.\r\n",(millis() - beginTime)/1000.0);
}
XFont:: ~XFont(void){
    clear();
}
void XFont:: clear(void){
    if(file)file.close();
    strAllUnicodes=String("");
}

void XFont::InitTFT()
{
    
#ifdef ARDUINO_GFX
    if (!tft->begin())
    {
        Serial.println("gfx->begin() failed!");
    }
    #ifdef GFX_BL
        pinMode(GFX_BL, OUTPUT);
        digitalWrite(GFX_BL, HIGH);
    #endif
    tft->setRotation(1);
    tft->setCursor(10, 10);
    tft->fillScreen(BLACK);
    tft->setTextColor(GREEN);
    // delay(200); // 5 seconds

#elif defined(TFT_ESPI)
    tft.begin();
    
    tft.setRotation(1);

    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_GREEN);
    // tft.drawString("pls waiting!",10, 10);
    tft.drawPixel(1,1,TFT_GREEN);

#endif
    isTftInited=true;

}
#ifdef ARDUINO_GFX
    void XFont:: InitTFT(Arduino_GFX *gfx_tft){
        tft=gfx_tft;
        if (!tft->begin())
        {
            Serial.println("gfx->begin() failed!");
        }
        #ifdef GFX_BL
            pinMode(GFX_BL, OUTPUT);
            digitalWrite(GFX_BL, HIGH);
        #endif
        tft->setRotation(1);
        tft->setCursor(10, 10);
        tft->fillScreen(BLACK);
        tft->setTextColor(GREEN);     
        isTftInited=true;   
    }
#endif  
// 转化字符数组为字符串
String XFont::getStringFromChars(uint8_t *bs, int l)
{
    String ret;
    // ret=(char *)bs;
    // int l=(int)(strlen(*bs));
    // int l=sizeof(bs) ;
    // Serial.println(l);
    // ret.reserve(l);
    // for (int i = 0; i < l; i++)
    // {
    //     ret += (char)bs[i];
    //     // Serial.println(ret.length());
    // }
    // return ret;
    char* input2 = (char *) bs;
    ret = input2;
    return ret.substring(0,l);
    
}

// 把utf8编码字符转unicode编码
String XFont::getUnicodeFromUTF8(String s)
{
    // Serial.println(s.length());
    // 32-127
    char character[s.length()];
    String string_to_hex = "";
    int n = 0;
    for (uint16_t i = 0; i < s.length(); i++)
    {
        // if (s[i] > 0 and s[i] < 32)
        // { // 特判ascii小于 32
        //     if (s[i] == 10 or s[i] == 13)
        //     { // 特判回车换行
        //         string_to_hex += "000a";
        //         continue;
        //     }
        //     if (s[i] == 9)
        //     { // 特判回车换行
        //         string_to_hex += "0009";
        //         continue;
        //     }
        //     s[i] = 32;
        // }
        if (s[i] >= 1 and s[i] <= 127)
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
int XFont::getFontPage(int font_size, int bin_type)
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
String XFont::getPixDataFromHex(String s)
{
    String  ret = "";
    // Serial.println(s);
    int l = s.length();
    char ch[font_size * font_size];
    int cnt=0;
    // String s32 = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ@#*$";
    // 注意，这里是两个字符进行的处理
    int cc = 3;
    // if (bin_type == 32)
    //     cc = 4;
    if (bin_type == 64)
        cc = 5;
    for (int i = 0; i < l; i++)
    {
        // String ch = (String)s[i];
        // int d = s32.indexOf(ch);

        int d=strchr( s64,s[i])-s64;
        // Serial.println(d);
        // 下面用了bitread来获取数字对应的二进制，bitread(value,k)是读取数字value中的二进制的第k位的值。
        // 使用bitread就没有使用getBin这种方式了，但是保留了两种getbin函数

        for (int k = cc; k >= 0; k--)
        {
            ch[cnt]=48+(int)bitRead(d, k);
            // Serial.print(ddd);
            cnt++;
            // ret.concat();
            // strcat(ret,s);
            // strncat(ch,s.c_str(),1);
            // ret += bitRead(d, k);
            // retNoReturn=retNoReturn+(String)sa[k];
        }
    }

    // Serial.println((String)ch);
    // Serial.println(retNoReturn);
    ret=(String)ch;
    return ret.substring(0, font_size * font_size);
    // return "";
}
void XFont::reInitZhiku(String fontPath){
    isInit=false;
    initZhiku( fontPath);
}
void XFont::initZhiku(String fontPath)
{
    if (isInit == true)
        return;
    if(LittleFS.begin()==false){
        Serial.println("littleFS 文件系统初始化失败,请检查相关配置。");
        return;

    }
    if (LittleFS.exists(fontPath))
    {
        file = LittleFS.open(fontPath, "r");

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
        // free(buf_total_str);free(buf_font_size);free(buf_bin_type);
        // Serial.println(s1);
        // Serial.println(s2);
        // Serial.println(s3);
        // Serial.println(font_page);
        // 待读取的总编码长度,每个字都对应着一个uxxxx，所以乘5
        font_unicode_cnt = total_font_cnt * 5;
        // String font_unicode = "";

// 如果是esp32系统，因为ram比较大，一次性读取出所有字符
// #if defined(ESP32)
//         uint8_t *buf_total_str_unicode;
//         buf_total_str_unicode = (uint8_t *)malloc(font_unicode_cnt);
//         Serial.println(font_unicode_cnt);
//         file.read(buf_total_str_unicode, font_unicode_cnt);
//         strAllUnicodes = getStringFromChars(buf_total_str_unicode, font_unicode_cnt);
//         free(buf_total_str_unicode);
// // 如果是esp8266或者其他系统则每次读取1k的内存一直到读完
// // #elif defined (ARDUINO_ARCH_ESP8266)
// #else
        strAllUnicodes="";
        uint8_t *buf_total_str_unicode2;
        int laststr = font_unicode_cnt;
        int read_size=512*2;
        buf_total_str_unicode2 = (uint8_t *)malloc(read_size);
        // file.seek(10);
        do
        {
            size_t k = read_size;
            if (laststr < read_size)
                k = laststr;
            file.read(buf_total_str_unicode2, k);
            strAllUnicodes += getStringFromChars(buf_total_str_unicode2, k);
            laststr -= read_size;
            /* code */
        } while (laststr > 0);
        free(buf_total_str_unicode2);
// #endif

        Serial.printf("     字符总数:%d \r\n",strAllUnicodes.length());
        unicode_begin_idx = 6 + 2 + 2 + total_font_cnt * 5;
        // file.close();
        isInit = true;
    }
    else
    {
        Serial.println("LitteFS系统工作正常，但是找不到字库文件。");
    }
    // LittleFS.end();
}


// 从字库文件获取字符对应的二进制编码字符串
String XFont::getPixBinStrFromString(String strUnicode)
{

    initZhiku(fontFilePath);
    String ret = "";
    // Serial.println(fontPath);
    if  (file)
    {
        // File file = LittleFS.open(fontPath, "r");
        // file.seek(0);
        uint8_t buf_seek_pixdata[font_page];
        String ff = "";
        const char* chrAllUnicodes=strAllUnicodes.c_str();
        for (uint16_t i = 0; i < strUnicode.length(); i = i + 4)
        {
            String _str = "u" + strUnicode.substring(i, i + 4);

            int uIdx = 0;
            // int p = strAllUnicodes.indexOf(_str);
            //下面代码用来查找定位，相对于string的indexof方法，性能相差10倍以上。
            char * chrFind=strstr(chrAllUnicodes,_str.c_str());
            int p =chrFind-strAllUnicodes.c_str();
            Serial.printf("P: %d ,ddd : %d \r\n",p,chrFind-chrAllUnicodes);
            // strAllUnicodes.indexOf
            // Serial.print("p:"+(String)p);

            uIdx = p / 5;
            // Serial.println(uIdx);
            int pixbeginidx = unicode_begin_idx + uIdx * font_page;
            file.seek(pixbeginidx);
            file.read(buf_seek_pixdata, font_page);
            String su = getStringFromChars(buf_seek_pixdata, font_page);
            // Serial.println(su);
            ret += getPixDataFromHex(su);
        }
        // file.close();
    }
    return ret;
}

// 从字库文件获取字符对应的编码字符串
String XFont::getCodeDataFromFile(String strUnicode)
{

    String ret = "";
    // Serial.println(fontPath);
    if (file)
    {
        // File file = LittleFS.open(fontPath, "r");
        // file.seek(0);
        uint8_t buf_seek_pixdata[font_page];
        String ff = "";
        const char* chrAllUnicodes=strAllUnicodes.c_str();
        for (uint16_t i = 0; i < strUnicode.length(); i = i + 4)
        {
            String _str = "u" + strUnicode.substring(i, i + 4);

            int uIdx = 0;
            char * chrFind=strstr(chrAllUnicodes,_str.c_str());
            int p =chrFind-strAllUnicodes.c_str();
            // int p = strAllUnicodes.indexOf(_str);
            // Serial.print("p:"+(String)p);

            uIdx = p / 5;
            // Serial.println(uIdx);
            int pixbeginidx = unicode_begin_idx + uIdx * font_page;
            file.seek(pixbeginidx);
            file.read(buf_seek_pixdata, font_page);
            String su = getStringFromChars(buf_seek_pixdata, font_page);
            // Serial.println(su);
            ret += su;
        }
        // file.close();
    }
    // Serial.println(ret);
    return ret;
}

// 判断是否ansi字符
bool XFont::chkAnsi(unsigned char c)
{
    if (c >= 0 && c <= 127)
        return true;
    return false;
}

void XFont::DrawSingleStr(int x, int y, String strBinData, int fontColor,int backColor, bool ansiChar)
{
       unsigned long beginTime = millis();
// 如果是ansi字符则只显示一半
// int lw = ansiChar == false ? font_size : font_size / 2;
#ifdef ARDUINO_GFX
tft->startWrite();
#endif
    for (uint16_t i = 0; i < strBinData.length(); i++)
    {

        int pX1 = int(i % font_size);
        int pY1 = int(i / font_size);
        if(ansiChar){
            uint16_t brk=i%font_size;
            if(brk>(font_size/2))continue;
        }        
        if (strBinData[i] == '1')
        {
            #ifdef ARDUINO_GFX
            // tft->drawPixel(pX1 + x, pY1 + y, c);
            tft->writePixelPreclipped(pX1 + x, pY1 + y, fontColor);
            #elif defined(TFT_ESPI)
            tft.drawPixel(pX1 + x, pY1 + y,fontColor);
            #endif
        }
        else{
            #ifdef ARDUINO_GFX
            // tft->drawPixel(pX1 + x, pY1 + y, c);
            tft->writePixelPreclipped(pX1 + x, pY1 + y, backColor);
            #elif defined(TFT_ESPI)
            tft.drawPixel(pX1 + x, pY1 + y, backColor);
            #endif                
         
        }
    }

#ifdef ARDUINO_GFX
tft->endWrite();
#endif
// Serial.printf("     字符像素显示耗时:%2f 秒.\r\n",(millis() - beginTime)/1000.0);
time_spent+=millis()  - beginTime;
}

void XFont::DrawSingleStr(int x, int y, String strBinData, int fontColor, bool ansiChar)
{
       unsigned long beginTime = millis();
// 如果是ansi字符则只显示一半
// int lw = ansiChar == false ? font_size : font_size / 2;
#ifdef ARDUINO_GFX
tft->startWrite();
#endif
    for (uint16_t i = 0; i < strBinData.length(); i++)
    {

        if (strBinData[i] == '1')
        {
            int pX1 = int(i % font_size);
            int pY1 = int(i / font_size);
            if(ansiChar){
                uint16_t brk=i%font_size;
                if(brk>(font_size/2))continue;
            }          
#ifdef ARDUINO_GFX
            // tft->drawPixel(pX1 + x, pY1 + y, c);
            tft->writePixelPreclipped(pX1 + x, pY1 + y, fontColor);
#elif defined(TFT_ESPI)
            tft.drawPixel(pX1 + x, pY1 + y, fontColor);
#endif
        }
    }
#ifdef ARDUINO_GFX
tft->endWrite();
#endif
// Serial.printf("     字符像素显示耗时:%2f 秒.\r\n",(millis() - beginTime)/1000.0);
time_spent+=millis()  - beginTime;
}


String XFont::GetPixDatasFromLib(String displayStr){

    initZhiku(fontFilePath);
    if (isInit == false)
    {
        Serial.println("字库初始化失败");
        return "";
    }
    String ret="";

    String strUnicode = getUnicodeFromUTF8(displayStr);
    Serial.println(strUnicode);
    for (uint16_t l = 0; l < strUnicode.length() / 4; l++)
    {

        String childUnicode = strUnicode.substring(4 * l, (4) + 4 * l);
        ret += getPixBinStrFromString(childUnicode);
    }
    return ret;
}


// DrawStr2尝试处理半角英文问题，是对DrawStr的修正。
// 位置计算和字符显示分开
void XFont::DrawStr(int x, int y, String str, int fontColor,int backColor)
{

    initZhiku(fontFilePath);
    if (isInit == false)
    {
        Serial.println("字库初始化失败");
        return;
    }
    unsigned long beginTime = millis();

    String strUnicode = getUnicodeFromUTF8(str);
    Serial.printf("     预处理要显示的汉字耗时:%2f 秒.\r\n",(millis() - beginTime)/1000.0);
    beginTime = millis();

    singleStrPixsAmount = font_size * font_size;

    int px = 0;
    int py = 0;

    px = x;
    py = y;
   

    for (uint16_t l = 0; l < strUnicode.length() / 4; l++)
    {

        String childUnicode = strUnicode.substring(4 * l, (4) + 4 * l);
        String childPixData = getPixBinStrFromString(childUnicode);
        u_int sep=1;//字间距
        // return;
        // // String childPixData=getPixDataFromHex(codeData.substring(font_page * l, font_page + font_page * l));
        int f = 0;
        sscanf(childUnicode.c_str(), "%x", &f);

        if (f <= 127) //如果是ansi字符，则只显示字符数据中的一半，并且显示位置也缩短了一半
        {
           if (f == 13 || f == 10) // 回车和换行\r\n的处理
            {
                px = 0;
                py += font_size + sep;
                continue;
            }
            else if (f == 9) //\t处理
            {
                px += font_size + sep;
                continue;
            }

            if ((px + font_size / 2) > screenWidth)
            {
                // Serial.println(px + font_size / 2);
                px = 0;
                py += font_size + sep;
            }
            //在外层判断backColor，是为了减少内层的每个像素的计算
            if(backColor==-1)DrawSingleStr(px, py, childPixData, fontColor, true);
            else{
                DrawSingleStr(px, py, childPixData, fontColor,backColor, true);
            }

            px += font_size / 2 + sep;
        }
        else
        {
            if ((px + font_size) > screenWidth)
            {
                px = 0;
                py += font_size + sep;
            }
            //在外层判断backColor，是为了减少内层的每个像素的计算
            if(backColor==-1)DrawSingleStr(px, py, childPixData, fontColor, false);
            else{
                DrawSingleStr(px, py, childPixData, fontColor,backColor, false);
            }
            px += font_size + sep;
        }
    }
    
    Serial.printf("     屏幕输出汉字耗时:%2f 秒.\r\n",(millis()  - beginTime)/1000.0);
}
void XFont::DrawStr(int x, int y, String str, int fontColor){
    DrawStr(x,y,str,fontColor,-1);
}
void XFont::DrawChinese(int x, int y, String str, int fontColor)
{
  DrawChinese(x,y,str,fontColor,-1);
}
void XFont::DrawChinese(int x, int y, String str, int fontColor,int backColor){
  if(isTftInited==false){

        Serial.printf(" 调用本方法必须先初始化TFT驱动。.\r\n");
    }
    else{
        DrawStr(x,y,str,fontColor,backColor);
    }
    Serial.printf("     屏幕显示所有汉字耗时:%.3f 秒.\r\n",time_spent/1000.0);
}

void XFont::DrawChineseEx(int x, int y, String str, int fontColor,int backColor)
{
    if(isTftInited==false){
        Serial.printf(" 调用本方法必须先初始化TFT驱动。.\r\n");
    }
    else{
         DrawStrEx(x,y,str,fontColor,backColor);
    }
    // Serial.printf("     屏幕显示所有汉字耗时:%.3f 秒.\r\n",time_spent/1000.0);
}
void XFont::DrawChineseEx(int x, int y, String str, int fontColor){
    DrawChineseEx(x,y,str,fontColor,-1);
}
// DrawStr2尝试处理半角英文问题，是对DrawStr的修正。
// 位置计算和字符显示分开
void XFont::DrawStrEx(int x, int y, String str, int fontColor,int backColor)
{
    initZhiku(fontFilePath);
    if (isInit == false)
    {
        Serial.println("字库初始化失败");
        return;
    }
    unsigned long beginTime = millis();
    //获取字符的unicode编码
    String strUnicode = getUnicodeFromUTF8(str);
    
    // Serial.printf("     预处理要显示的汉字耗时:%.3f 秒.\r\n",(millis() - beginTime)/1000.0);
    beginTime = millis();
    String codeData=getCodeDataFromFile(strUnicode);
    // Serial.println(codeData);
    // Serial.printf("str: %s ，unicode: %s  codedata:%s \r\n",str,strUnicode,codeData);
    singleStrPixsAmount = font_size * font_size;

    int px = x;
    int py = y;

//    int s=codeData.length()/str.length();
// Serial.println(strUnicode);
    for (uint16_t l = 0; l < strUnicode.length() / 4; l++)
    {

        String childUnicode = strUnicode.substring(4 * l, 4 * (l+1));
        //获取对应的字库编码
        String childCodeData = codeData.substring(font_page * l, font_page * (l+1));
        //字库编码转换成像素数据
        String childPixData = getPixDataFromHex(childCodeData);
        int f = 0;
        sscanf(childUnicode.c_str(), "%x", &f);
        u_int sep=1;//字间距
        // Serial.printf("char:%d unicode: %s \r\n", f,childUnicode);


        

        if (f <= 127) //如果是ansi字符，则只显示字符数据中的一半，并且显示位置也缩短了一半
        {
            // Serial.println(f);

            if (f == 13 || f == 10) // 回车和换行\r\n的处理
            {
                px = 0;
                py += font_size + sep;
                continue;
            }
            else if (f == 9) //\t处理
            {
                px += font_size + sep;
                continue;
            }

            //在外层判断backColor，是为了减少内层的每个像素的计算
            if(backColor==-1)DrawSingleStr(px, py, childPixData, fontColor, true);
            else{
                DrawSingleStr(px, py, childPixData, fontColor,backColor, true);
            }

            px += font_size / 2 + sep;
        }
        else
        {
            if ((px + font_size) > screenWidth)
            {
                px = 0;
                py += font_size + sep;
            }
            //在外层判断backColor，是为了减少内层的每个像素的计算
            if(backColor==-1)DrawSingleStr(px, py, childPixData, fontColor, false);
            else{
                DrawSingleStr(px, py, childPixData, fontColor,backColor, false);
            }
            px += font_size + sep;
        }
    }
    
    Serial.printf("     屏幕输出汉字耗时:%2f 秒.\r\n",(millis()  - beginTime)/1000.0);
}

void XFont::DrawStrEx(int x, int y, String str, int fontColor){
    DrawStrEx(x,y,str,fontColor,-1);
}
