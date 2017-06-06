#ifndef AREA_H
#define AREA_H

#include <fstream>
using std::ofstream;
using std::ifstream;

// DPIに応じてパーツをスケーリング
extern double scaleX;
extern double scaleY;
#define SCALEX(argX)    ((int) ((argX) * scaleX))
#define SCALEY(argY)    ((int) ((argY) * scaleY))

// フォントサイズ→半角文字幅
#define STOW(arg)       ((int) (((arg) + 1) / 2))

// 色指定
#define BLACK               0x000000    // 文字色
#define VIOLET1             0xE5D7F7    // 設定エリアの背景色(229, 215, 247)
#define VIOLET2             0xD1B2F7    // 設定項目ボタン背景色
#define VIOLET3             0xC399F7    // 主要ボタンの背景色
#define VIOLET4             0x9442F7    // カーソルの場所を示す枠の色
/*色コードマクロは以下のサイトを参考に、薄い順に並んでいる
http://www.color-sample.com/colors/d9bef8/
*/

enum eScene {
    eScene_Clock,       // メイン画面
    eScene_Config,      // 設定画面
    eScene_Alarm,       // アラーム設定画面
    eScene_Timer,       // タイマー画面
    eScene_None         // 無し
};

// シーンを変更するためのインターフェイスクラス
class ISceneChanger {
public:
    virtual void ChangeScene(eScene next) = 0;
};

// シーンの基底クラス
class BaseScene {
protected:
    // クラス所有元にシーン切り替えを伝えるインターフェイス
    ISceneChanger *pScene_changer;
public:
    BaseScene(ISceneChanger *pChanger) { pScene_changer = pChanger; }
    virtual ~BaseScene() {}
    virtual void Initialize() {}
    virtual void Finalize() {}
    virtual void Update() {}
    virtual void Draw() {}
};

// 画面の構成要素の基本クラス
class Area {
    bool is_clicking;               // エリア内でクリックされ、今もその状態かどうか
protected:
    int x1, y1, x2, y2;             // オブジェクトの座標
    static bool pre_lclick_status;  // 前ループで左クリックされていたかどうか
    static bool pre_hitkey_status;  // 前ループでキーボードが押されていたかどうか
public:
    Area(int x, int y, int w = 0, int h = 0)
    {
        x1 = x; y1 = y; x2 = x + w; y2 = y + h;
    }
    void SetWidth(int w) { x2 = x1 + w; }       // 幅がwとなるようx2を変更
    void SetHeight(int h) { y2 = y1 + h; }      // 高さがhとなるようy2を変更
    static void Update();                       // クリック状態の更新．1ループに1回しか実行してはいけない
    bool IsMouseOver();                         // オブジェクト上にマウスカーソルがあるか
    bool IsClicked();                           // オブジェクト上で左クリックがされた瞬間のみtrueを返す
    bool IsClicking();                          // エリア内でクリックされ、今もその状態かどうか
    friend class Alarm;                         // Alarmクラス内でもpre_lclick_statusが使えるように
    friend class ConfigTime;                    // ConfigTimeクラス内でもpre_hitkey_statusが使えるように
};

// 挿入子と抽出子をテンプレート関数として宣言する
template<class CfgType> class IOBase;
template<class CfgType>
ofstream& operator << (ofstream &stream, const IOBase<CfgType> &rhs);
template<class CfgType>
ifstream& operator >> (ifstream &stream, IOBase<CfgType> &rhs);

// ファイル入出力のための挿入子・抽出子を提供する基本クラス
template<class CfgType> class IOBase {
protected:
    CfgType config;
public:
    friend ofstream& operator << <CfgType> (ofstream &stream, const IOBase<CfgType> &rhs);
    friend ifstream& operator >> <CfgType> (ifstream&, IOBase<CfgType>&);
    CfgType GetCfg() { return config; }                                 // 設定を返す
};

template<class CfgType>
ofstream &operator << (ofstream& stream, const IOBase<CfgType>& rhs)
{
    stream.write((char *)&rhs.config, sizeof(CfgType));

    if (stream.bad())
        throw Exception(Exception::OutputError, "ios::badbit");
    else if (stream.fail()) {
        Exception(Exception::OutputError, "ios::failbit").ExportError();
        stream.clear();
    }

    return stream;
}

template<class CfgType>
ifstream &operator >> (ifstream& stream, IOBase<CfgType>& rhs)
{
    stream.read((char *)&rhs.config, sizeof(CfgType));
    rhs.config.Check();     // 読み込んだ値が適正かチェック及び修正する

    if (stream.bad())
        throw Exception(Exception::InputError, "ios::badbit");
    else if (stream.fail()) {
        Exception(Exception::InputError, "ios::failbit").ExportError();
        stream.clear();
    }

    return stream;
}

#endif // !AREA_H
