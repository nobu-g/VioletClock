#ifndef VOICE_H
#define VOICE_H

#include "Base.h"
#include "Character.h"
#include <array>
#include <unordered_map>
using std::ofstream;
using std::ifstream;
using std::array;
using std::unordered_map;

enum eTmData { None, Year, Month, Day, Week, Noon, Hour, TimeSignal, Minute, Tail };

// Voice関係の定数
#define YEAR        7       // 年の音声の数(2014～2020)
#define MONTH       12      // 月の音声の数(1～12月)
#define DAY         31      // 日の音声の数(1～31日)
#define WEEK        7       // 曜日の音声の数(日～土曜日)
#define NOON        2       // 午前・午後の音声の数
#define HOUR        24      // 時の音声の数(0～23時)
#define MINUTE      60      // 分の音声の数(0～59分)
#define TAIL        4       // 語尾の種類(…だね！…だよ!…になったよ!)
#define SEASON      10      // キャラが喋る日付依存の台詞の種類
#define SYSTEM      4       // システム音声の種類
#define ALARM       7       // アラーム関係の音声の種類
#define TIMER       22      // タイマー関係の音声の種類
#define WORDS       132     // キャラの台詞の種類

// キャラの台詞クラス
class Words {
    int hWord;              // 台詞音声へのハンドル

    // min_hour時min_minute分から、max_hour時max_minute分の間、台詞を喋ることができる
    int min_hour;
    int min_minute;
    int max_hour;
    int max_minute;

    bool programmer;        // プログラマ用の台詞かどうか
    eFace face;             // この台詞に合った表情
public:
    Words() {}
    Words(eFace f, int hw);
    Words(int minh, int maxh, bool prg, eFace f, int hw);
    Words(double minh, double maxh, bool prg, eFace f, int hw);
    int GetHandle() { return hWord; }
    eFace GetFace() { return face; }
    // 現時刻がmin_hour時min_minute分から、max_hour時max_minute分の間にあるか
    bool IsAppropriate();
};

// サウンド設定保存構造体
struct VoiceCfg {
    bool sound;                 // サウンドON/OFF
    bool auto_repro;            // 自動再生ON/OFF
    bool for_prg;               // プログラマ用ボイスを流すかどうか
    bool first_time;            // 初回起動かどうか
    void Check() {}             // メンバが不正な値の時修正する
};

// 音声管理クラス
class Voice : public IOBase<VoiceCfg> {
    // それぞれ音声のハンドルを格納
    array<int, YEAR>    hYear;
    array<int, MONTH>   hMonth;
    array<int, DAY>     hDay;
    array<int, WEEK>    hWeek;
    array<int, NOON>    hNoon;
    array<int, HOUR>    hHour;
    array<int, HOUR>    hTime_signal;
    array<int, MINUTE>  hMinute;
    array<int, TAIL>    hTail;

    array<int, SEASON>  hSeason;
    array<int, SYSTEM>  hSystem;
    array<int, ALARM>   hAlarm;
    array<int, TIMER>   hTimer;

    typedef unsigned char KeyCode;
    unordered_map<KeyCode, int> keyboard;

    array<Words, WORDS> words;

    int hPlaying;               // 最後に再生した音声のハンドル
    eFace face;                 // 最後に再生した音声に合った表情
public:
    Voice();                                    // 音声ファイルのロードなど

    void SoundTime(eTmData data, int index);    // 時間関連の音を再生する
    void SoundWords();                          // キャラのセリフを再生する
    void SoundSys(int index);                   // システム音声を再生する
    void SoundKey(KeyCode key);                 // キーを読み上げる
    void SoundAlarm();                          // 通常のアラーム音を再生する
    void SoundWakeUp();                         // 目覚ましのアラーム音を再生する

    enum eTimerVoice {
        Start1, Start2,
        Last1m, Last30, Last1, Last2, Last3, Last4, Last5, Last6, Last7, Last8, Last9, Last10,
        Over1, Over2, Over3, Over4, Over5, Over6, Over7, Over8
    };
    void SoundTimer(eTimerVoice voice);         // タイマー関係の音を再生する

    bool IsPlaying();                           // 現在音声が再生中か
    bool IsTimerPlaying();                      // 現在タイマー関係の音声が再生中か
    void Repeat();                              // 直前に再生した音声をもう一度再生
    void StopSound();                           // 現在鳴っているサウンドを止める
    enum eCfg { Sound, AutoRepro, ForPrg, FirstTime };
    void SetCfg(eCfg cfg, int value);           // 設定を変更する
    eFace GetFace() { return face; }            // 鳴っているサウンドにふさわしいキャラの表情を返す
};

extern Voice *pVoice;

#endif // !VOICE_H
