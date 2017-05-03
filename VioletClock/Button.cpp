#include "DxLib.h"
#include "Button.h"
#include "Voice.h"
#include "Alarm.h"
#include "Timer.h"
#include "Exception.h"

int RptButton::hImage = -1;
int CfgButton::hImage = -1;
int MuteButton::hMute_img = -1;
int MuteButton::hSnd_img = -1;
int AlmButton::hBase = -1;
int AlmButton::hHand = -1;
int TmrButton::hImage = -1;

RptButton::RptButton() : Area(RPT_X, RPT_Y, RPT_W, RPT_H)
{
    if (hImage == -1)
        switch (SCALEY(480)){
        case 480:   hImage = LoadGraph("image\\96dpi\\repeat_button.png");      break;
        case 600:   hImage = LoadGraph("image\\120dpi\\repeat_button.png");     break;
        case 720:   hImage = LoadGraph("image\\144dpi\\repeat_button.png");     break;
        default:    throw Exception(Exception::DPIError, "repeat_button.png");  break;
        }
}

void RptButton::Draw()
{
    // カーソルが上にあるとき影を表示
    // 他の場所をクリックしながら移動してきた場合は表示しない
    if ((!pre_lclick_status && IsMouseOver()) || IsClicking())
        DrawBox(x1, y1, x2, y2, VIOLET3, TRUE);

    // 繰り返しボタンを表示
    DrawGraph(x1, y1, hImage, TRUE);
}

void RptButton::Update()
{
    if (!pVoice->IsPlaying() && IsClicked())
        pVoice->Repeat();
}

CfgButton::CfgButton() : Area(CFG_X, CFG_Y, CFG_W, CFG_H)
{
    if (hImage == -1)
        switch (SCALEY(480)){
        case 480:   hImage = LoadGraph("image\\96dpi\\config_button.png");      break;
        case 600:   hImage = LoadGraph("image\\120dpi\\config_button.png");     break;
        case 720:   hImage = LoadGraph("image\\144dpi\\config_button.png");     break;
        default:    throw Exception(Exception::DPIError, "config_button.png");  break;
        }
}

void CfgButton::Draw()
{
    // カーソルが上にあるとき影を表示
    // 他の場所をクリックしながら移動してきた場合は表示しない
    if (!pre_lclick_status && IsMouseOver())
        DrawBox(x1, y1, x2, y2, VIOLET3, TRUE);

    // 設定ボタンを表示
    DrawGraph(x1, y1, hImage, TRUE);
}

MuteButton::MuteButton() : Area(MUTE_X, MUTE_Y, MUTE_W, MUTE_H)
{
    if (hSnd_img == -1)
        switch (SCALEY(480)){
        case 480:   hSnd_img = LoadGraph("image\\96dpi\\sound_icon.png");   break;
        case 600:   hSnd_img = LoadGraph("image\\120dpi\\sound_icon.png");  break;
        case 720:   hSnd_img = LoadGraph("image\\144dpi\\sound_icon.png");  break;
        default:    throw Exception(Exception::DPIError, "sound_icon.png"); break;
        }
    if (hMute_img == -1)
        switch (SCALEY(480)){
        case 480:   hMute_img = LoadGraph("image\\96dpi\\mute_icon.png");   break;
        case 600:   hMute_img = LoadGraph("image\\120dpi\\mute_icon.png");  break;
        case 720:   hMute_img = LoadGraph("image\\144dpi\\mute_icon.png");  break;
        default:    throw Exception(Exception::DPIError, "mute_icon.png");  break;
        }
}

void MuteButton::Draw()
{
    // カーソルが上にあるとき影を表示
    // 他の場所をクリックしながら移動してきた場合は表示しない
    if ((!pre_lclick_status && IsMouseOver()) || IsClicking())
        DrawBox(x1, y1, x2, y2, VIOLET3, TRUE);

    // ミュートボタンを表示
    if (pVoice->GetCfg().sound)
        DrawGraph(x1, y1, hSnd_img, TRUE);
    else
        DrawGraph(x1, y1, hMute_img, TRUE);
}

