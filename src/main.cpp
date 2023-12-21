#include <Arduino.h>
#include <painlessMesh.h>
#include <TFT_eSPI.h>
#include <LittleFS.h>
#include "common.h"

// 注意tft屏幕的借口配置在相关的文件中
TFT_eSPI tft = TFT_eSPI();


// 下面的代码处理wifi mesh
#define MESH_PREFIX "kaka"
#define MESH_PASSWORD "somethingSneaky"
#define MESH_PORT 5557
painlessMesh mesh;
Scheduler userScheduler; // to control your personal task
void receivedCallback(uint32_t from, String &msg)
{
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  // DrawStr(10,50,msg,TFT_WHITE);
  if(msg.indexOf("light")>=0){
    msg.replace("light:","");
    tft.setTextFont(2);
    tft.fillRect(10+16*4,34,100,16,TFT_BLACK);
    // tft.setTextColor(TFT_GREEN);
    tft.drawString(msg+"(lx)",10+16*4,34);
  }
  else if(msg.indexOf("humidity")>=0){
    msg.replace("humidity:","");
    tft.setTextFont(2);
    tft.fillRect(10+16*5,18,100,16,TFT_BLACK);
    // tft.setTextColor(TFT_GREEN);
    tft.drawString(msg,10+16*5,18);
  }
    else if(msg.indexOf("air_temperature")>=0){
    msg.replace("air_temperature:","");
    tft.setTextFont(2);
    tft.fillRect(10+16*5,2,100,16,TFT_BLACK);
    // tft.setTextColor(TFT_GREEN);
    tft.drawString(msg,10+16*5,2);
  }
}

void newConnectionCallback(uint32_t nodeId)
{
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback()
{
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset)
{
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void setup()
{

  Serial.begin(115200);
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);

  // String strBinDisplay = getPixBinStrFromString("好软,这是一个自定义软字库的显示。欢迎你的使用！这个软字库有七千个汉字，基本囊括了日常使用的汉字内容。", "/x.font");

  DrawStr(tft,10, 2, "空气温度：", TFT_GREEN);
  DrawStr(tft,10, 18, "土壤湿度：", TFT_RED);
  DrawStr(tft,10, 34, "光照度：", TFT_DARKGREEN);
  // DrawStr(2, 2, "GB2312 gb abcdefg 我看这世界繁华，都是浮云一般无二。而倘若你尚未体验这繁华，这世界又同你有什么关系。好软,这是一个自定义软字库的显示。欢迎你的使用！这个软字库有七千个汉字，基本囊括了日常使用的汉字内容。", TFT_GREEN);
  mesh.setDebugMsgTypes(ERROR | STARTUP); // set before init() so that you can see startup messages

  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
}

void loop()
{
  delay(1);
  mesh.update();
}

 