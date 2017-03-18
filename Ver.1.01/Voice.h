#ifndef VOICE_H
#define VOICE_H

#include <array>
#include <map>
using std::array;
using std::map;

enum eTmData { None, Year, Month, Day, Week, Noon, Hour, TimeSignal, Minute, Tail };

// Voice関係の定数
#define YEAR		7		// 年の音声の数(2014～2020)
#define MONTH		12		// 月の音声の数(1～12月)
#define DAY			31		// 日の音声の数(1～31日)
#define WEEK		7		// 曜日の音声の数(日～土曜日)
#define NOON		2		// 午前・午後の音声の数
#define HOUR		24		// 時の音声の数(0～23時)
#define MINUTE		60		// 分の音声の数(0～59分)
#define TAIL		4		// 語尾の種類(…だね！…だよ!…になったよ!)
#define SEASON		10		// キャラが喋る日付依存の台詞の種類
#define SYSTEM		3		// システム音声の種類
#define WORDS		132		// キャラの台詞の種類

// キャラの台詞クラス
class Words {
	int hWord;				// 台詞音声へのハンドル

	// min_hour時min_minute分から、max_hour時max_minute分の間、台詞を喋ることができる
	int min_hour;
	int min_minute;
	int max_hour;
	int max_minute;

	bool programmer;		// プログラマ用の台詞かどうか
	bool kurei_kei;			// キャラが暮井慧かどうか
public:
	Words() {}
	Words(int minh, int maxh, bool prg, int hw);
	Words(double minh, double maxh, bool prg, int hw);
	Words(int minh, int maxh, bool prg, bool k, int hw);
	int GetHandle() { return hWord; }
	// 現時刻がmin_hour時min_minute分から、max_hour時max_minute分の間にあるか
	bool IsAppropriate();
};

// 音声管理クラス
class Voice {
	// それぞれ音声のハンドルを格納
	array<int, YEAR>	hYear;
	array<int, MONTH>	hMonth;
	array<int, DAY>		hDay;
	array<int, WEEK>	hWeek;
	array<int, NOON>	hNoon;
	array<int, HOUR>	hHour;
	array<int, HOUR>	hTime_signal;
	array<int, MINUTE>	hMinute;
	array<int, TAIL>	hTail;

	array<int, SEASON>	hSeason;
	array<int, SYSTEM>	hSystem;

	typedef unsigned char KeyCode;
	map<KeyCode, int>	keyboard;

	array<Words, WORDS>	words;

	int hPlaying;				// 最後に再生した音声のハンドル
public:
	Voice();									// 音声ファイルのロードなど
	bool IsPlaying()							// 現在音声が再生中か
	{
		return CheckSoundMem(hPlaying) && hPlaying != -1;
	}
	void SoundTime(eTmData data, int index);	// 時間関連の音を再生する
	void SoundWords();							// キャラのセリフを再生する
	void SoundSys(int index);					// システム音声を再生する
	void SoundKey(KeyCode key);					// キーを読み上げる
	void Repeat();								// 直前に再生した音声をもう一度再生
	void StopSound();							// 現在鳴っているサウンドを止める
};

extern Voice *pVoice;

#endif
