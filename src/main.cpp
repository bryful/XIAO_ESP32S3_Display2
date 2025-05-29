#include <LittleFS.h>
#include <WiFi.h>
#include <time.h>
#include <vector>
// #include "LGFX_XIAO_ESP32S3_SPI_ST7789.hpp"
//  #include "LGFX_XIAO_SPI_ST7735S.hpp"
#include "LGFX_XIAO_ESP32S3_SPI_ST7789B.hpp"
#include "FsSerial.hpp"
#include "FsWiFi.hpp"
#include "FsUtils.hpp"
// 準備したクラスのインスタンスを作成します。
LGFX_XIAO_ESP32S3_SPI_ST7789B display;
BrySerial srl;
FsWifi fsWifi;

static unsigned long mmTime = 0;

void DisplayPrint(const char *str)
{
  display.startWrite();
  display.drawBmpFile(LittleFS, "/girl.bmp", 0, 0);
  display.setCursor(0, 20);
  display.setTextColor(TFT_WHITE);
  display.println(str);
  display.endWrite();
}

void printLocalTime()
{
  if (!fsWifi.isConnected())
    return;

  if (fsWifi.UpdateTime())
  {
    display.startWrite();
    display.drawBmpFile(LittleFS, "/girl.bmp", 0, 0);
    display.setTextColor(TFT_RED);
    display.setCursor(20, 0);
    display.println(WiFi.localIP().toString().c_str());
    display.setCursor(20, 210);
    display.println(&fsWifi.timeinfo, "%A, %B %d %Y %H:%M:%S");
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
    char str[] = "XIAO ESP32S3";
    srl.SendBin("info", (uint8_t *)str, strlen(str));
    // DisplayPrint(str);
    //  ExtPrintln("Hello!");
  }
  else if (srl.compHeader(header, (char *)"rset") == true)
  {
    DisplayPrint(header);
    // DisplayPrint("Resetting...");
    // ESP.restart();
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
        printLocalTime();
        // srl.SendText("WiFi connection failed!");
      }
      else
      {
        DisplayPrint("WiFi connection failed!");
      }
    }
  }

  else
  {
    // ExtPrintln("header:" + String(header) + " size:" + String(rsize) + " cnt:" + String(cnt));
  }
}
// -----------------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);

  display.init();
  display.setRotation(3); // 0:横向き, 1:縦向き, 2:横向き反転, 3:縦向き反転
  // display.fillScreen(TFT_RED);
  //display.setBrightness(255);

  LittleFS.begin();
  display.drawBmpFile(LittleFS, "/girl.bmp", 0, 0);
  display.setTextSize(2);

  // display.drawBmpFile(LittleFS, "/girl.bmp", 0, 0);
  mmTime = millis() + 1000;
  DisplayPrint("WiFi connection started...");
  fsWifi.Begin(); // WiFi接続を開始
}
void loop()
{
  unsigned long now = millis();
  GetSerialCMD();

  if (now > mmTime)
  {
    mmTime = now + 1000;
    printLocalTime();
  }
}