#include "DxLib.h"
#include "Clock.h"
#include "Exception.h"

// extern��t���Ȃ����const�ϐ��̓f�t�H���g�œ��������P�[�W������
extern const array<char *, WEEK> week_name = { "��", "��", "��", "��", "��", "��", "�y" };
extern std::ofstream ferr;

void Clock::Initialize()
{
    pTime->Init();
}

void Clock::Finalize()
{
    pVoice->StopSound();
}

void Clock::Update()
{
    pDate->Update();
    pTime->Update();
    character.Update();
    rpt_btn.Update();
    mute_btn.Update();

    if (cfg_btn.IsClicked())
        pScene_changer->ChangeScene(eScene_Config);     // �V�[����ݒ��ʂɕύX
    else if (alm_btn.IsClicked())
        pScene_changer->ChangeScene(eScene_Alarm);      // �V�[�����A���[����ʂɕύX
    else if (tmr_btn.IsClicked())
        pScene_changer->ChangeScene(eScene_Timer);      // �V�[�����^�C�}�[��ʂɕύX
}

void Clock::Draw()
{
    // ��ʂ̕`��
    pTime->Draw();
    pDate->Draw();
    character.Draw();
    rpt_btn.Draw();
    cfg_btn.Draw();
    mute_btn.Draw();
    alm_btn.Draw();
    tmr_btn.Draw();
}

TimeSrc::TimeSrc()
{
    config.offset = 0;
    time_t t = time(NULL) + config.offset;
    if (localtime_s(&local, &t) == -1)
        throw Exception(Exception::FuncError, "localtime_s()");
}

void TimeSrc::Update()
{
    time_t t = time(NULL) + config.offset;
    if (localtime_s(&local, &t) == -1)
        throw Exception(Exception::FuncError, "localtime_s()");
}

void TimeSrc::SetCfg(time_t t)
{
    config.offset = t;
    config.Check();
}

Date::Date() :
    Area::Area(DATE_X, DATE_Y, 0, FSIZE_D)
{
    hFont_d = CreateFontToHandle(NULL, FSIZE_D, -1, DX_FONTTYPE_ANTIALIASING_4X4);
    speaking = None;
    config.sound_year = true;
    config.sound_week = true;
    config.tail = Tail_None;
    // �f�t�H���g��CV:��₷�݂�̒a����(12/19)
    config.birthmonth = 12;
    config.birthday = 19;
}

Time::Time() :
    Area::Area(TIME_X, TIME_Y, STOW(FSIZE_HM) * 5 + SPACE_MS + STOW(FSIZE_S) * 2, FSIZE_HM)
{
    hFont_hm = CreateFontToHandle(NULL, FSIZE_HM, -1, DX_FONTTYPE_ANTIALIASING_4X4);
    hFont_s = CreateFontToHandle(NULL, FSIZE_S, 4, DX_FONTTYPE_ANTIALIASING_4X4);
    hFont_ap = CreateFontToHandle(NULL, FSIZE_AP, 2, DX_FONTTYPE_ANTIALIASING_4X4);
    speaking = None;
    config.h_form = Form24h;
    config.tail = Tail_None;
}

void Date::Draw()
{
    DrawFormatStringToHandle(x1, y1, BLACK, hFont_d, "%d�N%d��%d��(%s)", Gety(), Getm(), Getd(), week_name[Getw()]);
}

void Time::Draw()
{
    int hour = Geth();

    // 0-12�\���̎�
    if (config.h_form == Form12h) {
        // ���Ԃ�0�`12��
        if (Getn())
            hour -= HOUR / 2;

        // AM/PM��\��
        DrawFormatStringToHandle(x1 + STOW(FSIZE_HM) * 5 + SPACE_MS, y1 + 10, BLACK, hFont_ap, Getn() ? "PM" : "AM");
    }

    // ���A����\��
    DrawFormatStringToHandle(x1, y1, BLACK, hFont_hm, "%2d:%02d", hour, Getm());
    // �b��\��
    DrawFormatStringToHandle(x1 + STOW(FSIZE_HM) * 5 + SPACE_MS, y1 + FSIZE_HM - FSIZE_S - 15, BLACK, hFont_s, "%02d", Gets());
}

void Date::Update()
{
    // �\�����̍X�V
    SetWidth(GetDrawFormatStringWidthToHandle(hFont_d, "%d�N%d��%d��(%s)", Gety(), Getm(), Getd(), week_name[Getw()]));

    const int year_offset = 2014;   // ������2014�N����

    // ���݂̓��t�̉������Đ�����
    if (!pVoice->IsPlaying()) {
        if (IsClicked() && speaking == None) {
            if (config.sound_year)
                pVoice->SoundTime(Year, Gety() - year_offset);
            speaking = Year;
        }
        else if (speaking == Year) {
            pVoice->SoundTime(Month, Getm() - 1);
            speaking = Month;
        }
        else if (speaking == Month) {
            pVoice->SoundTime(Day, Getd() - 1);
            speaking = Day;
        }
        else if (speaking == Day) {
            if (config.sound_week)
                pVoice->SoundTime(Week, Getw());
            speaking = Week;
        }
        else if (speaking == Week) {
            if (Gett() < TAIL)
                pVoice->SoundTime(Tail, Gett());
            speaking = Tail;
        }
        else if (speaking == Tail)
            speaking = None;
    }
}

void Time::Update()
{
    if (!pVoice->IsPlaying()) {
        // ���ݎ����̉������Đ�����
        if (IsClicked() && speaking == None) {
            if (config.h_form == Form12h)
                pVoice->SoundTime(Noon, Getn());
            speaking = Noon;
        }
        else if (speaking == Noon) {
            int hour = Geth();
            if (config.h_form == Form12h && Getn())
                hour -= HOUR / 2;
            pVoice->SoundTime(Hour, hour);
            speaking = Hour;
        }
        else if (speaking == Hour) {
            pVoice->SoundTime(Minute, Getm());
            speaking = Minute;
        }
        else if (speaking == Minute) {
            if (Gett() < TAIL)
                pVoice->SoundTime(Tail, Gett());
            speaking = Tail;
        }
        else if (speaking == Tail || speaking == TimeSignal)
            speaking = None;

        // �����炷
        else if (time_signal_flag && !pVoice->IsPlaying()) {
            if (pVoice->GetCfg().auto_repro) {
                pVoice->SoundTime(TimeSignal, Geth());
                speaking = TimeSignal;
            }
            time_signal_flag = false;
        }
    }

    // �������傤�ǂɂȂ����Ƃ������炷
    if (Getm() == 0 && Gets() == 0 && speaking != TimeSignal)
        time_signal_flag = true;
}

void Date::ResetSpeaking()
{
    speaking = None;
}

void Time::ResetSpeaking()
{
    speaking = None;
}

void Date::SetCfg(eCfg cfg, int value)
{
    switch (cfg) {
    case SoundYear:  config.sound_year = (value != 0); break;
    case SoundWeek:  config.sound_week = (value != 0); break;
    case TailDate:   config.tail = (eTail)value;       break;
    case BirthMonth: config.birthmonth = value;        break;
    case BirthDay:   config.birthday = value;          break;
    }
    config.Check();
}

void Time::SetCfg(eCfg cfg, int value)
{
    switch (cfg) {
    case HForm:    config.h_form = (eHourForm)value; break;
    case TailTime: config.tail   = (eTail)value;     break;
    }
    config.Check();
}
