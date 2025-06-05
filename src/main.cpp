#include <LittleFS.h>
#include <WiFi.h>
#include <Preferences.h>
#include <time.h>
#include <vector>
#include "DHTesp.h"

#include "LGFX_XIAO_ESP32S3_SPI_ST7789.hpp"
//  #include "LGFX_XIAO_SPI_ST7735S.hpp"
// #include "LGFX_XIAO_ESP32S3_SPI_ST7789B.hpp"
#include "FsSerial.hpp"
#include "FsWiFi.hpp"
#include "FsUtils.hpp"
#include "Face.hpp"
// 準備したクラスのインスタンスを作成します。
LGFX_XIAO_ESP32S3_SPI_ST7789 display;
BrySerial srl;
FsWifi fsWifi;
Face face;

Preferences pref;
char AppID[] = "XIAO-ESP32S3-XXX\0";

// Initialize DHT sensor.
DHTesp dht;
static unsigned long _FaceMM = 0;
static unsigned long mmTime = 0;
static unsigned long mmTemp = 0;

void DisplayPrint(const char *str)
{
  display.startWrite();
  face.Update();
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.setTextColor(TFT_WHITE);
  display.println(str);
  display.endWrite();
  mmTime += 5000;
}
// -----------------------------------------------------------------------------
void getAppID()
{
  pref.begin("app", false);
  char buf[18] = "\0";
  if (pref.getString("appid", buf, 18) > 0)
  {
    strcpy(AppID, buf);
    DisplayPrint(AppID);
  }
  else
  {
    DisplayPrint("ERR");
  }
  pref.end();
  mmTime += 5000;
  // Serial.println(AppID);
}
void setAppID()
{
  pref.begin("app", false);
  size_t b = strlen(AppID);
  size_t a = pref.putString("appid", AppID);
  if (a == b)
  {
    DisplayPrint("OK");
  }
  else
  {
    char ss[256];
    snprintf(ss, 256, "err %d %d", a, b);
    DisplayPrint(ss);
  }
  mmTime += 5000;
  pref.end();
}

void printLocalTime()
{
  if (!fsWifi.isConnected())
    return;

  if (fsWifi.UpdateTime())
  {
    face.tmStr = fsWifi.tmstr();

    display.startWrite();
    face.Draw();
    face.Update();
    display.endWrite();
  }
}

// -----------------------------------------------------------------------------
void GetSerialCMD()
{
  if (Serial.available() < 8)
    return;

  char header[4];
  for (int i = 0; i < 4; ++i)
  {
    header[i] = (char)Serial.read();
  }
  uint32_t rsize = 0;
  const size_t maxSize = 1024; // 最大100KB
  for (int i = 0; i < 4; ++i)
  {
    rsize |= Serial.read() << (i * 8);
  }

  int err = 0;
  int cnt = 0;
  std::vector<uint8_t> dataBuffer;
  if ((rsize > 0) && (rsize < 1024))
  {
    while (dataBuffer.size() < rsize)
    {
      if (Serial.available())
      {
        dataBuffer.push_back(Serial.read());
        cnt++;
      }
      else
      {
        err++;
        if (err > 1000)
        {
          err = -1;
          break;
        }
        delay(10);
      }
    }
  }
  if (srl.compHeader(header, (char *)"info") == true)
  {
    DisplayPrint(AppID);
    srl.SendBin("info", (uint8_t *)AppID, strlen(AppID));
    mmTime = millis() + 5000;
  }
  else if (srl.compHeader(header, (char *)"stid") == true)
  {
    strcpy(AppID, (char *)dataBuffer.data());
    setAppID();
  }
  else if (srl.compHeader(header, (char *)"gtid") == true)
  {
    getAppID();
    srl.SendText(AppID);
  }
  else if (srl.compHeader(header, (char *)"rset") == true)
  {
    string s = string(AppID) + "restart!";
    srl.SendText(s.c_str());
    ESP.restart();
  }
  else if (srl.compHeader(header, (char *)"text") == true)
  {

    // ExtPrintln(String((char *)dataBuffer.data()));
    String str = "return:" + String((char *)dataBuffer.data());
    srl.SendText(str.c_str());
  }
  else if (srl.compHeader(header, (char *)"wifi") == true)
  {
    String str = String((char *)dataBuffer.data());
    FsUtils fsu;
    String *dst = new String[3];
    int idx = fsu.split(str, ',', dst);
    if (idx >= 2)
    {
      // DisplayPrint(String("dst[0]:" + dst[0] + " dst[1]:" + dst[1]).c_str());

      // Serial.println("dst[0]:" + dst[0] + " dst[1]:" + dst[1]);
      if (fsWifi.Begin(dst[0].c_str(), dst[1].c_str()))
      {
        face.ipstr = fsWifi.ipstr();
        printLocalTime();
        // srl.SendText("WiFi connection failed!");
      }
      else
      {
        DisplayPrint("WiFi connection failed!");
      }
    }
  }
}
// -----------------------------------------------------------------------------
void NextFaceTime()
{
  unsigned long v;
  if (face.EyeMode() >= EyeMode::EX1)
  {
    v = random(400, 1500);
  }
  else
  {
    v = random(500, 4000);
  }
  _FaceMM = millis() + v;
}

// -----------------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);

  display.init();
  display.setRotation(3); // 0:横向き, 1:縦向き, 2:横向き反転, 3:縦向き反転

  getAppID();

  DisplayPrint(AppID);

  LittleFS.begin();
  face.Begin(&display);
  face.ChangeFace();
  face.Draw();
  face.Update();

  display.setTextSize(2);

  DisplayPrint("WiFi Started...");
  fsWifi.Begin(); // WiFi接続を開始
  face.ipstr = fsWifi.ipstr();
  face.tmStr = fsWifi.tmstr();
  mmTime = millis() + 1000;
  mmTemp = millis() + 2000;
  NextFaceTime();
  dht.setup(44, DHTesp::DHT11); // Connect DHT sensor to GPIO 17
  face.humidity = dht.getHumidity();
  face.temperature = dht.getTemperature();
  face.dht = String(dht.getStatusString());
}
void loop()
{
  unsigned long now = millis();
  GetSerialCMD();

  if (now > mmTemp)
  {
    face.humidity = dht.getHumidity();
    face.temperature = dht.getTemperature();
    face.dht = String(dht.getStatusString());
    Serial.println(dht.getStatusString());
    mmTemp = millis() + 2000;
    printLocalTime();
  }
  else if (now > mmTime)
  {
    mmTime = now + 1000;
    printLocalTime();
  }
  else if (now > _FaceMM)
  {

    face.DrawEyeBlink();

    NextFaceTime();
    // Serial.printf("%d\n", now);
  }
}