#ifndef ALARM_H
#define ALARM_H

#include "Main.h"
#include "Button.h"
#include "Config.h"

#define ALMTIME_X       SCALEX(170)     // 設定時刻のx座標
#define ALMTIME_Y       SCALEY(150)     // 設定時刻のy座標
#define FSIZE_T         SCALEY(120)     // 設定時刻のフォントサイズ

#define ALARMVALID_X    (MAINAREA_X + SCALEX(15))
#define ALARMVALID_Y    (MAINAREA_Y + SCALEY(297))
#define ALARMVALID_W    SCALEX(280)
#define ALARMVALID_H    SCALEY(65)
#define MODE_X          (MAINAREA_X + MAINAREA_W/2 + SCALEX(15))
#define MODE_Y          (MAINAREA_Y + SCALEY(297))
#define MODE_W          SCALEX(280)
#define MODE_H          SCALEY(65)

// アラーム設定保存構造体
struct AlarmCfg {
    int alarm_h;                    // アラームを鳴らす時間
    int alarm_m;                    // アラームを鳴らす分
    bool is_alarm_valid;            // アラームがオン状態か
    bool is_wakeup_mode;            // true:目覚ましモード，false:通常モード
    void Check()                    // メンバが不正な値の時修正する
    {
        if (alarm_h < 0 || alarm_h > 23)    alarm_h = 0;
        if (alarm_m < 0 || alarm_m > 59)    alarm_m = 0;
    }
};

// 時刻表示部分の制御
class AlarmTime : public Area {
    AlarmCfg &temp;

    enum eTimeCfgStatus { Invariable, HVariable, MVariable };
    eTimeCfgStatus status;
    int changing;           // 設定時刻変更から30フレームを数える
    int hFont;              // 設定時刻を表示するフォント
public:
    AlarmTime(AlarmCfg &ref);
    ~AlarmTime();
    void Update();
    void Draw();
};

// アラーム画面で使うフォント作成クラス
class FontMakerAlarm {
public:
    FontMakerAlarm();
};

// アラーム画面クラス
class AlarmScreen : public BaseScene, public FontMakerAlarm {
    AlarmCfg temp;                  // アラーム設定一時保存オブジェクト

    AlarmTime alarm_time;           // 設定時刻オブジェクト
    Item<bool> alarm_valid;         // アラームON/OFF切り替えUI
    Item<bool> mode;                // アラーム設定切り替えUI

    Button complete;                // 設定完了ボタン
    Button cancel;                  // キャンセルボタン
public:
    AlarmScreen(ISceneChanger *changer);
    ~AlarmScreen();
    void Initialize();
    void Finalize() {}
    void Update();
    void Draw();
    void Reflect();                 // tempが持つ設定を反映させる
};

// アラームのバックグラウンド処理を行う
class Alarm : public IOBase<AlarmCfg> {
    bool is_ringing;            // アラームが現在鳴っているか
public:
    Alarm();
    void Update();                                              // アラームを鳴らす処理など
    enum eCfg { AlarmH, AlarmM, IsAlarmValid, IsWakeupMode };
    void SetCfg(eCfg cfg, int value);                           // 設定を変更する
};

extern Alarm *pAlarm;

#endif // !ALARM_H
