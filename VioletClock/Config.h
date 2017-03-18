#ifndef CONFIG_H
#define CONFIG_H

#include "Main.h"
#include "Voice.h"
#include "Clock.h"
#include "Button.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

// ボタンの座標関係の定数
#define COMP_X              370         // 完了ボタンのx座標
#define COMP_Y              435         // 完了ボタンのy座標
#define COMP_W              120         // 完了ボタンの幅
#define CANCEL_X            500         // キャンセルボタンのx座標
#define CANCEL_Y            435         // キャンセルボタンのy座標
#define CANCEL_W            120         // キャンセルボタンの幅
#define RESET_X             25          // 既定値に戻すボタンのx座標
#define RESET_Y             435         // 既定値に戻すボタンのy座標
#define RESET_W             150         // 既定値に戻すボタンの幅

// 選択項目の座標関係の定数
#define HFORM_X             (MAINAREA_X + 15)
#define HFORM_Y             (MAINAREA_Y + 15)
#define AUTOREPRO_X         (MAINAREA_X + 15)
#define AUTOREPRO_Y         (MAINAREA_Y + 109)
#define SOUNDYEAR_X         (MAINAREA_X + 15)
#define SOUNDYEAR_Y         (MAINAREA_Y + 203)
#define SOUNDWEEK_X         (MAINAREA_X + 15)
#define SOUNDWEEK_Y         (MAINAREA_Y + 297)
#define TAILDATE_X          (MAINAREA_X + MAINAREA_W/2 + 15)
#define TAILDATE_Y          (MAINAREA_Y + 15)
#define TAILTIME_X          (MAINAREA_X + MAINAREA_W/2 + 15)
#define TAILTIME_Y          (MAINAREA_Y + 165)
#define FORPRG_X            (MAINAREA_X + MAINAREA_W/2 + 15)
#define FORPRG_Y            (MAINAREA_Y + 315)

// 設定画面等の作業領域(色が薄いエリア)関係の定数
#define MAINAREA_X          5           // 作業領域のx座標
#define MAINAREA_Y          37          // 作業領域のy座標
#define MAINAREA_W          630         // 作業領域の幅
#define MAINAREA_H          389         // 作業領域の高さ

// デザイン関係の定数
#define FSIZE_CHOICE        17          // 選択肢ボタンのフォントサイズ
#define FSIZE_MAIN          20          // 設定画面等で使う主要フォントサイズ

// 誕生日設定関係の定数
#define BIRTHDAY_X          95          // 表示された誕生日のx座標
#define BIRTHDAY_Y          181         // 表示された誕生日のy座標
#define FSIZE_BD            100         // 誕生日のフォントサイズ

// 設定データ一時保存クラス
class CfgData {
public:
    VoiceCfg vcfg;
    DateCfg dcfg;
    TimeCfg tcfg;

    CfgData();                  // 現在の設定を読み込む
    void Default();             // (誕生日以外の)メンバをデフォルト値に戻す
    void DefaultBd();           // 誕生日をデフォルト値に戻す
};

template <class DataType> class Choice;
// 設定項目クラス
template <class DataType>
class Item : public Area {
    int x, y;                               // 項目名を表示する座標
    string name;                            // 項目名
    DataType &data;                         // この項目が参照する設定データ
    vector<Choice<DataType> *> pChoice;     // 選択肢
public:
    Item(int x0, int y0, int w, int h, string str, DataType &ref);
    ~Item();
    void PushBack(Choice<DataType> *pCh);
    void Update();
    void Draw();
};

// 設定項目の選択肢クラス
template <class DataType>
class Choice : public Area {
    string name;                    // 選択肢名(ON/OFFなど)
    int str_width;                  // 表示される選択肢名の幅
    DataType value;                 // 選択肢固有の値
public:
    Choice(int x, int y, int w, string str, DataType val);
    void Set(DataType &data);       // dataを値valueに設定する                  
    void Draw(DataType &data);      // 選択肢の文字やマーカーを表示」
};

// 時刻調整クラス
class ConfigTime : public TimeSrc {
    enum eTimeCfgStatus { Invariable, HVariable, MinVariable, YVariable, MonVariable, DVariable };
    eTimeCfgStatus status;
    int changing;       // 数値変更から30フレームを数える
public:
    ConfigTime();
    void Update();
    void Draw();
    void ResetStatus() { status = Invariable; }
};

// 誕生日設定クラス
class SetBirthday : public Area {
    DateCfg &dcfg;

    enum eDateCfgStatus { Invariable, MVariable, DVariable };
    eDateCfgStatus status;
    int changing;       // 数値変更から30フレーム数える
public:
    SetBirthday(DateCfg &ref);
    void Update();
    void Draw();
    void ResetStatus() { status = Invariable; }
};

// フォントデータを作るためのクラス
class FontMaker {
public:
    FontMaker();
};

// 設定画面クラス
class Config : public BaseScene, public FontMaker {
    enum eTab { Tab_MainCfg, Tab_TimeAdj, Tab_SetBirthday };
    eTab tab;                   // 現在表示しているタブ
    CfgData temp;               // 変更データを一時保存

    ConfigTime cfg_time;        // 時刻設定用オブジェクト
    SetBirthday set_bd;         // 誕生日設定用オブジェクト

    Item<eHourForm> h_form;
    Item<bool> auto_repro;
    Item<bool> sound_year;
    Item<bool> sound_week;
    Item<eTail> tail_date;
    Item<eTail> tail_time;
    Item<bool> for_prg;

    Button reset;               // 既定値に戻すボタン
    Button complete;            // 設定完了ボタン
    Button cancel;              // キャンセルボタン
public:
    Config(ISceneChanger *changer);
    ~Config();
    void Initialize();
    void Finalize() {}
    void Update();
    void Draw();
    void Reflect();             // tempが持つ設定を反映させる
};

#endif // !CONFIG_H
