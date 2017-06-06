#ifndef TIMER_H
#define TIMER_H

#include "Base.h"
#include "Config.h"
#include "Button.h"

// フォントサイズ
#define FSIZE_HMS       SCALEY(100)
#define FSIZE_VSELECT   SCALEY(16)

// 時間表示部分関係の定数
#define TMRTIME_X       SCALEX(145)
#define TMRTIME_Y       SCALEY(150)
#define TMRTIME_W       (STOW(FSIZE_HMS) * 7)
#define TMRTIME_H       FSIZE_HMS

// 戻るボタン関係の定数
#define BACK_X          COMP_X
#define BACK_Y          COMP_Y
#define BACK_W          COMP_W

// タイマー終了時の音声の種類
#define OVER_VOICES     8

// タイマー設定保存構造体
struct TimerCfg {
    int hour;
    int minute;
    int second;
    int voice_index;    // タイマー終了時どの音声を再生するか
    void Check()
    {
        if (hour < 0 || hour > 23)  hour = 0;
        if (minute < 0 || minute > 59)  minute = 0;
        if (second < 0 || second > 59)  second = 0;
        if (voice_index < 0 || voice_index >= OVER_VOICES)   voice_index = 0;
    }
};

// 時間表示部分の制御
class TimerTime : public Area {
    enum eTimerCfgStatus { Invariable, HVariable, MVariable, SVariable };
    eTimerCfgStatus status;                         // 時、分、秒、どの部分が変更可能か
    int changing;                                   // 設定時間変更から30フレームを数える
public:
    TimerTime(TimerCfg &ref);
    void ResetStatus() { status = Invariable; }     // ステータスをInvariableに設定する
    void Update();
    void Draw();
};

// 終了時音声選択UI
class VoiceSelect {
    array<string, OVER_VOICES> words;
    TmrVoiceChgButton chg_btn;
public:
    VoiceSelect();
    void Update();
    void Draw();
};

// タイマー画面で使うフォント作成クラス
class FontMakerTimer {
public:
    FontMakerTimer();
};

// タイマー画面クラス
class TimerScreen : public BaseScene, public FontMakerTimer {
    TimerTime timer_time;
    TmrStartButton start_btn;
    TmrResetButton reset_btn;
    VoiceSelect vselect;
    Button back;
public:
    TimerScreen(ISceneChanger *changer);
    ~TimerScreen();
    void Initialize() {}
    void Finalize() {}
    void Update();
    void Draw();
};

// タイマーのカウントを行うクラス
class Timer : public IOBase<TimerCfg> {
public:
    enum eTimerStatus { Idle, Counting, Pause };
private:
    int base_ms;            // 1秒経過したか調べるための基準(ミリ秒)
    int stop_ms;            // タイマーが一時停止した時刻(ミリ秒)
    int remaining_h;        // 残り時間(時)
    int remaining_m;        // 残り時間(分)
    int remaining_s;        // 残り時間(秒)
    eTimerStatus status;
public:
    Timer();
    void Start();
    void Stop();
    void Restart();
    void Reset();
    int Geth() { return remaining_h; }
    int Getm() { return remaining_m; }
    int Gets() { return remaining_s; }
    eTimerStatus GetStatus() { return status; }
    void SoundCount();
    void Update();
    enum eCfg { Hour, Minute, Second, Voice };
    void SetCfg(eCfg cfg, int value);
};

extern Timer *pTimer;

#endif // !TIMER_H
