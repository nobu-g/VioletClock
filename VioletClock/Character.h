#ifndef CHARACTER_H
#define CHARACTER_H

#include "Base.h"
#include <unordered_map>
using std::unordered_map;

// キャラクタ関係の定数
#define CHR_X               SCALEX(10)      // キャラクタ左上のx座標
#define CHR_Y               SCALEY(148)     // キャラクタ左上のy座標
#define CHR_W               SCALEX(120)     // キャラクタの幅
#define CHR_H               SCALEY(290)     // キャラクタの高さ

// バッテリー関係の定数
#define BAT_LOW             20              // バッテリーが減ったとみなす残量(%)
#define BAT_CRISIS          10              // バッテリー限界とみなす残量(%)

// キーボード関係の定数
#define KEY_INPUT_WIN       (0xDA)          // 左右Windowsキー
#define KEY_INPUT_CTRL      (0x5D)          // 左右Controlキー
#define KEY_INPUT_ALT       (0x78)          // 左右Altキー
#define KEY_INPUT_SHIFT     (0x29)          // 左右Shiftキー

// キャラの表情
enum eFace { Confident, Doya, Excited, Kyoton, Laugh, Normal, Shocked, Speaking, Tearful, Wink, Blink };

// キャラクタクラス
class Chr : public Area {
    static int hBase;                           // ベース画像へのハンドル
    static unordered_map<eFace, int> hFace;     // 表情の差分画像へのハンドル
    static int hBackground;                     // 背景画像へのハンドル
public:
    Chr();
    void Draw();
    void Update();
    static void DrawBackImg();
};

extern Chr *pChr;

#endif // !CHARACTER_H
