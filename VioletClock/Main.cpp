#include "DxLib.h"
#include "Main.h"
#include "Voice.h"
#include "Clock.h"
#include "Alarm.h"
#include "Timer.h"
#include "Exception.h"
#include <Windows.h>
#include <fstream>
#include <new>

// グローバルオブジェクト
TimeSrc *pTimeSrc = NULL;
Date *pDate = NULL;
Time *pTime = NULL;
Voice *pVoice = NULL;
Alarm *pAlarm = NULL;
Timer *pTimer = NULL;

const std::string config_file = "config.bin";
extern ofstream ferr;

SceneMgr::SceneMgr()
{
    pScene = (BaseScene *) new Clock(this);
    next_scene = eScene_None;
}

void SceneMgr::Initialize()
{
    pScene->Initialize();
}

void SceneMgr::Finalize()
{
    pScene->Finalize();
    delete pScene;
}

void SceneMgr::Update()
{
    if (next_scene != eScene_None) {    // 次のシーンがセットされていたら
        pScene->Finalize();             // 現在のシーンの終了処理を実行
        delete pScene;
        switch (next_scene) {           // 次のシーンに対応するインスタンスを生成する
        case eScene_Clock:
            pScene = (BaseScene *) new Clock(this);
            break;
        case eScene_Config:
            pScene = (BaseScene *) new Config(this);
            break;
        case eScene_Alarm:
            pScene = (BaseScene *) new AlarmScreen(this);
            break;
        case eScene_Timer:
            pScene = (BaseScene *) new TimerScreen(this);
            break;
        }
        next_scene = eScene_None;       // 次のシーン情報をクリア
        pScene->Initialize();           // シーンを初期化
    }

    pScene->Update();
}


void Main::Init()
{
    try {
        // デバイスのDPIを取得
        HDC screen = GetDC(0);
        scaleX = GetDeviceCaps(screen, LOGPIXELSX) / 96.0;
        scaleY = GetDeviceCaps(screen, LOGPIXELSY) / 96.0;
        ReleaseDC(0, screen);

        // バックグラウンドでも動作させる
        if (SetAlwaysRunFlag(TRUE) == -1)
            throw Exception(Exception::FuncError, "SetAlwaysRunFlag()");
        // 2重起動を許可
        if (SetDoubleStartValidFlag(TRUE) == -1)
            throw Exception(Exception::FuncError, "SetDoubleStartValidFlag()");
        // ウインドウのアイコンを設定
        switch (SCALEY(480)) {
        case 480:
            if (SetWindowIconID(101) == -1) throw Exception(Exception::FuncError, "SetWindowIconID()");
            break;
        case 600:
            if (SetWindowIconID(102) == -1) throw Exception(Exception::FuncError, "SetWindowIconID()");
            break;
        case 720:
            if (SetWindowIconID(103) == -1) throw Exception(Exception::FuncError, "SetWindowIconID()");
            break;
        default:
            throw Exception(Exception::OtherError, "Unsupported DPI");
            break;
        }
        // 背景をすみれ色に
        if (SetBackgroundColor(217, 190, 248) == -1)
            throw Exception(Exception::FuncError, "SetBackgroundColor()");
        if (ChangeWindowMode(TRUE) == -1)
            throw Exception(Exception::FuncError, "ChangeWindowMode()");
        if (SetGraphMode(SCALEX(640), SCALEY(480), 16) != DX_CHANGESCREEN_OK)
            throw Exception(Exception::FuncError, "SetGraphMode()");
        if (DxLib_Init() == -1)
            throw Exception(Exception::FuncError, "DxLib_Init()");
        if (SetDrawScreen(DX_SCREEN_BACK) == -1)
            throw Exception(Exception::FuncError, "SetDrawScreen()");

        // ロード中...
        switch (SCALEY(480)){
        case 480:   LoadGraphScreen(188, 220, "image\\96dpi\\Now Loading!!!!.png", TRUE);   break;
        case 600:   LoadGraphScreen(235, 275, "image\\120dpi\\Now Loading!!!!.png", TRUE);  break;
        case 720:   LoadGraphScreen(282, 330, "image\\144dpi\\Now Loading!!!!.png", TRUE);  break;
        }
        ScreenFlip();

        SetWindowText("時計");

        // インスタンスの生成
        pTimeSrc = new TimeSrc;
        pDate = new Date;
        pTime = new Time;
        pVoice = new Voice;
        pAlarm = new Alarm;
        pTimer = new Timer;

        // 設定ファイルの読み込み
        std::ifstream cfg_data(config_file, std::ios::in | std::ios::binary);
        if (cfg_data) {
            cfg_data >> *pTimeSrc >> *pDate >> *pTime >> *pVoice >> *pAlarm >> *pTimer;
            cfg_data.close();
        }
        // 設定ファイルが存在しなかった時
        else
            pVoice->SetCfg(Voice::FirstTime, true);     // 初回起動の扱い

    }
    // エラーとなった場合、ファイルに記録
    catch (Exception e) {
        e.ExportError();
        exit(1);
    }
    catch (std::bad_alloc) {
        Exception(Exception::FailToAlloc, "Main::Init内").ExportError();
        exit(1);
    }
    catch (...) {
        Exception(Exception::Unexpected, "Main::Init内").ExportError();
        exit(1);
    }
}

void Main::Run()
{
    try {
        SceneMgr scene_mgr;
        scene_mgr.Initialize();

        while (!ProcessMessage() && !ScreenFlip() && !ClearDrawScreen()) {

            pTimeSrc->Update();     // 時刻の更新
            pAlarm->Update();       // アラーム状態を更新
            pTimer->Update();       // タイマー状態を更新
            scene_mgr.Update();     // シーンを更新
            Area::Update();         // 画面クリック状態を更新(更新処理の中でも最後に行う)
            scene_mgr.Draw();       // 描画
        }

        scene_mgr.Finalize();
    }
    catch (Exception e) {
        e.ExportError();
        exit(1);
    }
    catch (std::bad_alloc) {
        Exception(Exception::FailToAlloc, "Main::Run内").ExportError();
        exit(1);
    }
    catch (...) {
        Exception(Exception::Unexpected, "Main::Run内").ExportError();
        exit(1);
    }
}

void Main::End()
{
    try {
        // 設定を保存
        std::ofstream cfg_data(config_file, std::ios::out | std::ios::binary);
        if (cfg_data) {
            cfg_data << *pTimeSrc << *pDate << *pTime << *pVoice << *pAlarm << *pTimer;
            cfg_data.close();
        }

        delete pTimeSrc;
        delete pDate;
        delete pTime;
        delete pVoice;
        delete pAlarm;
        delete pTimer;
        ferr.close();
        if (DxLib_End() == -1)
            throw Exception(Exception::FuncError, "DxLib_End()");
    }
    catch (Exception e) {
        e.ExportError();
        exit(1);
    }
    catch (...) {
        Exception(Exception::Unexpected, "Main::End内").ExportError();
        exit(1);
    }
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
