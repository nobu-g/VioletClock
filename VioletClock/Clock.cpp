#include "DxLib.h"
#include "Clock.h"
#include "Exception.h"

// externを付けなければconst変数はデフォルトで内部リンケージを持つ
extern const array<char *, WEEK> week_name = { "日", "月", "火", "水", "木", "金", "土" };
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
        pScene_changer->ChangeScene(eScene_Config);     // シーンを設定画面に変更
    else if (alm_btn.IsClicked())
        pScene_changer->ChangeScene(eScene_Alarm);      // シーンをアラーム画面に変更
    else if (tmr_btn.IsClicked())
        pScene_changer->ChangeScene(eScene_Timer);      // シーンをタイマー画面に変更
}

void Clock::Draw()
{
    // 画面の描画
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
    // デフォルトはCV:上坂すみれの誕生日(12/19)
    config.birthmonth = 12;
    config.birthday = 19;
}

Time::Time() :
    Area::Area(TIME_X, TIME_Y, FSIZE_HM * 5 / 2 + SPACE_MS + FSIZE_S, FSIZE_HM)
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
    DrawFormatStringToHandle(x1, y1, BLACK, hFont_d, "%d年%d月%d日(%s)", Gety(), Getm(), Getd(), week_name[Getw()]);
}

void Time::Draw()
{
    int hour = Geth();

    // 0-12表示の時
    if (config.h_form == Form12h) {
        // 時間を0～12に
        if (Getn())
            hour -= HOUR / 2;

        // AM/PMを表示
        DrawFormatStringToHandle(x1 + FSIZE_HM * 5 / 2 + SPACE_MS, y1 + 10, BLACK, hFont_ap, Getn() ? "PM" : "AM");
    }

    // 時、分を表示
    DrawFormatStringToHandle(x1, y1, BLACK, hFont_hm, "%2d:%02d", hour, Getm());
    // 秒を表示
    DrawFormatStringToHandle(x1 + FSIZE_HM * 5 / 2 + SPACE_MS, y1 + FSIZE_HM - FSIZE_S - 15, BLACK, hFont_s, "%02d", Gets());
}

void Date::Update()
{
    // 表示幅の更新
    SetWidth(GetDrawFormatStringWidthToHandle(hFont_d, "%d年%d月%d日(%s)", Gety(), Getm(), Getd(), week_name[Getw()]));

    const int year_offset = 2014;   // 音声は2014年から

    // 現在の日付の音声を再生する
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
        // 現在時刻の音声を再生する
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

        // 時報を鳴らす
        else if (time_signal_flag && !pVoice->IsPlaying()) {
            if (pVoice->GetCfg().auto_repro) {
                pVoice->SoundTime(TimeSignal, Geth());
                speaking = TimeSignal;
            }
            time_signal_flag = false;
        }
    }

    // ○時ちょうどになったとき時報を鳴らす
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
