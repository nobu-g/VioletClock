#include "DxLib.h"
#include "Timer.h"
#include <ctime>

static int hFont_hms;
extern int hFont_main;
static int hFont_vselect;

TimerTime::TimerTime(TimerCfg &ref) : Area(TMRTIME_X, TMRTIME_Y, TMRTIME_W, TMRTIME_H)
{
    status = Invariable;
    changing = 0;
}

void TimerTime::Update()
{
    if (IsClicked() && pTimer->GetStatus() == Timer::Idle) {
        switch (status) {
        case Invariable:    status = HVariable;     break;
        case HVariable:     status = MVariable;     break;
        case MVariable:     status = SVariable;     break;
        case SVariable:     status = Invariable;    break;
        }
    }

    if (changing > 30)      changing = 0;
    else if (changing > 0)  changing++;

    int val = GetMouseWheelRotVol();
    if (!Area::pre_hitkey_status) {
        if (CheckHitKey(KEY_INPUT_UP))      val++;
        if (CheckHitKey(KEY_INPUT_DOWN))    val--;
    }
    if (status != Invariable && val != 0)
        changing = 1;

    TimerCfg temp = pTimer->GetCfg();
    switch (status) {
    case Invariable:
        break;
    case HVariable:
        temp.hour += val;
        if (temp.hour < 0)          temp.hour += 10;
        else if (temp.hour > 9)     temp.hour -= 10;
        pTimer->SetCfg(Timer::Hour, temp.hour);
        break;
    case MVariable:
        temp.minute += val;
        if (temp.minute < 1)        temp.minute += 60;
        else if (temp.minute > 59)  temp.minute -= 60;
        pTimer->SetCfg(Timer::Minute, temp.minute);
        break;
    case SVariable:
        temp.second += val;
        if (temp.second < 1)        temp.second += 60;
        else if (temp.second > 59)  temp.second -= 60;
        pTimer->SetCfg(Timer::Second, temp.second);
        break;
    }
}

void TimerTime::Draw()
{
    static int cnt;     // 初期位相は周期の1/4にしておく

    DrawFormatStringToHandle(x1, y1, BLACK, hFont_hms, "%d:%02d:%02d", pTimer->Geth(), pTimer->Getm(), pTimer->Gets());
    // 変更中は点滅させない
    if (changing == 0) {
        // 変更する部分を点滅させる
        switch (status) {
        case Invariable:
            cnt = 15;
            break;
        case HVariable:
            cnt++;
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(255 / (exp(4 * sin(cnt * PI / 30)) + 1)));
            DrawBox(x1, y1, x1 + STOW(FSIZE_HMS), y2, VIOLET1, TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            break;
        case MVariable:
            cnt++;
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(255 / (exp(4 * sin(cnt * PI / 30)) + 1)));
            DrawBox(x1 + STOW(FSIZE_HMS) * 2, y1, x1 + STOW(FSIZE_HMS) * 4, y2, VIOLET1, TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            break;
        case SVariable:
            cnt++;
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(255 / (exp(4 * sin(cnt * PI / 30)) + 1)));
            DrawBox(x1 + STOW(FSIZE_HMS) * 5, y1, x1 + STOW(FSIZE_HMS) * 7, y2, VIOLET1, TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            break;
        }
    }
}

VoiceSelect::VoiceSelect()
{
    words = {
        "しゅう～りょう～",
        "お・わ・り、だよ！",
        "おわりだよ！",
        "はい、しゅーりょー",
        "ラーメンできたよ！",
        "パスタ茹で上がるよ！",
        "お風呂沸いたよ～！",
        "ご飯炊けたよ～！"
    };
}

void VoiceSelect::Update()
{
    if (chg_btn.IsClicked()) {
        // 音声を変更する
        int val = pTimer->GetCfg().voice_index + 1;

        if (val >= OVER_VOICES)
            val -= OVER_VOICES;
        pTimer->SetCfg(Timer::Voice, val);
    }
}

void VoiceSelect::Draw()
{
    DrawFormatStringToHandle(SCALEX(300), SCALEY(390), BLACK, hFont_vselect, "終了時: 「%s」", words[pTimer->GetCfg().voice_index].c_str());

    chg_btn.Draw();
}

FontMakerTimer::FontMakerTimer()
{
    hFont_hms = CreateFontToHandle(NULL, FSIZE_HMS, -1, DX_FONTTYPE_ANTIALIASING_4X4);
    hFont_main = CreateFontToHandle(NULL, FSIZE_MAIN, 5, DX_FONTTYPE_ANTIALIASING_4X4);
    hFont_vselect = CreateFontToHandle(NULL, FSIZE_VSELECT, 1, DX_FONTTYPE_ANTIALIASING_4X4);
}

TimerScreen::TimerScreen(ISceneChanger * changer) : BaseScene(changer), FontMakerTimer(),
timer_time(pTimer->GetCfg()),
start_btn(hFont_main),
reset_btn(hFont_main),
back(BACK_X, BACK_Y, BACK_W, "戻る", hFont_main)
{
}

TimerScreen::~TimerScreen()
{
    DeleteFontToHandle(hFont_hms);
    DeleteFontToHandle(hFont_main);
    DeleteFontToHandle(hFont_vselect);
}

void TimerScreen::Update()
{
    timer_time.Update();
    vselect.Update();

    if (start_btn.IsClicked()) {
        switch (pTimer->GetStatus()) {
        case Timer::Idle:
            timer_time.ResetStatus();
            pTimer->Start();
            break;
        case Timer::Counting:
            pTimer->Stop();
            break;
        case Timer::Pause:
            pTimer->Restart();
            break;
        }
    }
    else if (pTimer->GetStatus() != Timer::Idle && reset_btn.IsClicked())
        pTimer->Reset();
    else if (back.IsClicked())
        pScene_changer->ChangeScene(eScene_Clock);
}

