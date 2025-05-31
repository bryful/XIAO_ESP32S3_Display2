#include "FACE.hpp"

Face::Face()
{
    _isSetup = false;
    display = nullptr;
    Comp = nullptr;
    _SkinColor = 0xFF53;                           // 肌色
    _SkinColor = lgfx::color565(0xE1, 0xA9, 0x90); // ピンク色に変更
}
Face::~Face()
{
    if (_isSetup == true)
    {
        delete Comp;
        delete eye00;
        delete eyec0;
        delete eyec1;
        delete eyer0;
        delete eyer1;
        delete eyel0;
        delete eyel1;
        delete eyeex1;
        delete eyeex2;
        delete eyeex3;
        delete mayu;
        delete mouth1;
        delete mouth2;
        delete mouth3;
        delete mouth4;
        delete mouth5;
        delete mouth6;

        _isSetup = false;
    }
}
void Face::setupSprite(lgfx::LGFX_Sprite *d, const char *f, int w, int h, bool isCenter)
{
    d->setColorDepth(4);
    d->createSprite(w, h);
    d->createPalette(); // パレットを作成
    if (strlen(f) != 0)
    {
        d->drawBmpFile(LittleFS, f, 0, 0);
    }
    if (isCenter)
    {
        d->setPivot(d->width() / 2, d->height() / 2);
    }
}

void Face::Begin(lgfx::LGFX_Device *d)
{
    if (_isSetup == true)
    {
        return;
    }
    if (d == nullptr)
    {
        return;
    }
    LittleFS.begin();

    display = d;
    Comp = new lgfx::LGFX_Sprite(display);

    eye00 = new lgfx::LGFX_Sprite(Comp);
    eyec0 = new lgfx::LGFX_Sprite(Comp);
    eyec1 = new lgfx::LGFX_Sprite(Comp);
    eyel0 = new lgfx::LGFX_Sprite(Comp);
    eyel1 = new lgfx::LGFX_Sprite(Comp);
    eyer0 = new lgfx::LGFX_Sprite(Comp);
    eyer1 = new lgfx::LGFX_Sprite(Comp);

    eyeex1 = new lgfx::LGFX_Sprite(Comp);
    eyeex2 = new lgfx::LGFX_Sprite(Comp);
    eyeex3 = new lgfx::LGFX_Sprite(Comp);

    mayu = new lgfx::LGFX_Sprite(Comp);

    mouth1 = new lgfx::LGFX_Sprite(Comp);
    mouth2 = new lgfx::LGFX_Sprite(Comp);
    mouth3 = new lgfx::LGFX_Sprite(Comp);
    mouth4 = new lgfx::LGFX_Sprite(Comp);
    mouth5 = new lgfx::LGFX_Sprite(Comp);
    mouth6 = new lgfx::LGFX_Sprite(Comp);

    setupSprite(Comp, "/girl.bmp", DisplayWidth, DisplayHeight, false);
    setupSprite(eye00, "/eye00.bmp", EyeBufWidth, EyeBufHeight);
    setupSprite(eyec0, "/eyec0.bmp", EyeBufWidth, EyeBufHeight);
    setupSprite(eyec1, "/eyec1.bmp", EyeBufWidth, EyeBufHeight);
    setupSprite(eyel0, "/eyel0.bmp", EyeBufWidth, EyeBufHeight);
    setupSprite(eyel1, "/eyel1.bmp", EyeBufWidth, EyeBufHeight);
    setupSprite(eyer0, "/eyer0.bmp", EyeBufWidth, EyeBufHeight);
    setupSprite(eyer1, "/eyer1.bmp", EyeBufWidth, EyeBufHeight);

    setupSprite(eyeex1, "/eyeex1.bmp", EyeEXBufWidth, EyeEXBufHeight);
    setupSprite(eyeex2, "/eyeex2.bmp", EyeEXBufWidth, EyeEXBufHeight);
    setupSprite(eyeex3, "/eyeex3.bmp", EyeEXBufWidth, EyeEXBufHeight);

    setupSprite(mayu, "/mayu.bmp", MayuBufWidth, MayuBufHeight);

    setupSprite(mouth1, "/mouth1.bmp", MouthBufWidth, MouthBufHeight);
    setupSprite(mouth2, "/mouth2.bmp", MouthBufWidth, MouthBufHeight);
    setupSprite(mouth3, "/mouth3.bmp", MouthBufWidth, MouthBufHeight);
    setupSprite(mouth4, "/mouth4.bmp", MouthBufWidth, MouthBufHeight);
    setupSprite(mouth5, "/mouth5.bmp", MouthBufWidth, MouthBufHeight);
    setupSprite(mouth6, "/mouth6.bmp", MouthBufWidth, MouthBufHeight);

    for (int i = 0; i < 16; i++)
    {
        SetPalette(i, palette[i]);
    }
    _isSetup = true;
}
void Face::SetPalette(int idx, uint16_t color)
{
    if (_isSetup == true)
    {
        return;
    }
    Comp->setPaletteColor(idx, color);
    eye00->setPaletteColor(idx, color);
    eyec0->setPaletteColor(idx, color);
    eyec1->setPaletteColor(idx, color);
    eyeex1->setPaletteColor(idx, color);
    eyeex2->setPaletteColor(idx, color);
    eyeex3->setPaletteColor(idx, color);
    eyel0->setPaletteColor(idx, color);
    eyel1->setPaletteColor(idx, color);
    eyer0->setPaletteColor(idx, color);
    eyer1->setPaletteColor(idx, color);
    mayu->setPaletteColor(idx, color);
    mouth1->setPaletteColor(idx, color);
    mouth2->setPaletteColor(idx, color);
    mouth3->setPaletteColor(idx, color);
    mouth4->setPaletteColor(idx, color);
    mouth5->setPaletteColor(idx, color);
    mouth6->setPaletteColor(idx, color);
}
// ********************************************************************************
bool Read4BitBmpPalette(fs::FS &fs, const char *filePath, lgfx::bgr888_t palette[16])
{
    File file = fs.open(filePath, "r");
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return false;
    }

    // ヘッダをスキップしてパレット位置へ
    file.seek(28); // 14(BITMAPFILEHEADER) + 14(biSizeまで) = 28
    uint16_t biBitCount = file.read() | (file.read() << 8);
    if (biBitCount != 4)
    {
        Serial.println("Not a 4-bit BMP file");
        file.close();
        return false;
    }

    file.seek(54); // 14(BITMAPFILEHEADER) + 40(BITMAPINFOHEADER) = 54

    for (int i = 0; i < 16; ++i)
    {
        uint8_t b = file.read();
        uint8_t g = file.read();
        uint8_t r = file.read();
        file.read(); // reserved
        palette[i] = lgfx::bgr888_t(r, g, b);
    }
    file.close();
    return true;
}
// ********************************************************************************
bool load4bitBmpfile(char *f, lgfx::LGFX_Sprite *g)
{
    if (!LittleFS.exists(f))
    {
        return false;
    }
    lgfx::bgr888_t palette[16];
    for (size_t i = 0; i < 16; i++)
    {
        palette[i] = lgfx::bgr888_t(i * 16, i * 16, i * 16); // 初期化
    }

    g->drawBmpFile(LittleFS, f, 0, 0);
    if (Read4BitBmpPalette(LittleFS, f, palette))
    {
        for (int i = 0; i < 16; ++i)
        {
            g->setPaletteColor(i, palette[i]);
        }
        return true;
    }
    else
    {
        return false;
    }
}

