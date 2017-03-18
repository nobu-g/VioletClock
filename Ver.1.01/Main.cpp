#include "DxLib.h"
#include "Main.h"
#include "Config.h"
#include "Voice.h"
#include "Clock.h"
#include <fstream>
#include <new>

// �O���[�o���I�u�W�F�N�g
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

	// �ݒ�t�@�C���̓ǂݍ���
	std::ifstream cfg_data(config_file, std::ios::in | std::ios::binary);
	if (cfg_data) {
		cfg_data.read((char *)pCfgData, sizeof(*pCfgData));
		cfg_data.close();
	}
	// �ݒ�t�@�C�������݂��Ȃ�������
	else {
		pCfgData->Setup();							// �f�t�H���g�ݒ��K�p
		pCfgData->SetCfg(CfgData::FirstTime, true);	// ����N���̈���
	}
}

void SceneMgr::Finalize()
{
	pScene->Finalize();
	delete pScene;

	// �ݒ��ۑ�
	std::ofstream cfg_data(config_file, std::ios::out | std::ios::binary);
	if (cfg_data) {
		cfg_data.write((char *)pCfgData, sizeof(*pCfgData));
		cfg_data.close();
	}
}

void SceneMgr::Update()
{
	if (next_scene != eScene_None) {	// ���̃V�[�����Z�b�g����Ă�����
		pScene->Finalize();				// ���݂̃V�[���̏I�����������s
		delete pScene;
		switch (next_scene) {			// ���̃V�[���ɑΉ�����C���X�^���X�𐶐�����
		case eScene_Clock:
			pScene = (BaseScene *) new Clock(this);
			break;
		case eScene_Config:
			pScene = (BaseScene *) new Config(this);
			break;
		}
		next_scene = eScene_None;		// ���̃V�[�������N���A
		pScene->Initialize();			// �V�[����������
	}

	pScene->Update();
}


void Main::Init()
{
	if (!ferr)
		exit(1);

	SetAlwaysRunFlag(TRUE);				// �o�b�N�O���E���h�ł����삳����
	SetDoubleStartValidFlag(TRUE);		// 2�d�N��������
	SetWindowIconID(101);				// �E�C���h�E�̃A�C�R����ݒ�
	try {
		// �w�i�����݂�F��
		if (SetBackgroundColor(217, 190, 248) == -1)
			throw "SetBackgroundColor()";
		if (ChangeWindowMode(TRUE) == -1)
			throw "ChangeWindowMode()";
		if (DxLib_Init() == -1)
			throw "DxLib_Init()";
		if (SetDrawScreen(DX_SCREEN_BACK) == -1)
			throw "SetDrawScreen()";
	}
	// �G���[�ƂȂ����ꍇ�A�t�@�C���ɋL�^
	catch (char *fname) {
		ferr << "�G���[: " << fname << "�֐�" << std::endl;
		exit(1);
	}

	// ���[�h��...
	LoadGraphScreen(188, 220, "image\\Now Loading!!!!.png", TRUE);
	ScreenFlip();

	SetWindowText("���v");

	try {
		// �C���X�^���X�̐���
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
		ferr << "���������蓖�Ď��s\n" << std::endl;
		return;
	}
	catch (int i) {
		ferr << "���[�h�̒��f(�ꏊ:" << i << ')' << std::endl;
		return;
	}
}

void Main::Run()
{
	SceneMgr scene_mgr;
	scene_mgr.Initialize();

	while (!ProcessMessage() && !ScreenFlip() && !ClearDrawScreen()) {

		TimeSrc::Update();	// �����̍X�V
		scene_mgr.Update();	// �V�[�����X�V
		Area::Update();		// ��ʃN���b�N��Ԃ��X�V
		scene_mgr.Draw();	// �`��
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
