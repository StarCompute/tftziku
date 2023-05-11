#include <Arduino.h>
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
String getStringFromChars2(uint8_t *bs, int l)
{
    String ret;
    ret=(char *)bs;
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


String getUnicodeFromUTF82(String s)
{
    Serial.println(s.length());
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
    Serial.println(string_to_hex);
    return string_to_hex;
}

String getUnicodeFromUTF8(String s)
{

    char buffer[s.length()];
    strcpy(buffer, s.c_str());
    int BSIZE = (sizeof(buffer) / 3);
    // int BSIZE = s.length();
    char character[BSIZE * 2];
    // Serial.println(BSIZE);
    String string_to_hex = "";

    for (int i = 0; i < BSIZE; i = i + 1)
    {
        character[i * 2 + 1] = ((buffer[i * 3 + 1] & 0x3) << 6) + (buffer[i * 3 + 2] & 0x3F);
        character[i * 2] = ((buffer[i * 3] & 0xF) << 4) + ((buffer[i * 3 + 1] >> 2) & 0xF);
        String ss1 = String(character[i * 2], HEX);
        String ss2 = String(character[i * 2 + 1], HEX);
        // 下面的代码用于处理出现x xx和xx x这种情况，补足前面的0为0x xx或者xx 0x
        string_to_hex += ss1.length() == 1 ? "0" + ss1 : ss1;
        string_to_hex += ss2.length() == 1 ? "0" + ss2 : ss2;
        // string_to_hex += String(character[i * 2 + 1], HEX);
    }
    // int i0 = (int)character[0];
    // int i1 = (int)character[1];

    // string_to_hex += String(character[0], HEX);
    // string_to_hex += String(character[1], HEX);
    // Serial.println(character[0]);
    // Serial.println(character[1]);
    Serial.println(string_to_hex);
    return string_to_hex;
}

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


String getPixDataFromHex(String s)
{
    String ret = "";
    String retNoReturn;
    // Serial.println(s);
    int l = s.length();
    for (int i = 0; i < l; i = i + 2)
    {
        // if (i % 4 == 0)
        //     ret += "\r\n";
        // int *sa;
        String ch = (String)s[i] + (String)s[i + 1];
        int d = 0;
        sscanf(ch.c_str(), "%x", &d);
        // 下面用了bitread来获取数字对应的二进制，bitread(value,k)是读取数字value中的二进制的第k位的值。
        for (int k = 7; k >=0; k--)
        {
            // Serial.print(sa[k]);
            // ret = ret + bitRead(d,k);
            retNoReturn+=bitRead(d,k);
            // retNoReturn=retNoReturn+(String)sa[k];
        }
    }

    // Serial.println(ret);
    // Serial.println(retNoReturn);
    return retNoReturn;
}
