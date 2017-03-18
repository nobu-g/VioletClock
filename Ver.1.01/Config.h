#ifndef CONFIG_H
#define CONFIG_H

#include "Main.h"
#include "Area.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

// 座標関係の定数
#define CFGAREA_X		5			// 設定エリアのx座標
#define CFGAREA_Y		37			// 設定エリアのy座標
#define CFGAREA_W		630			// 設定エリアの幅
#define CFGAREA_H		389			// 設定エリアの高さ
#define COMP_X			370			// 完了ボタンのx座標
#define COMP_Y			435			// 完了ボタンのy座標
#define COMP_W			120			// 完了ボタンの幅
#define CANCEL_X		500			// キャンセルボタンのx座標
#define CANCEL_Y		435			// キャンセルボタンのy座標
#define CANCEL_W		120			// キャンセルボタンの幅
#define RESET_X			25			// 既定値に戻すボタンのx座標
#define RESET_Y			435			// 既定値に戻すボタンのy座標
#define RESET_W			150			// 既定値に戻すボタンの幅

// デザイン関係の定数
#define FSIZE_MAIN		20			// 設定画面使う主要フォントサイズ
#define FSIZE_CHOICE	17			// 選択肢ボタンのフォントサイズ
#define MARGIN			8			// ボタンの文字の上下のスペース
#define VIOLET1			0xe5D7F7	// 設定エリアの背景色
#define VIOLET2			0xD1B2F7	// 設定項目ボタン背景色
#define VIOLET3			0xC399F7	// 主要ボタンの背景色
#define VIOLET4			0x9442F7	// カーソルの場所を示す枠の色
/*色コードマクロは以下のサイトを参考に、薄い順に並んでいる
http://www.color-sample.com/colors/d9bef8/
*/

// 設定データ保存クラス
class CfgData{
public:
	enum eHourForm { Form12h, Form24h };
	enum eTail { Dane, Dayo, Ninattayo, Datte, None };
	enum eWho { KureiKei };
	enum eData { Sound, AutoRepro, HForm, SoundYear, SoundWeek, ForPrg, TailDate, TailTime, ChrName, FirstTime };
private:
	bool sound;					// サウンドON/OFF
	bool auto_repro;			// 自動再生ON/OFF
	eHourForm h_form;			// 時刻を0-12形式/0-24形式のどちらで表示するか
	bool sound_year;			// 年を読み上げるかどうか
	bool sound_week;			// 曜日を読み上げるかどうか
	bool for_prg;				// プログラマ用かどうか
	eTail tail_date;			// 日付読み上げ時の語尾
	eTail tail_time;			// 時刻読み上げ時の語尾
	eWho chr_name;				// 表示されるキャラクター(現在は暮井慧のみ)
	bool first_time;			// 初回起動かどうか
public:
	int GetCfg(eData data);
	void SetCfg(eData data, int value);
	void Default();				// メンバをデフォルト値に戻す
	void Setup();				// 設定ファイルがない場合、デフォルトに設定
};

// 設定画面のボタンクラス
class Button : public Area {
	string name;				// ボタンに表示する文字列
	int str_width;				// 表示される選択肢名の幅
public:
	Button(int x, int y, int w, string str);
	void Draw();
};

class Choice;
// 設定項目クラス
class Item : public Area {
public:
	enum eItemType { Type_HForm, Type_AutoRepro, Type_SoundYear, Type_SoundWeek, Type_TailDate, Type_TailTime, Type_ForPrg };
private:
	int x, y;					// 項目名を表示する座標
	eItemType type;				// どの設定項目か
	string name;				// 項目名
	vector<Choice *> pChoice;	// 選択肢
	CfgData &temp;				// 仮の設定データへの参照
public:
	Item(int x0, int y0, string str, eItemType t, CfgData &tmp);
	~Item();
	void Update();
	void Draw();
};

// 設定項目の選択肢クラス
class Choice : public Area {
	string name;				// 選択肢名
	int str_width;				// 表示される選択肢名の幅
public:
	Choice(int x, int y, int w, string str);
	void Draw();				// 文字及び、マウスオーバーのときの影を表示
	void DrawShade();			// 選択されているときの影を表示
};

// フォントデータを作るためのクラス
class FontMaker {
public:
	FontMaker();
};

// 設定画面クラス
class Config : public BaseScene, public FontMaker {
	CfgData temp;				// 変更データを一時保存

	Item h_form;
	Item auto_repro;
	Item sound_year;
	Item sound_week;
	Item tail_date;
	Item tail_time;
	Item for_prg;

	Button Reset;				// 既定値に戻すボタン
	Button Complete;			// 設定完了ボタン
	Button Cancel;				// キャンセルボタン
public:
	Config(ISceneChanger *changer);
	~Config();
	void Initialize();
	void Finalize() {}
	void Update();
	void Draw();
};

extern CfgData *pCfgData;

#endif