void Face::Update()
{
    if (_isSetup == false)
    {
        return;
    }
    Comp->pushSprite(0, 0);
}

void Face::Draw()
{
    if (_isSetup == false)
    {
        return;
    }
    DrawHoho();
    DrawMouth();
    DrawEye();
}
void Face::DrawHoho()
{
    if (_isSetup == false)
    {
        return;
    }
    Comp->fillScreen(FacePallet::Skin);
    Comp->fillEllipse(WidthCenter - _HohoRX, _HohoTop, HohoRX, HohoRY, FacePallet::Hoho);
    Comp->fillEllipse(WidthCenter + _HohoRX, _HohoTop, HohoRX, HohoRY, FacePallet::Hoho);
}
void Face::DrawMouth()
{
    switch (_MouthMode)
    {
    case MouthMode::M1:
        mouth1->pushSprite(_MouseLeft, _MouseTop, SKIN_NUMBER);
        break;
    case MouthMode::M2:
        mouth2->pushSprite(_MouseLeft, _MouseTop, SKIN_NUMBER);
        break;
    case MouthMode::M3:
        mouth3->pushSprite(_MouseLeft, _MouseTop, SKIN_NUMBER);
        break;
    case MouthMode::M4:
        mouth4->pushSprite(_MouseLeft, _MouseTop, SKIN_NUMBER);
        break;
    case MouthMode::M5:
        mouth5->pushSprite(_MouseLeft, _MouseTop, SKIN_NUMBER);
        break;
    case MouthMode::M6:
        mouth6->pushSprite(_MouseLeft, _MouseTop, SKIN_NUMBER);
        break;
    }
}
void Face::DrawEyeClose()
{
    mayu->pushRotateZoom(WidthCenter + _MayuRX, _MayuTop + 20, 0, 1, 1, FacePallet::Skin);
    mayu->pushRotateZoom(WidthCenter - _MayuRX, _MayuTop * 20, 0, -1, 1, FacePallet::Skin);
    eye00->pushRotateZoom(WidthCenter + _EyeRX, _EyeTop, 0, 1, 1, FacePallet::Skin);
    eye00->pushRotateZoom(WidthCenter - _EyeRX, _EyeTop, 0, -1, 1, FacePallet::Skin);
}
void Face::DrawEyeInt()
{
    mayu->pushRotateZoom(WidthCenter + _MayuRX, _MayuTop + 8, 0, 1, 1, FacePallet::Skin);
    mayu->pushRotateZoom(WidthCenter - _MayuRX, _MayuTop * 8, 0, -1, 1, FacePallet::Skin);

    switch (_EyeMode)
    {
    case EyeMode::CENTER:
        eyec0->pushRotateZoom(WidthCenter + _EyeRX, _EyeTop, 0, 1, 1, FacePallet::Skin);
        eyec0->pushRotateZoom(WidthCenter - _EyeRX, _EyeTop, 0, -1, 1, FacePallet::Skin);
        break;
    case EyeMode::LEFT:
        eyel0->pushRotateZoom(WidthCenter + _EyeRX, _EyeTop, 0, 1, 1, FacePallet::Skin);
        eyer0->pushRotateZoom(WidthCenter - _EyeRX, _EyeTop, 0, -1, 1, FacePallet::Skin);
        break;
    case EyeMode::RIGHT:
        eyer0->pushRotateZoom(WidthCenter + _EyeRX, _EyeTop, 0, 1, 1, FacePallet::Skin);
        eyel0->pushRotateZoom(WidthCenter - _EyeRX, _EyeTop, 0, -1, 1, FacePallet::Skin);
        break;
    }
}
void Face::DrawEye()
{

    if (_EyeMode < EyeMode::EX1)
    {
        mayu->pushRotateZoom(WidthCenter + _MayuRX, _MayuTop, 0, 1, 1, FacePallet::Skin);
        mayu->pushRotateZoom(WidthCenter - _MayuRX, _MayuTop, 0, -1, 1, FacePallet::Skin);
    }
    switch (_EyeMode)
    {
    case EyeMode::CENTER:
        eyec1->pushRotateZoom(WidthCenter + _EyeRX, _EyeTop, 0, 1, 1, FacePallet::Skin);
        eyec1->pushRotateZoom(WidthCenter - _EyeRX, _EyeTop, 0, -1, 1, FacePallet::Skin);
        break;
    case EyeMode::LEFT:
        eyel1->pushRotateZoom(WidthCenter + _EyeRX, _EyeTop, 0, 1, 1, FacePallet::Skin);
        eyer1->pushRotateZoom(WidthCenter - _EyeRX, _EyeTop, 0, -1, 1, FacePallet::Skin);
        break;
    case EyeMode::RIGHT:
        eyer1->pushRotateZoom(WidthCenter + _EyeRX, _EyeTop, 0, 1, 1, FacePallet::Skin);
        eyel1->pushRotateZoom(WidthCenter - _EyeRX, _EyeTop, 0, -1, 1, FacePallet::Skin);
        break;
    case EyeMode::EX1:
        eyeex1->pushRotateZoom(WidthCenter + _EyeEXRX, _EyeEXTop, 0, 1, 1, FacePallet::Skin);
        eyeex1->pushRotateZoom(WidthCenter - _EyeEXRX, _EyeEXTop, 0, -1, 1, FacePallet::Skin);
        break;
    case EyeMode::EX2:
        eyeex2->pushRotateZoom(WidthCenter + _EyeEXRX, _EyeEXTop, 0, 1, 1, FacePallet::Skin);
        eyeex2->pushRotateZoom(WidthCenter - _EyeEXRX, _EyeEXTop, 0, -1, 1, FacePallet::Skin);
        break;
    case EyeMode::EX3:
        eyeex3->pushRotateZoom(WidthCenter + _EyeEXRX, _EyeEXTop, 0, 1, 1, FacePallet::Skin);
        eyeex3->pushRotateZoom(WidthCenter - _EyeEXRX, _EyeEXTop, 0, -1, 1, FacePallet::Skin);
        break;
    }
}
void Face::DrawEyeBlink()
{
    if (_isSetup == false)
        return;
    ChangeFace();
    // 閉じ目
    DrawHoho();
    DrawMouth();
    DrawEyeClose();
    Update();
    delay(50);

    if (_EyeMode < EyeMode::EX1)
    {
        DrawHoho();
        DrawMouth();
        DrawEyeInt();
        Update();
        delay(50);
    }
    DrawHoho();
    DrawMouth();
    DrawEye();
    Update();
}
