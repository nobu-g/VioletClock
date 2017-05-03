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

// �O���[�o���I�u�W�F�N�g
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
    if (next_scene != eScene_None) {    // ���̃V�[�����Z�b�g����Ă�����
        pScene->Finalize();             // ���݂̃V�[���̏I�����������s
        delete pScene;
        switch (next_scene) {           // ���̃V�[���ɑΉ�����C���X�^���X�𐶐�����
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
        next_scene = eScene_None;       // ���̃V�[�������N���A
        pScene->Initialize();           // �V�[����������
    }

    pScene->Update();
}


void Main::Init()
{
    try {
        // �f�o�C�X��DPI���擾
        HDC screen = GetDC(0);
        scaleX = GetDeviceCaps(screen, LOGPIXELSX) / 96.0;
        scaleY = GetDeviceCaps(screen, LOGPIXELSY) / 96.0;
        ReleaseDC(0, screen);

        // �o�b�N�O���E���h�ł����삳����
        if (SetAlwaysRunFlag(TRUE) == -1)
            throw Exception(Exception::FuncError, "SetAlwaysRunFlag()");
        // 2�d�N��������
        if (SetDoubleStartValidFlag(TRUE) == -1)
            throw Exception(Exception::FuncError, "SetDoubleStartValidFlag()");
        // �E�C���h�E�̃A�C�R����ݒ�
        if (SetWindowIconID(101) == -1)
            throw Exception(Exception::FuncError, "SetWindowIconID()");
        // �w�i�����݂�F��
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

        // ���[�h��...
        switch (SCALEY(480)){
        case 480:   LoadGraphScreen(188, 220, "image\\96dpi\\Now Loading!!!!.png", TRUE);   break;
        case 600:   LoadGraphScreen(235, 275, "image\\120dpi\\Now Loading!!!!.png", TRUE);  break;
        case 720:   LoadGraphScreen(282, 330, "image\\144dpi\\Now Loading!!!!.png", TRUE);  break;
        default:    throw Exception(Exception::DPIError, "Now Loading!!!!.png");            break;
        }
        ScreenFlip();

        SetWindowText("���v");

        // �C���X�^���X�̐���
        pTimeSrc = new TimeSrc;
        pDate = new Date;
        pTime = new Time;
        pVoice = new Voice;
        pAlarm = new Alarm;
        pTimer = new Timer;

        // �ݒ�t�@�C���̓ǂݍ���
        std::ifstream cfg_data(config_file, std::ios::in | std::ios::binary);
        if (cfg_data) {
            cfg_data >> *pTimeSrc >> *pDate >> *pTime >> *pVoice >> *pAlarm >> *pTimer;
            cfg_data.close();
        }
        // �ݒ�t�@�C�������݂��Ȃ�������
        else
            pVoice->SetCfg(Voice::FirstTime, true);     // ����N���̈���

    }
    // �G���[�ƂȂ����ꍇ�A�t�@�C���ɋL�^
    catch (Exception e) {
        e.ExportError();
        exit(1);
    }
    catch (std::bad_alloc) {
        Exception(Exception::FailToAlloc, "Main::Init��").ExportError();
        exit(1);
    }
    catch (...) {
        Exception(Exception::Unexpected, "Main::Init��").ExportError();
        exit(1);
    }
}

void Main::Run()
{
    try {
        SceneMgr scene_mgr;
        scene_mgr.Initialize();

        while (!ProcessMessage() && !ScreenFlip() && !ClearDrawScreen()) {

            pTimeSrc->Update();     // �����̍X�V
            pAlarm->Update();       // �A���[����Ԃ��X�V
            pTimer->Update();       // �^�C�}�[��Ԃ��X�V
            scene_mgr.Update();     // �V�[�����X�V
            Area::Update();         // ��ʃN���b�N��Ԃ��X�V(�X�V�����̒��ł��Ō�ɍs��)
            scene_mgr.Draw();       // �`��
        }

        scene_mgr.Finalize();
    }
    catch (Exception e) {
        e.ExportError();
        exit(1);
    }
    catch (std::bad_alloc) {
        Exception(Exception::FailToAlloc, "Main::Run��").ExportError();
        exit(1);
    }
    catch (...) {
        Exception(Exception::Unexpected, "Main::Run��").ExportError();
        exit(1);
    }
}

void Main::End()
{
    try {
        // �ݒ��ۑ�
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
        Exception(Exception::Unexpected, "Main::End��").ExportError();
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