void MuteButton::Update()
{
    // クリックされた時、ミュート/ミュート解除を切り替え
    if (IsClicked()) {
        if (pVoice->GetCfg().sound) {
            pVoice->StopSound();
            pVoice->SetCfg(Voice::Sound, false);
        }
        else
            pVoice->SetCfg(Voice::Sound, true);
    }
}

AlmButton::AlmButton() : Area(ALM_X, ALM_Y, ALM_W, ALM_H)
{
    if (hBase == -1)
        switch (SCALEY(480)){
        case 480:   hBase = LoadGraph("image\\96dpi\\clock_base.png");      break;
        case 600:   hBase = LoadGraph("image\\120dpi\\clock_base.png");     break;
        case 720:   hBase = LoadGraph("image\\144dpi\\clock_base.png");     break;
        default:    throw Exception(Exception::DPIError, "clock_base.png"); break;
        }
    if (hHand == -1)
        switch (SCALEY(480)){
        case 480:   hHand = LoadGraph("image\\96dpi\\clock_hand.png");      break;
        case 600:   hHand = LoadGraph("image\\120dpi\\clock_hand.png");     break;
        case 720:   hHand = LoadGraph("image\\144dpi\\clock_hand.png");     break;
        default:    throw Exception(Exception::DPIError, "clock_hand.png"); break;
        }
}

