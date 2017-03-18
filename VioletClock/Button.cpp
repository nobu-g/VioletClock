#include "DxLib.h"
#include "Button.h"
#include "Voice.h"
#include "Alarm.h"
#include "Timer.h"

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
        hImage = LoadGraph("image\\repeat_button.png");
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
        hImage = LoadGraph("image\\config_button.png");
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
        hSnd_img = LoadGraph("image\\sound_icon.png");
    if (hMute_img == -1)
        hMute_img = LoadGraph("image\\mute_icon.png");
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
        hBase = LoadGraph("image\\clock_base.png");
    if (hHand == -1)
        hHand = LoadGraph("image\\clock_hand.png");
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
        DrawRotaGraph2(x1 + 20, y1 + 23, 20, 23, 1.0, (pAlarm->GetCfg().alarm_h + pAlarm->GetCfg().alarm_m / 60.0) * PI / 6, hHand, TRUE);
        // 分針を表示
        DrawRotaGraph2(x1 + 20, y1 + 23, 20, 23, 1.0, (pAlarm->GetCfg().alarm_m) * PI / 30, hHand, TRUE);
    }
    else {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 70);   // アラームオフのときは半透過して表示
        DrawGraph(x1, y1, hBase, TRUE);
        DrawRotaGraph2(x1 + 20, y1 + 23, 20, 23, 1.0, (pAlarm->GetCfg().alarm_h + pAlarm->GetCfg().alarm_m / 60.0) * PI / 6, hHand, TRUE);
        DrawRotaGraph2(x1 + 20, y1 + 23, 20, 23, 1.0, (pAlarm->GetCfg().alarm_m) * PI / 30, hHand, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}

TmrButton::TmrButton() : Area(TMR_X, TMR_Y, TMR_W, TMR_H)
{
    if (hImage == -1)
        hImage = LoadGraph("image\\timer_button.png");
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
    hFont = CreateFontToHandle(NULL, 14, 1, DX_FONTTYPE_ANTIALIASING_4X4);
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

    DrawStringToHandle((x1 + x2) / 2 - 14, y1 + 3, "切替", BLACK, hFont);
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
