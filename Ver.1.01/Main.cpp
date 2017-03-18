#include "DxLib.h"
#include "Main.h"
#include "Config.h"
#include "Voice.h"
#include "Clock.h"
#include <fstream>
#include <new>

// グローバルオブジェクト
Date *pDate = NULL;
Time *pTime = NULL;
Chr *pChr = NULL;
RptButton *pRptButton = NULL;
CfgButton *pCfgButton = NULL;
MuteButton *pMuteButton = NULL;
CfgData *pCfgData = NULL;
Voice *pVoice = NULL;

const std::string config_file = "config.bin";
std::ofstream ferr("error.log");

SceneMgr::SceneMgr()
{
	pScene = (BaseScene *) new Clock(this);
	next_scene = eScene_None;
}

void SceneMgr::Initialize()
{
	pScene->Initialize();

	// 設定ファイルの読み込み
	std::ifstream cfg_data(config_file, std::ios::in | std::ios::binary);
	if (cfg_data) {
		cfg_data.read((char *)pCfgData, sizeof(*pCfgData));
		cfg_data.close();
	}
	// 設定ファイルが存在しなかった時
	else {
		pCfgData->Setup();							// デフォルト設定を適用
		pCfgData->SetCfg(CfgData::FirstTime, true);	// 初回起動の扱い
	}
}

void SceneMgr::Finalize()
{
	pScene->Finalize();
	delete pScene;

	// 設定を保存
	std::ofstream cfg_data(config_file, std::ios::out | std::ios::binary);
	if (cfg_data) {
		cfg_data.write((char *)pCfgData, sizeof(*pCfgData));
		cfg_data.close();
	}
}

void SceneMgr::Update()
{
	if (next_scene != eScene_None) {	// 次のシーンがセットされていたら
		pScene->Finalize();				// 現在のシーンの終了処理を実行
		delete pScene;
		switch (next_scene) {			// 次のシーンに対応するインスタンスを生成する
		case eScene_Clock:
			pScene = (BaseScene *) new Clock(this);
			break;
		case eScene_Config:
			pScene = (BaseScene *) new Config(this);
			break;
		}
		next_scene = eScene_None;		// 次のシーン情報をクリア
		pScene->Initialize();			// シーンを初期化
	}

	pScene->Update();
}


void Main::Init()
{
	if (!ferr)
		exit(1);

	SetAlwaysRunFlag(TRUE);				// バックグラウンドでも動作させる
	SetDoubleStartValidFlag(TRUE);		// 2重起動を許可
	SetWindowIconID(101);				// ウインドウのアイコンを設定
	try {
		// 背景をすみれ色に
		if (SetBackgroundColor(217, 190, 248) == -1)
			throw "SetBackgroundColor()";
		if (ChangeWindowMode(TRUE) == -1)
			throw "ChangeWindowMode()";
		if (DxLib_Init() == -1)
			throw "DxLib_Init()";
		if (SetDrawScreen(DX_SCREEN_BACK) == -1)
			throw "SetDrawScreen()";
	}
	// エラーとなった場合、ファイルに記録
	catch (char *fname) {
		ferr << "エラー: " << fname << "関数" << std::endl;
		exit(1);
	}

	// ロード中...
	LoadGraphScreen(188, 220, "image\\Now Loading!!!!.png", TRUE);
	ScreenFlip();

	SetWindowText("時計");

	try {
		// インスタンスの生成
		pDate = new Date;
		pTime = new Time;
		pChr = new Chr;
		pRptButton = new RptButton;
		pCfgButton = new CfgButton;
		pMuteButton = new MuteButton;
		pCfgData = new CfgData;
		pVoice = new Voice;
	}
	catch (std::bad_alloc) {
		ferr << "メモリ割り当て失敗\n" << std::endl;
		return;
	}
	catch (int i) {
		ferr << "ロードの中断(場所:" << i << ')' << std::endl;
		return;
	}
}

void Main::Run()
{
	SceneMgr scene_mgr;
	scene_mgr.Initialize();

	while (!ProcessMessage() && !ScreenFlip() && !ClearDrawScreen()) {

		TimeSrc::Update();	// 時刻の更新
		scene_mgr.Update();	// シーンを更新
		Area::Update();		// 画面クリック状態を更新
		scene_mgr.Draw();	// 描画
	}

	scene_mgr.Finalize();
}

void Main::End()
{
	delete pDate;
	delete pTime;
	delete pChr;
	delete pRptButton;
	delete pCfgButton;
	delete pMuteButton;
	delete pCfgData;
	delete pVoice;
	ferr.close();
	DxLib_End();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLinem, int nCmdShow)
{
	Main main_ob;

	main_ob.Init();
	main_ob.Run();
	main_ob.End();

	return 0;
}
