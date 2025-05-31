#pragma once
#ifndef FACE_320_240_HPP
#define FACE_320_240_HPP

#include <LittleFS.h>
#include <arduino.h>
#include <LovyanGFX.hpp>
#include <FS.h>
#include <lgfx/v1/lgfx_fonts.hpp> // bgr888_t型
/*
    0 透明色（肌色）0000FF
    1 ホホの色（ピンク）
    2 主線の色（黒）
    3 目の色１（赤暗い）
    4 目の色２（赤明るい）
    5 目の色3（赤明るい2）
    6 目のハイライト（白）
    7 白 白目 歯
    8 口の色（赤）
    10 黒
    11 赤
    12 青
    13 緑
    14 黄色
    14 マゼンダ
    15 シアン

*/
namespace FacePallet
{
    typedef enum PalType
    {
        Skin = 0,
        Hoho,
        Line,
        Eye1,
        Eye2,
        Eye3,
        EyeHi,
        EyeWhite,
        White,
        Mouth,
        Black,
        Red,
        Blue,
        Green,
        Yellow,
        Magenda,
        SCyan

    } PaletteType;
};
namespace EyeMode
{
    typedef enum EYEMODE
    {
        CENTER = 0,
        RIGHT,
        LEFT,
        EX1,
        EX2,
        EX3,

    } EyeMode;
};
namespace MouthMode
{
    typedef enum MOUTHMODE
    {
        M1 = 0,
        M2,
        M3,
        M4,
        M5,
        M6,

    } EyeMode;
};

#define SKIN_NUMBER 0

#define DisplayWidth 240
#define DisplayHeight 240
#define WidthCenter (DisplayWidth / 2)

#define EyeBufWidth 160
#define EyeBufHeight 160
#define EyeEXBufWidth 150
#define EyeEXBufHeight 140
#define MayuBufWidth 130
#define MayuBufHeight 30
#define MouthBufWidth 120
#define MouthBufHeight 48

#define HohoRX 24
#define HohoRY 12

class Face
{
private:
    bool _isSetup;

    int _MouseLeft = WidthCenter - MouthBufWidth / 2;
    int _MouseTop = 185;

    int _HohoTop = 190;
    int _HohoRX = 90;

    int _EyeRX = 80;
    int _EyeTop = 130;

    int _EyeEXRX = 80;
    int _EyeEXTop = 120;

    int _MayuRX = 80;
    int _MayuTop = 40;

    int _EyeMode = EyeMode::CENTER;
    int _MouthMode = MouthMode::M2;
    lgfx::LGFX_Device *display;
    u_int16_t _SkinColor = 0xFDD2;
    u_int16_t palette[16] = {
        0xFDD2,
        0xDC1C,
        0x38A3,
        0x6861,
        0xE828,
        0xF5D9,
        0xE7DF,
        0xFFFF,
        0xFCD9,
        0x0,
        0xD848,
        0x153F,
        0x23E0,
        0xF76A,
        0xE25F,
        0x2FFE};
    // 目パチ用の裏画面
    lgfx::LGFX_Sprite *Comp;

    lgfx::LGFX_Sprite *eye00;
    lgfx::LGFX_Sprite *eyec0;
    lgfx::LGFX_Sprite *eyec1;
    lgfx::LGFX_Sprite *eyel0;
    lgfx::LGFX_Sprite *eyel1;
    lgfx::LGFX_Sprite *eyer0;
    lgfx::LGFX_Sprite *eyer1;
    lgfx::LGFX_Sprite *mayu;
    lgfx::LGFX_Sprite *eyeex1;
    lgfx::LGFX_Sprite *eyeex2;
    lgfx::LGFX_Sprite *eyeex3;
    lgfx::LGFX_Sprite *mouth1;
    lgfx::LGFX_Sprite *mouth2;
    lgfx::LGFX_Sprite *mouth3;
    lgfx::LGFX_Sprite *mouth4;
    lgfx::LGFX_Sprite *mouth5;
    lgfx::LGFX_Sprite *mouth6;

    void setupSprite(lgfx::LGFX_Sprite *d, const char *f, int w, int h, bool isCenter = true);
    void DrawSkin();
    void DrawMouth();
    void DrawHoho();
    void DrawEyeClose();
    void DrawEyeInt();

    long FaceModeTable[11] = {
        EyeMode::CENTER,
        EyeMode::CENTER,
        EyeMode::CENTER,
        EyeMode::CENTER,
        EyeMode::LEFT,
        EyeMode::LEFT,
        EyeMode::RIGHT,
        EyeMode::RIGHT,
        EyeMode::EX1,
        EyeMode::EX2,
        EyeMode::EX3};

public:
    Face();
    ~Face();
    void Begin(lgfx::LGFX_Device *d);
    void SetPalette(int idx, uint16_t color);

    void Update();
    void Draw();
    void ReDraw();
    void ChangeFace()
    {
        _EyeMode = FaceModeTable[random(0, 11)];
        _MouthMode = random(MouthMode::M1, MouthMode::M6 + 1);
    }
    void DrawEye();
    void DrawEyeBlink();
    int EyeMode() { return _EyeMode; }
};

#endif
