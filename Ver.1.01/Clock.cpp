#include "DxLib.h"
#include "Clock.h"
#include "Voice.h"

#define BLACK	0x000000

const array<char *, WEEK> week_name = { "��", "��", "��", "��", "��", "��", "�y" };
static eTmData data = None;		// ���ݍĐ�����Ă��鉹��

struct tm *TimeSrc::local;

void Clock::Initialize()
{
	pTime->Init();
}

void Clock::Finalize()
{
	pVoice->StopSound();
	data = None;
}

void Clock::Update()
{
	pDate->Action();
	pTime->Action();
	pChr->Action();
	pRptButton->Action();
	pMuteButton->Action();

	if(pCfgButton->IsClicked())
		pScene_changer->ChangeScene(eScene_Config);		// �V�[����ݒ��ʂɕύX
}

void Clock::Draw()
{
	// ��ʂ̕`��
	pTime->Draw();
	pDate->Draw();
	pChr->Draw();
	pRptButton->Draw();
	pCfgButton->Draw();
	pMuteButton->Draw();
}

void TimeSrc::Update()
{
	time_t t = time(NULL);
	local = localtime(&t);
}

Date::Date() :
	Area::Area(DATE_X, DATE_Y, 0, FSIZE_D)
{
	hFont_d = CreateFontToHandle(NULL, FSIZE_D, -1, DX_FONTTYPE_ANTIALIASING_4X4);
}

Time::Time() :
	Area::Area(TIME_X, TIME_Y, FSIZE_HM * 5 / 2 + SPACE_MS + FSIZE_S, FSIZE_HM)
{
	hFont_hm = CreateFontToHandle(NULL, FSIZE_HM, -1, DX_FONTTYPE_ANTIALIASING_4X4);
	hFont_s  = CreateFontToHandle(NULL, FSIZE_S,  4,  DX_FONTTYPE_ANTIALIASING_4X4);
	hFont_ap = CreateFontToHandle(NULL, FSIZE_AP, 2,  DX_FONTTYPE_ANTIALIASING_4X4);
}

void Date::Draw()
{
	DrawFormatStringToHandle(x1, y1, BLACK, hFont_d, "%d�N%d��%d��(%s)", Gety(), Getm(), Getd(), week_name[Getw()]);
}

void Time::Draw()
{
	int hour = Geth();

	// 0-12�\���̎�
	if (pCfgData->GetCfg(CfgData::HForm) == CfgData::Form12h) {
		// ���Ԃ�0�`12��
		if (Getn())
			hour -= HOUR / 2;

		// AM/PM��\��
		DrawFormatStringToHandle(x1 + FSIZE_HM * 5 / 2 + SPACE_MS, y1 + 10, BLACK, hFont_ap, Getn() ? "PM" : "AM");
	}

	// ���A����\��
	DrawFormatStringToHandle(x1, y1, BLACK, hFont_hm, "%2d:%02d", hour, Getm());
	// �b��\��
	DrawFormatStringToHandle(x1 + FSIZE_HM * 5 / 2 + SPACE_MS, y1 + FSIZE_HM - FSIZE_S - 15, BLACK, hFont_s, "%02d", Gets());
}

void Date::Action()
{
	// �\�����̍X�V
	SetWidth(GetDrawFormatStringWidthToHandle(hFont_d, "%d�N%d��%d��(%s)", Gety(), Getm(), Getd(), week_name[Getw()]));

	const int year_offset = 2014;	// ������2014�N����

	// ���݂̓��t�̉������Đ�����
	if (!pVoice->IsPlaying()) {
		if (IsClicked() && data == None) {
			if (pCfgData->GetCfg(CfgData::SoundYear))
				pVoice->SoundTime(Year, Gety() - year_offset);
			data = Year;
		}
		else if (data == Year) {
			pVoice->SoundTime(Month, Getm() - 1);
			data = Month;
		}
		else if (data == Month) {
			pVoice->SoundTime(Day, Getd() - 1);
			data = Day;
		}
		else if (data == Day) {
			if (pCfgData->GetCfg(CfgData::SoundWeek))
				pVoice->SoundTime(Week, Getw());
			data = Week;
		}
		else if (data == Week) {
			if (Gett() < TAIL)
				pVoice->SoundTime(Tail, Gett());
			data = Tail;
		}
		else if (data == Tail)
			data = None;
	}
}

void Time::Action()
{
	if (!pVoice->IsPlaying()) {
		// ���ݎ����̉������Đ�����
		if (IsClicked() && data == None) {
			if (pCfgData->GetCfg(CfgData::HForm) == CfgData::Form12h)
				pVoice->SoundTime(Noon, Getn());
			data = Noon;
		}
		else if (data == Noon) {
			int hour = Geth();
			if (pCfgData->GetCfg(CfgData::HForm) == CfgData::Form12h && Getn())
				hour -= HOUR / 2;
			pVoice->SoundTime(Hour, hour);
			data = Hour;
		}
		else if (data == Hour) {
			pVoice->SoundTime(Minute, Getm());
			data = Minute;
		}
		else if (data == Minute) {
			if (Gett() < TAIL)
				pVoice->SoundTime(Tail, Gett());
			data = Tail;
		}
		else if (data == Tail || data == TimeSignal)
			data = None;

		// �����炷
		else if (time_signal_flag && !pVoice->IsPlaying()) {
			if (pCfgData->GetCfg(CfgData::AutoRepro)) {
				pVoice->SoundTime(TimeSignal, Geth());
				data = TimeSignal;
			}
			time_signal_flag = false;
		}
	}

	// �������傤�ǂɂȂ����Ƃ������炷
	if (Getm() == 0 && Gets() == 0 && data != TimeSignal)
		time_signal_flag = true;
}
