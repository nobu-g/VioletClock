#ifndef BUTTON_H
#define BUTTON_H

#include "Base.h"
#include <string>
using std::string;

#define PI 3.141592653589793        // 円周率

// 設定ボタン関係の定数
#define CFG_X           570         // 設定ボタン左上のx座標
#define CFG_Y           400         // 設定ボタン左上のy座標
#define CFG_W           40          // 設定ボタンの幅
#define CFG_H           40          // 設定ボタンの高さ

// もう一度再生ボタン関係の定数
#define RPT_X           500         // もう一度再生ボタン左上のx座標
#define RPT_Y           400         // もう一度再生ボタン左上のy座標
#define RPT_W           40          // もう一度再生ボタンの幅
#define RPT_H           40          // もう一度再生ボタンの高さ

// ミュートボタン関係の定数
#define MUTE_X          430         // ミュートボタン左上のx座標
#define MUTE_Y          400         // ミュートボタン左上のy座標
#define MUTE_W          40          // ミュートボタンの幅
#define MUTE_H          40          // ミュートボタンの高さ

// アラームボタン関係の定数
#define ALM_X           360         // アラームボタン左上のx座標
#define ALM_Y           400         // アラームボタン左上のy座標
#define ALM_W           40          // アラームボタンの幅
#define ALM_H           40          // アラームボタンの高さ

// タイマー関係の定数
#define TMR_X           290         // タイマーボタン左上のx座標
#define TMR_Y           400         // タイマーボタン左上のy座標
#define TMR_W           40          // タイマーボタンの幅
#define TMR_H           40          // タイマーボタンの高さ
#define TMRSTART_X      400         // スタートボタン左上のx座標
#define TMRSTART_Y      300         // スタートボタン左上のy座標
#define TMRSTART_W      100         // スタートボタンの幅
#define TMRRESET_X      140         // リセットボタン左上のx座標
#define TMRRESET_Y      300         // リセットボタン左上のy座標
#define TMRRESET_W      100         // リセットボタンの幅
#define TMRCHANGE_X     560         // ボイス変更ボタンのx座標
#define TMRCHANGE_Y     388         // ボイス変更ボタンのy座標
#define TMRCHANGE_W     48          // ボイス変更ボタンの幅
#define TMRCHANGE_H     20          // ボイス変更ボタンの高さ

#define MARGIN      8           // ボタンの文字の上下のスペース

// もう一度再生ボタンクラス
class RptButton : public Area {
    static int hImage;
public:
    RptButton();
    void Draw();
    void Update();
};

// 設定ボタンクラス
class CfgButton : public Area {
    static int hImage;
public:
    CfgButton();
    void Draw();
};

// ミュートボタンクラス
class MuteButton : public Area {
    static int hSnd_img;
    static int hMute_img;
public:
    MuteButton();
    void Draw();
    void Update();
};

// アラームボタンクラス
class AlmButton : public Area {
    static int hBase;
    static int hHand;
public:
    AlmButton();
    void Draw();
};

// タイマーボタンクラス
class TmrButton : public Area {
    static int hImage;
public:
    TmrButton();
    void Draw();
};

// タイマー画面のスタート/一時停止ボタンクラス
class TmrStartButton : public Area {
    int hFont;                  // 表示する文字列のフォント
public:
    TmrStartButton(int hF);
    void Draw();
};

// タイマー画面のリセットボタンクラス
class TmrResetButton : public Area {
    int hFont;                  // 表示する文字列のフォント
public:
    TmrResetButton(int hF);
    void Draw();
};

// タイマー画面の終了時音声変更ボタン
class TmrVoiceChgButton : public Area {
    int hFont;                  // 表示する文字列のフォント
public:
    TmrVoiceChgButton();
    ~TmrVoiceChgButton();
    void Draw();
};

// 設定画面その他のボタンクラス
class Button : public Area {
    string name;                // ボタンに表示する文字列
    int str_width;              // 表示される文字列の幅
    int hFont;                  // 表示する文字列のフォント
public:
    Button(int x, int y, int w, string str, int hF);
    Button() : Area(0, 0, 0, 0) {}
    void Draw();
};

#endif // !BUTTON_H
