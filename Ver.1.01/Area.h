#ifndef SCREEN_H
#define SCREEN_H

#include <array>

// キャラクタ関係の定数
#define CHR_X		10		// キャラクタ左上のx座標
#define CHR_Y		170		// キャラクタ左上のy座標
#define CHR_W		122		// キャラクタの幅
#define CHR_H		268		// キャラクタの高さ
#define CHR_NUM		1		// キャラクタの種類

// バッテリー関係の定数
#define BAT_LOW		20		// バッテリーが減ったとみなす残量(%)
#define BAT_CRISIS	10		// バッテリー限界とみなす残量(%)

// キーボード関係の定数
#define KEY_INPUT_WIN	(0xDA)
#define KEY_INPUT_CTRL	(0x5D)
#define KEY_INPUT_ALT	(0x78)
#define KEY_INPUT_SHIFT	(0x29)

// もう一度再生ボタン関係の定数
#define RPT_X		500		// もう一度再生ボタン左上のx座標
#define RPT_Y		400		// もう一度再生ボタン左上のy座標
#define RPT_W		40		// もう一度再生ボタンの幅
#define RPT_H		40	 	// もう一度再生ボタンの高さ

// 設定ボタン関係の定数
#define CFG_X		570		// 設定ボタン左上のx座標
#define CFG_Y		400		// 設定ボタン左上のy座標
#define CFG_W		40		// 設定ボタンの幅
#define CFG_H		40		// 設定ボタンの高さ

// ミュートボタン関係の定数
#define MUTE_X		430		// ミュートボタン左上のx座標
#define MUTE_Y		400		// ミュートボタン左上のy座標
#define MUTE_W		40		// ミュートボタンの幅
#define MUTE_H		40		// ミュートボタンの高さ

// メイン画面の構成要素の基本クラス
class Area {
	bool is_clicking;				// エリア内でクリックされ、今もその状態かどうか
protected:
	int x1, y1, x2, y2;				// オブジェクトの座標
	static bool pre_lclick_status;	// 前ループで左クリックされていたかどうか
public:
	Area(int x, int y, int w = 0, int h = 0)
	{
		x1 = x; y1 = y; x2 = x + w; y2 = y + h;
	}
	void SetWidth(int w) { x2 = x1 + w; }		// 幅がwとなるようx2を変更
	void SetHeight(int h) { y2 = y1 + h; }		// 高さがhとなるようy2を変更
	static void Update();						// クリック状態の更新．1ループに1回しか実行してはいけない
	bool IsMouseOver();							// オブジェクト上にマウスカーソルがあるか
	bool IsClicked();							// オブジェクト上で左クリックがされた瞬間のみtrueを返す
	bool IsClicking();							// エリア内でクリックされ、今もその状態かどうか
};

// キャラクタクラス
class Chr : public Area {
	std::array<int, 25> hChr;
public:
	Chr();
	void Draw();
	void Action();			// クリックされたときの動作
};

// もう一度再生ボタンクラス
class RptButton : public Area {
	int hImage;
public:
	RptButton();
	void Draw();
	void Action();
};

// 設定ボタンクラス
class CfgButton : public Area {
	int hImage;
public:
	CfgButton();
	void Draw();
};

// ミュートボタンクラス
class MuteButton : public Area {
	int hSnd_img;
	int hMute_img;
public:
	MuteButton();
	void Draw();
	void Action();			// クリックされたときの動作
};

extern Chr *pChr;
extern RptButton *pRptButton;
extern CfgButton *pCfgButton;
extern MuteButton *pMuteButton;

#endif
