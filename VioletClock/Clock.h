#ifndef CLOCK_H
#define CLOCK_H

#include "Main.h"
#include "Base.h"
#include "Button.h"
#include "Voice.h"
#include <ctime>

// Time, Date関係の定数
#define DATE_X      270     // 日付左上のx座標
#define DATE_Y      330     // 日付左上のy座標
#define TIME_X      150     // 時刻左上のx座標
#define TIME_Y      150     // 時刻左上のy座標
#define FSIZE_D     30      // 日付表示用フォントのサイズ
#define FSIZE_HM    150     // 時、分用フォントのサイズ
#define FSIZE_S     50      // 秒用フォントのサイズ
#define FSIZE_AP    35      // AM/PM用フォントのサイズ
#define SPACE_MS    10      // 分と秒の間の間隔(ピクセル)

enum eHourForm { Form12h, Form24h };
enum eTail { Tail_Dane, Tail_Dayo, Tail_Ninattayo, Tail_Datte, Tail_None };

// メイン画面クラス
class Clock : public BaseScene {
    Chr character;
    RptButton rpt_btn;
    CfgButton cfg_btn;
    MuteButton mute_btn;
    AlmButton alm_btn;
    TmrButton tmr_btn;
public:
    Clock(ISceneChanger *changer) : BaseScene(changer) {}
    void Initialize();
    void Finalize();
    void Update();
    void Draw();
};

// 時差設定保存構造体
struct TimeSrcCfg {
    time_t offset;          // 標準時とのずれ(秒)
    void Check()            // メンバが不正な値の時修正する
    {
        // 1970年から3000年までの秒数
        if (offset < -32535244799 || offset > 32535244799)
            offset = 0;
    }
};

// 標準時を提供する時間クラス
class TimeSrc : public IOBase<TimeSrcCfg> {
protected:
    tm local;           // 時間を格納する構造体
public:
    TimeSrc();
    void Update();                  // 現在時刻を更新
    void SetCfg(time_t t);          // 設定を変更する
    friend class Date;              // localにアクセスしたい
    friend class Time;              // localにアクセスしたい
};
extern TimeSrc *pTimeSrc;

// 日付設定保存構造体
struct DateCfg {
    bool sound_year;        // 年を読み上げるかどうか
    bool sound_week;        // 曜日を読み上げるかどうか
    eTail tail;             // 日付読み上げ時の語尾
    int birthmonth;         // 誕生月
    int birthday;           // 誕生日
    void Check()            // メンバが不正な値の時修正する
    {
        switch (tail) {
        case Tail_Dane:  case Tail_Dayo:  case Tail_Ninattayo:  case Tail_Datte:  case Tail_None:  break;
        default:        tail = Tail_None; break;
        }
        if (birthmonth < 1 || birthmonth > 12)  birthmonth = 12;
        if (birthday   < 1 || birthday   > 31)  birthday = 19;
    }
};

// 日付クラス
class Date : public Area, public IOBase<DateCfg> {
    int hFont_d;                // 日付表示用フォント
    eTmData speaking;           // 現在再生されている音声
public:
    Date();
    int Gety() { return pTimeSrc->local.tm_year + 1900; }   // 年を返す
    int Getm() { return pTimeSrc->local.tm_mon + 1; }       // 月を返す
    int Getd() { return pTimeSrc->local.tm_mday; }          // 日を返す
    int Getw() { return pTimeSrc->local.tm_wday; }          // 曜日を返す
    int Gett() { return config.tail; }                      // 語尾を返す
    void Draw();
    void Update();                          // クリックされたときの動作
    void ResetSpeaking();                   // 日付の読み上げをリセット
    bool IsNewYear() {                      // 今日が元日(1/1)かどうか
        return Getm() == 1 && Getd() == 1;
    }
    bool IsHalloween() {                    // 今日がハロウィン(10/31)かどうか
        return Getm() == 10 && Getd() == 31;
    }
    bool IsChristmas() {                    // 今日がクリスマス(12/25)かどうか
        return Getm() == 12 && Getd() == 25;
    }
    bool IsBirthday() {                     // 今日が誕生日か
        return Getm() == config.birthmonth && Getd() == config.birthday;
    }
    bool IsSummer() {                       // 今が夏(6月～8月)かどうか
        return Getm() >= 6 && Getm() <= 8;
    }
    bool IsWinter() {                       // 今が冬(12月～3月)かどうか
        return Getm() >= 12 || Getm() <= 3;
    }
    enum eCfg { SoundYear, SoundWeek, TailDate, BirthMonth, BirthDay };
    void SetCfg(eCfg cfg, int value);                               // 設定を変更する
};

// 時刻設定保存構造体
struct TimeCfg {
    eHourForm h_form;       // 時刻を0-12形式/0-24形式のどちらで表示するか
    eTail tail;             // 時刻読み上げ時の語尾
    void Check()            // メンバが不正な値の時修正する
    {
        if (h_form != Form12h && h_form != Form24h) h_form = Form24h;
        switch (tail) {
        case Tail_Dane:  case Tail_Dayo:  case Tail_Ninattayo:  case Tail_Datte:  case Tail_None:  break;
        default:        tail = Tail_None; break;
        }
    }
};

// 時刻クラス
class Time : public Area, public IOBase<TimeCfg> {
    int hFont_hm;               // 時、分用フォント
    int hFont_s;                // 秒用フォント
    int hFont_ap;               // AM/PM用フォント

    eTmData speaking;           // 現在再生されている音声
    bool time_signal_flag;      // 時報を鳴らさなければならないか
public:
    Time();
    void Init() { time_signal_flag = false; }                       // 初期化処理
    int Getn() { return (pTimeSrc->local.tm_hour < 12) ? 0 : 1; }   // 午前:0、午後:1を返す
    int Geth() { return pTimeSrc->local.tm_hour; }                  // 時を返す
    int Getm() { return pTimeSrc->local.tm_min; }                   // 分を返す
    int Gets() { return pTimeSrc->local.tm_sec; }                   // 秒を返す
    int Gett() { return config.tail; }                              // 語尾を返す
    void Draw();
    void Update();                                                  // クリックされたときの動作
    void ResetSpeaking();                                           // 時刻の読み上げをリセット
    enum eCfg { HForm, TailTime };
    void SetCfg(eCfg cfg, int value);                               // 設定を変更する
};

extern Date *pDate;
extern Time *pTime;

#endif // !CLOCK_H
