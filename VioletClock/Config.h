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

// デザイン関係の定数
#define FSIZE_CHOICE        SCALEY(17)          // 選択肢ボタンのフォントサイズ
#define FSIZE_MAIN          SCALEY(20)          // 設定画面等で使う主要フォントサイズ

// ボタンの座標関係の定数
#define COMP_X              SCALEX(370)         // 完了ボタンのx座標
#define COMP_Y              SCALEY(435)         // 完了ボタンのy座標
#define COMP_W              SCALEX(120)         // 完了ボタンの幅
#define CANCEL_X            SCALEX(500)         // キャンセルボタンのx座標
#define CANCEL_Y            SCALEY(435)         // キャンセルボタンのy座標
#define CANCEL_W            SCALEX(120)         // キャンセルボタンの幅
#define RESET_X             SCALEX(25)          // 既定値に戻すボタンのx座標
#define RESET_Y             SCALEY(435)         // 既定値に戻すボタンのy座標
#define RESET_W             SCALEX(150)         // 既定値に戻すボタンの幅

// 設定画面等の作業領域(色が薄いエリア)関係の定数
#define MAINAREA_X          SCALEX(5)           // 作業領域のx座標
#define MAINAREA_Y          SCALEY(37)          // 作業領域のy座標
#define MAINAREA_W          SCALEX(630)         // 作業領域の幅
#define MAINAREA_H          SCALEY(389)         // 作業領域の高さ

// 選択項目の座標関係の定数
#define HFORM_X             (MAINAREA_X + SCALEX(15))
#define HFORM_Y             (MAINAREA_Y + SCALEY(15))
#define HFORM_W             SCALEX(280)
#define HFORM_H             SCALEY(65)
#define AUTOREPRO_X         (MAINAREA_X + SCALEX(15))
#define AUTOREPRO_Y         (MAINAREA_Y + SCALEY(109))
#define AUTOREPRO_W         SCALEX(280)
#define AUTOREPRO_H         SCALEY(65)
#define SOUNDYEAR_X         (MAINAREA_X + SCALEX(15))
#define SOUNDYEAR_Y         (MAINAREA_Y + SCALEY(203))
#define SOUNDYEAR_W         SCALEX(280)
#define SOUNDYEAR_H         SCALEY(65)
#define SOUNDWEEK_X         (MAINAREA_X + SCALEX(15))
#define SOUNDWEEK_Y         (MAINAREA_Y + SCALEY(297))
#define SOUNDWEEK_W         SCALEX(280)
#define SOUNDWEEK_H         SCALEY(65)
#define TAILDATE_X          (MAINAREA_X + MAINAREA_W/2 + SCALEX(15))
#define TAILDATE_Y          (MAINAREA_Y + SCALEY(15))
#define TAILDATE_W          SCALEX(280)
#define TAILDATE_H          SCALEY(130)
#define TAILTIME_X          (MAINAREA_X + MAINAREA_W/2 + SCALEX(15))
#define TAILTIME_Y          (MAINAREA_Y + SCALEY(165))
#define TAILTIME_W          SCALEX(280)
#define TAILTIME_H          SCALEY(130)
#define FORPRG_X            (MAINAREA_X + MAINAREA_W/2 + SCALEX(15))
#define FORPRG_Y            (MAINAREA_Y + SCALEY(315))
#define FORPRG_W            SCALEX(280)
#define FORPRG_H            SCALEY(57)

// タブ関係の定数
#define TAB_X               MAINAREA_X
#define TAB_Y               SCALEY(7)
#define TAB_H               (MAINAREA_Y - TAB_Y)
#define TAB_SPACE           SCALEX(16)
#define TAB_MAINCFG_X       TAB_X
#define TAB_MAINCFG_W       (TAB_SPACE * 2 + FSIZE_MAIN * 2)
#define TAB_TIMEADG_X       (TAB_MAINCFG_X + TAB_MAINCFG_W)
#define TAB_TIMEADG_W       (TAB_SPACE * 2 + FSIZE_MAIN * 4)
#define TAB_SETBIRTHDAY_X   (TAB_TIMEADG_X + TAB_TIMEADG_W)
#define TAB_SETBIRTHDAY_W   (TAB_SPACE * 2 + FSIZE_MAIN * 5)
#define TAB_STRING_Y        SCALEY(14)

// 誕生日設定関係の定数
#define BIRTHDAY_X          SCALEX(95)          // 表示された誕生日のx座標
#define BIRTHDAY_Y          SCALEY(181)         // 表示された誕生日のy座標
#define FSIZE_BD            SCALEY(100)         // 誕生日のフォントサイズ

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