void TimerScreen::Draw()
{
    DrawBox(MAINAREA_X, SCALEY(5), SCALEX(117), MAINAREA_Y, VIOLET1, TRUE);
    DrawBox(MAINAREA_X, MAINAREA_Y, MAINAREA_X + MAINAREA_W, MAINAREA_Y + MAINAREA_H, VIOLET1, TRUE);
    DrawStringToHandle(SCALEX(20), SCALEY(13), "タイマー", BLACK, hFont_main);
        
    start_btn.Draw();
    reset_btn.Draw();
    vselect.Draw();
    back.Draw();
    timer_time.Draw();

    Chr::DrawBackImg();
}

Timer::Timer()
{
    config.hour = 0;
    config.minute = 0;
    config.second = 0;
    config.voice_index = 0;
    status = Idle;
}

void Timer::Start()
{
    base_ms = GetNowCount();
    status = Counting;
    pVoice->StopSound();
    if (GetRand(1) == 0)
        pVoice->SoundTimer(Voice::Start1);
    else
        pVoice->SoundTimer(Voice::Start2);
}

void Timer::Stop()
{
    stop_ms = GetNowCount();
    status = Pause;
}

void Timer::Restart()
{
    base_ms = GetNowCount() - stop_ms + base_ms;
    status = Counting;
}

void Timer::Reset()
{
    status = Idle;
}

void Timer::SoundCount()
{
    if (remaining_h == 0) {
        if (remaining_m == 1 && remaining_s == 0) {
            if (!pVoice->IsTimerPlaying()) {
                pVoice->StopSound();
                pVoice->SoundTimer(Voice::Last1m);
            }
        }
        else if (remaining_m == 0) {
            switch (remaining_s) {
            case 30:
                if (!pVoice->IsTimerPlaying()) {
                    pVoice->StopSound();
                    pVoice->SoundTimer(Voice::Last30);
                }
                break;
            case 1:
                if (!pVoice->IsTimerPlaying()) {
                    pVoice->StopSound();
                    pVoice->SoundTimer(Voice::Last1);
                }
                break;
            case 2:
                if (!pVoice->IsTimerPlaying()) {
                    pVoice->StopSound();
                    pVoice->SoundTimer(Voice::Last2);
                }
                break;
            case 3:
                if (!pVoice->IsTimerPlaying()) {
                    pVoice->StopSound();
                    pVoice->SoundTimer(Voice::Last3);
                }
                break;
            case 4:
                if (!pVoice->IsTimerPlaying()) {
                    pVoice->StopSound();
                    pVoice->SoundTimer(Voice::Last4);
                }
                break;
            case 5:
                if (!pVoice->IsTimerPlaying()) {
                    pVoice->StopSound();
                    pVoice->SoundTimer(Voice::Last5);
                }
                break;
            case 6:
                if (!pVoice->IsTimerPlaying()) {
                    pVoice->StopSound();
                    pVoice->SoundTimer(Voice::Last6);
                }
                break;
            case 7:
                if (!pVoice->IsTimerPlaying()) {
                    pVoice->StopSound();
                    pVoice->SoundTimer(Voice::Last7);
                }
                break;
            case 8:
                if (!pVoice->IsTimerPlaying()) {
                    pVoice->StopSound();
                    pVoice->SoundTimer(Voice::Last8);
                }
                break;
            case 9:
                if (!pVoice->IsTimerPlaying()) {
                    pVoice->StopSound();
                    pVoice->SoundTimer(Voice::Last9);
                }
                break;
            case 10:
                if (!pVoice->IsTimerPlaying()) {
                    pVoice->StopSound();
                    pVoice->SoundTimer(Voice::Last10);
                }
                break;
            }
        }
    }
}

void Timer::Update()
{
    switch (status) {
    case Idle:
        remaining_h = config.hour;
        remaining_m = config.minute;
        remaining_s = config.second;
        break;
    case Counting:
        // 1000ms経過したら残り時間1秒減らす
        if (GetNowCount() - base_ms > 1000) {
            base_ms += 1000;
            remaining_s--;
            if (remaining_s < 0) {
                remaining_s = 59;
                remaining_m--;
            }
            if (remaining_m < 0) {
                remaining_m = 59;
                remaining_h--;
            }
            // カウントダウンボイスを再生する
            SoundCount();
        }
        // カウントが0になったとき
        if (remaining_h == 0 && remaining_m == 0 && remaining_s == 0) {
            // 終了時音声を流す
            pVoice->StopSound();
            switch (config.voice_index) {
            case 0: pVoice->SoundTimer(Voice::Over1);   break;
            case 1: pVoice->SoundTimer(Voice::Over2);   break;
            case 2: pVoice->SoundTimer(Voice::Over3);   break;
            case 3: pVoice->SoundTimer(Voice::Over4);   break;
            case 4: pVoice->SoundTimer(Voice::Over5);   break;
            case 5: pVoice->SoundTimer(Voice::Over6);   break;
            case 6: pVoice->SoundTimer(Voice::Over7);   break;
            case 7: pVoice->SoundTimer(Voice::Over8);   break;
            default:    break;
            }
            status = Idle;
        }
        break;
    case Pause:
        break;
    }
}

void Timer::SetCfg(eCfg cfg, int value)
{
    switch (cfg) {
    case Hour:
        config.hour = value;
        break;
    case Minute:
        config.minute = value;
        break;
    case Second:
        config.second = value;
        break;
    case Voice:
        config.voice_index = value;
        break;
    }
    config.Check();
}