void AlmButton::Draw()
{
    // カーソルが上にあるとき影を表示
    // 他の場所をクリックしながら移動してきた場合は表示しない
    if (!pre_lclick_status && IsMouseOver())
        DrawBox(x1, y1, x2, y2, VIOLET3, TRUE);

    // アラームボタンを表示
    if (pAlarm->GetCfg().is_alarm_valid) {
        DrawGraph(x1, y1, hBase, TRUE);         // 針以外を表示
        // 時針を表示
        DrawRotaGraph2(x1 + SCALEX(20), y1 + SCALEY(23), SCALEX(20), SCALEY(23), 1.0,
            (pAlarm->GetCfg().alarm_h + pAlarm->GetCfg().alarm_m / 60.0) * PI / 6, hHand, TRUE);
        // 分針を表示
        DrawRotaGraph2(x1 + SCALEX(20), y1 + SCALEY(23), SCALEX(20), SCALEY(23), 1.0, 
            (pAlarm->GetCfg().alarm_m) * PI / 30, hHand, TRUE);
    }
    else {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 70);   // アラームオフのときは半透過して表示
        DrawGraph(x1, y1, hBase, TRUE);
        DrawRotaGraph2(x1 + SCALEX(20), y1 + SCALEY(23), SCALEX(20), SCALEY(23), 1.0,
            (pAlarm->GetCfg().alarm_h + pAlarm->GetCfg().alarm_m / 60.0) * PI / 6, hHand, TRUE);
        DrawRotaGraph2(x1 + SCALEX(20), y1 + SCALEY(23), SCALEX(20), SCALEY(23), 1.0,
            (pAlarm->GetCfg().alarm_m) * PI / 30, hHand, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}

TmrButton::TmrButton() : Area(TMR_X, TMR_Y, TMR_W, TMR_H)
{
    if (hImage == -1)
        switch (SCALEY(480)){
        case 480:   hImage = LoadGraph("image\\96dpi\\timer_button.png");       break;
        case 600:   hImage = LoadGraph("image\\120dpi\\timer_button.png");      break;
        case 720:   hImage = LoadGraph("image\\144dpi\\timer_button.png");      break;
        default:    throw Exception(Exception::DPIError, "timer_button.png");   break;
        }
}

void TmrButton::Draw()
{
    // カーソルが上にあるとき影を表示
    // 他の場所をクリックしながら移動してきた場合は表示しない
    if (!pre_lclick_status && IsMouseOver())
        DrawBox(x1, y1, x2, y2, VIOLET3, TRUE);

    // タイマーボタンを表示
    DrawGraph(x1, y1, hImage, TRUE);
}

TmrStartButton::TmrStartButton(int hF) : Area::Area(TMRSTART_X, TMRSTART_Y, TMRSTART_W, FSIZE_MAIN + MARGIN * 2)
{
    hFont = hF;
}

void TmrStartButton::Draw()
{
    // カーソルが上にあるとき影を表示
    // 他の場所をクリックしながら移動してきた場合は表示しない
    if ((!pre_lclick_status && IsMouseOver()) || IsClicking())
        DrawBox(x1, y1, x2, y2, VIOLET2, TRUE);

    switch (pTimer->GetStatus()) {
    case Timer::Idle:
        DrawStringToHandle((x2 + x1 - FSIZE_MAIN * 4) / 2, y1 + MARGIN, "スタート", BLACK, hFont);
        break;
    case Timer::Counting:
        DrawStringToHandle((x2 + x1 - FSIZE_MAIN * 4) / 2, y1 + MARGIN, "一時停止", BLACK, hFont);
        break;
    case Timer::Pause:
        DrawStringToHandle((x2 + x1 - FSIZE_MAIN * 2) / 2, y1 + MARGIN, "再開", BLACK, hFont);
        break;
    }
}

TmrResetButton::TmrResetButton(int hF) : Area::Area(TMRRESET_X, TMRRESET_Y, TMRRESET_W, FSIZE_MAIN + MARGIN * 2)
{
    hFont = hF;
}

void TmrResetButton::Draw()
{
    // カーソルが上にあるとき影を表示
    // 他の場所をクリックしながら移動してきた場合は表示しない
    // タイマーがIdleステータスの時表示しない
    if (((!pre_lclick_status && IsMouseOver()) || IsClicking()) && pTimer->GetStatus() != Timer::Idle)
        DrawBox(x1, y1, x2, y2, VIOLET2, TRUE);

    // タイマーがIdleステータスの時は文字色を灰色に
    if (pTimer->GetStatus() == Timer::Idle)
        DrawStringToHandle((x2 + x1 - FSIZE_MAIN * 4) / 2, y1 + MARGIN, "リセット", GetColor(183, 172, 198), hFont);
    else
        DrawStringToHandle((x2 + x1 - FSIZE_MAIN * 4) / 2, y1 + MARGIN, "リセット", BLACK, hFont);
}

TmrVoiceChgButton::TmrVoiceChgButton() : Area::Area(TMRCHANGE_X, TMRCHANGE_Y, TMRCHANGE_W, TMRCHANGE_H)
{
    hFont = CreateFontToHandle(NULL, SCALEY(14), 1, DX_FONTTYPE_ANTIALIASING_4X4);
}

TmrVoiceChgButton::~TmrVoiceChgButton()
{
    DeleteFontToHandle(hFont);
}

void TmrVoiceChgButton::Draw()
{
    // カーソルが上にあるとき影を表示
    // 他の場所をクリックしながら移動してきた場合は表示しない
    if ((!pre_lclick_status && IsMouseOver()) || IsClicking())
        DrawBox(x1, y1, x2, y2, VIOLET2, TRUE);

    DrawStringToHandle((x1 + x2) / 2 - SCALEX(14), y1 + SCALEY(3), "切替", BLACK, hFont);
}

Button::Button(int x, int y, int w, string str, int hF) : Area::Area(x, y, w, 0)
{
    int fsize;

    GetFontStateToHandle(NULL, &fsize, NULL, hF);
    SetHeight(fsize + MARGIN * 2);

    name = str;
    str_width = GetDrawStringWidthToHandle(str.c_str(), str.length(), hF);
    hFont = hF;
}

void Button::Draw()
{
    // カーソルが上にあるとき影を表示
    // 他の場所をクリックしながら移動してきた場合は表示しない
    if ((!pre_lclick_status && IsMouseOver()) || IsClicking())
        DrawBox(x1, y1, x2, y2, VIOLET3, TRUE);

    DrawStringToHandle((x2 + x1 - str_width) / 2, y1 + MARGIN, name.c_str(), BLACK, hFont);
}
