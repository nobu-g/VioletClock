#ifndef CLOCK_H
#define CLOCK_H

#include "Main.h"
#include "Config.h"
#include <ctime>

// Time, Date関係の定数
#define DATE_X		270		// 日付左上のx座標
#define DATE_Y		330		// 日付左上のy座標
#define TIME_X		150		// 時刻左上のx座標
#define TIME_Y		150		// 時刻左上のy座標
#define FSIZE_D		30		// 日付表示用フォントのサイズ
#define FSIZE_HM	150		// 時、分用フォントのサイズ
#define FSIZE_S		50		// 秒用フォントのサイズ
#define FSIZE_AP	35		// AM/PM用フォントのサイズ
#define SPACE_MS	10		// 分と秒の間の間隔(ピクセル)

// メイン画面クラス
class Clock : public BaseScene {
public:
	Clock(ISceneChanger *changer) : BaseScene(changer){}
	void Initialize();
	void Finalize();
	void Update();
	void Draw();
};

// 標準時を提供する時間クラス
class TimeSrc {
protected:
	static struct tm *local;	// 時間を格納する構造体
public:
	static void Update();		// 現在時刻を更新
};

// 日付クラス
class Date : public Area, public TimeSrc {
	int hFont_d;				// 日付表示用フォント
public:
	Date();
	int Gety() { return local->tm_year + 1900; }				// 年を返す
	int Getm() { return local->tm_mon + 1; }					// 月を返す
	int Getd() { return local->tm_mday; }						// 日を返す
	int Getw() { return local->tm_wday; }						// 曜日を返す
	int Gett() { return pCfgData->GetCfg(CfgData::TailDate); }	// 語尾を返す
	void Draw();
	void Action();					// クリックされたときの動作
	bool IsNewYear()				// 今日が元日(1/1)かどうか
	{
		return local->tm_mon == 0 && local->tm_mday == 1;
	}
	bool IsHalloween()				// 今日がハロウィン(10/31)かどうか				
	{
		return local->tm_mon == 9 && local->tm_mday == 31;
	}
	bool IsChristmas()				// 今日がクリスマス(12/25)かどうか
	{
		return local->tm_mon == 11 && local->tm_mday == 25;
	}
	bool IsBirthday()				// 今日がCV:上坂すみれの誕生日(12/19)か
	{
		return local->tm_mon == 11 && local->tm_mday == 19;
	}
	bool IsSummer()					// 今が夏(6月～8月)かどうか
	{
		return local->tm_mon >= 6 && local->tm_mon <= 8;
	}
	bool IsWinter()					// 今が冬(12月～3月)かどうか
	{
		return local->tm_mon >= 11 && local->tm_mon <= 2;
	}
};

// 時刻クラス
class Time : public Area, public TimeSrc {
	int hFont_hm;				// 時、分用フォント
	int hFont_s;				// 秒用フォント
	int hFont_ap;				// AM/PM用フォント
	
	bool time_signal_flag;		// 時報を鳴らさなければならないか
public:
	Time();
	void Init() { time_signal_flag = false; }					// 初期化処理
	int Getn() { return (local->tm_hour < 12) ? 0 : 1; }		// 午前:0、午後:1を返す
	int Geth() { return local->tm_hour; }						// 時を返す
	int Getm() { return local->tm_min; }						// 分を返す
	int Gets() { return local->tm_sec; }						// 秒を返す
	int Gett() { return pCfgData->GetCfg(CfgData::TailTime); }	// 語尾を返す
	void Draw();
	void Action();			// クリックされたときの動作
};

extern Date *pDate;
extern Time *pTime;

#endif
