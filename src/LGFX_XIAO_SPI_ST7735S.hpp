#pragma once
#ifndef LGFX_XIAO_SPI_ST7735S_HPP
#define LGFX_XIAO_SPI_ST7735S_HPP

// #include <M5GFX.h>
// #include <M5Unified.h>
#include <LovyanGFX.hpp>
#include <lgfx/v1/panel/Panel_ST7735.hpp>

// -----------------------------------------------------------------------------
// 使用したピン
// 3.3V -> VCC
// G    -> GND
constexpr int PIN_SCL = 7; // SCLK(SPI Clock) (SCL)
constexpr int PIN_SDA = 9; // MOSI (SDA)
constexpr int PIN_RST = 1; // Reset
constexpr int PIN_DC = 2;  // Data/Command
constexpr int PIN_BLK = 5; // Backlight
constexpr int PIN_CS = 4;  // Backlight
// -----------------------------------------------------------------------------
/* https://qiita.com/nobrin/items/c9490a2d1e16e849c5c2 の記事を参考に
 */
class LGFX_XIAO_SPI_ST7735S : public lgfx::LGFX_Device
{
    lgfx::Panel_ST7735S _panel_instance; // 接続するパネルの型にあったインスタンスを用意
    lgfx::Bus_SPI _bus_instance;         // パネルを接続するバスの種類にあったインスタンスを用意
    lgfx::Light_PWM _light_instance;     // バックライト制御が可能な場合はインスタンスを用意(必要なければ削除)

public:
    LGFX_XIAO_SPI_ST7735S(void)
    {
        {                                      // バス制御の設定を行います。
            auto cfg = _bus_instance.config(); // バス設定用の構造体を取得

            // 使用するSPIを選択
            // ESP32-S2,C3 : SPI2_HOST or SPI3_HOST / ESP32 : VSPI_HOST or HSPI_HOST
            // (追記) AtomS3ではSPI2_HOST
            // ※ ESP-IDFバージョンアップに伴い、VSPI_HOST , HSPI_HOSTの記述は非推奨になるため、
            // エラーが出る場合は代わりにSPI2_HOST , SPI3_HOSTを使用してください。
            cfg.spi_host = SPI2_HOST;
            cfg.spi_mode = 3;       // SPI通信モードを設定(0-3)
            cfg.pin_sclk = PIN_SCL; // SPIのSCLK(SCL)ピン番号を設定
            cfg.pin_mosi = PIN_SDA; // SPIのMOSI(SDA)ピン番号を設定
            cfg.pin_miso = -1;      // SPIのMISOピン番号を設定 (-1 = disable)
            cfg.pin_dc = PIN_DC;    // SPIのD/C(Data/Command)ピン番号を設定 (-1 = disable)
            // SDカードと共通のSPIバスを使う場合、MISOは省略せず必ず設定してください。
            _bus_instance.config(cfg);              // 設定値をバスに反映します。
            _panel_instance.setBus(&_bus_instance); // バスをパネルにセットします。
        }

        {                                        // 表示パネル制御の設定を行います。
            auto cfg = _panel_instance.config(); // 表示パネル設定用の構造体を取得
            cfg.pin_cs = PIN_CS;                 // CSが接続されているピン番号   (-1 = disable)
            cfg.pin_rst = PIN_RST;               // RSTが接続されているピン番号  (-1 = disable)
            cfg.pin_busy = -1;                   // BUSYが接続されているピン番号 (-1 = disable)
            cfg.panel_width = 128;               // 実際に表示可能な幅
            cfg.panel_height = 160;              // 実際に表示可能な高さ
            cfg.offset_x = 2;                    // パネルのX方向オフセット量
            cfg.offset_y = 1;                    // パネルのY方向オフセット量
            cfg.offset_rotation = 2;             // 回転方向の値のオフセット 0~7 (4~7は上下反転)
            cfg.invert = false;   // パネルの明暗が反転してしまう場合 trueに設定
            cfg.rgb_order = true; // パネルの赤と青が入れ替わってしまう場合 trueに設定
            _panel_instance.config(cfg);
        }

        {                                        // バックライト制御の設定を行います。（必要なければ削除）
            auto cfg = _light_instance.config(); // バックライト設定用の構造体を取得
            cfg.pin_bl = PIN_BLK;                // バックライトが接続されているピン番号
            cfg.invert = faccessat;                  // バックライトの輝度を反転させる場合 true
            cfg.freq = 44100;                    // バックライトのPWM周波数
            cfg.pwm_channel = 7;                 // 使用するPWMのチャンネル番号
            _panel_instance.setLight(&_light_instance); // バックライトをパネルにセット
        }

        setBrightness(255);
        setPanel(&_panel_instance); // 使用するパネルをセット
    }

    void writeLn(const char *str, int x = 0, int y = 0, int bcolor = TFT_BLACK, int color = TFT_WHITE)
    {
        clear(bcolor);
        setTextColor(color);
        setCursor(x, y);
        print(str);
    }
};

// extern LGFX_AtomS3_SPI_ST7735S ExtDisplay; // インスタンスを作成

#endif