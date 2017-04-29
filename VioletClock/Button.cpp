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
    // �J�[�\������ɂ���Ƃ��e��\��
    // ���̏ꏊ���N���b�N���Ȃ���ړ����Ă����ꍇ�͕\�����Ȃ�
    if ((!pre_lclick_status && IsMouseOver()) || IsClicking())
        DrawBox(x1, y1, x2, y2, VIOLET3, TRUE);

    // �J��Ԃ��{�^����\��
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
    // �J�[�\������ɂ���Ƃ��e��\��
    // ���̏ꏊ���N���b�N���Ȃ���ړ����Ă����ꍇ�͕\�����Ȃ�
    if (!pre_lclick_status && IsMouseOver())
        DrawBox(x1, y1, x2, y2, VIOLET3, TRUE);

    // �ݒ�{�^����\��
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
    // �J�[�\������ɂ���Ƃ��e��\��
    // ���̏ꏊ���N���b�N���Ȃ���ړ����Ă����ꍇ�͕\�����Ȃ�
    if ((!pre_lclick_status && IsMouseOver()) || IsClicking())
        DrawBox(x1, y1, x2, y2, VIOLET3, TRUE);

    // �~���[�g�{�^����\��
    if (pVoice->GetCfg().sound)
        DrawGraph(x1, y1, hSnd_img, TRUE);
    else
        DrawGraph(x1, y1, hMute_img, TRUE);
}

void MuteButton::Update()
{
    // �N���b�N���ꂽ���A�~���[�g/�~���[�g������؂�ւ�
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
    // �J�[�\������ɂ���Ƃ��e��\��
    // ���̏ꏊ���N���b�N���Ȃ���ړ����Ă����ꍇ�͕\�����Ȃ�
    if (!pre_lclick_status && IsMouseOver())
        DrawBox(x1, y1, x2, y2, VIOLET3, TRUE);

    // �A���[���{�^����\��
    if (pAlarm->GetCfg().is_alarm_valid) {
        DrawGraph(x1, y1, hBase, TRUE);         // �j�ȊO��\��
        // ���j��\��
        DrawRotaGraph2(x1 + 20, y1 + 23, 20, 23, 1.0, (pAlarm->GetCfg().alarm_h + pAlarm->GetCfg().alarm_m / 60.0) * PI / 6, hHand, TRUE);
        // ���j��\��
        DrawRotaGraph2(x1 + 20, y1 + 23, 20, 23, 1.0, (pAlarm->GetCfg().alarm_m) * PI / 30, hHand, TRUE);
    }
    else {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 70);   // �A���[���I�t�̂Ƃ��͔����߂��ĕ\��
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
    // �J�[�\������ɂ���Ƃ��e��\��
    // ���̏ꏊ���N���b�N���Ȃ���ړ����Ă����ꍇ�͕\�����Ȃ�
    if (!pre_lclick_status && IsMouseOver())
        DrawBox(x1, y1, x2, y2, VIOLET3, TRUE);

    // �^�C�}�[�{�^����\��
    DrawGraph(x1, y1, hImage, TRUE);
}

TmrStartButton::TmrStartButton(int hF) : Area::Area(TMRSTART_X, TMRSTART_Y, TMRSTART_W, FSIZE_MAIN + MARGIN * 2)
{
    hFont = hF;
}

void TmrStartButton::Draw()
{
    // �J�[�\������ɂ���Ƃ��e��\��
    // ���̏ꏊ���N���b�N���Ȃ���ړ����Ă����ꍇ�͕\�����Ȃ�
    if ((!pre_lclick_status && IsMouseOver()) || IsClicking())
        DrawBox(x1, y1, x2, y2, VIOLET2, TRUE);

    switch (pTimer->GetStatus()) {
    case Timer::Idle:
        DrawStringToHandle((x2 + x1 - FSIZE_MAIN * 4) / 2, y1 + MARGIN, "�X�^�[�g", BLACK, hFont);
        break;
    case Timer::Counting:
        DrawStringToHandle((x2 + x1 - FSIZE_MAIN * 4) / 2, y1 + MARGIN, "�ꎞ��~", BLACK, hFont);
        break;
    case Timer::Pause:
        DrawStringToHandle((x2 + x1 - FSIZE_MAIN * 2) / 2, y1 + MARGIN, "�ĊJ", BLACK, hFont);
        break;
    }
}

TmrResetButton::TmrResetButton(int hF) : Area::Area(TMRRESET_X, TMRRESET_Y, TMRRESET_W, FSIZE_MAIN + MARGIN * 2)
{
    hFont = hF;
}

void TmrResetButton::Draw()
{
    // �J�[�\������ɂ���Ƃ��e��\��
    // ���̏ꏊ���N���b�N���Ȃ���ړ����Ă����ꍇ�͕\�����Ȃ�
    // �^�C�}�[��Idle�X�e�[�^�X�̎��\�����Ȃ�
    if (((!pre_lclick_status && IsMouseOver()) || IsClicking()) && pTimer->GetStatus() != Timer::Idle)
        DrawBox(x1, y1, x2, y2, VIOLET2, TRUE);

    // �^�C�}�[��Idle�X�e�[�^�X�̎��͕����F���D�F��
    if (pTimer->GetStatus() == Timer::Idle)
        DrawStringToHandle((x2 + x1 - FSIZE_MAIN * 4) / 2, y1 + MARGIN, "���Z�b�g", GetColor(183, 172, 198), hFont);
    else
        DrawStringToHandle((x2 + x1 - FSIZE_MAIN * 4) / 2, y1 + MARGIN, "���Z�b�g", BLACK, hFont);
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
    // �J�[�\������ɂ���Ƃ��e��\��
    // ���̏ꏊ���N���b�N���Ȃ���ړ����Ă����ꍇ�͕\�����Ȃ�
    if ((!pre_lclick_status && IsMouseOver()) || IsClicking())
        DrawBox(x1, y1, x2, y2, VIOLET2, TRUE);

    DrawStringToHandle((x1 + x2) / 2 - SCALEX(14), y1 + SCALEY(3), "�ؑ�", BLACK, hFont);
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
    // �J�[�\������ɂ���Ƃ��e��\��
    // ���̏ꏊ���N���b�N���Ȃ���ړ����Ă����ꍇ�͕\�����Ȃ�
    if ((!pre_lclick_status && IsMouseOver()) || IsClicking())
        DrawBox(x1, y1, x2, y2, VIOLET3, TRUE);

    DrawStringToHandle((x2 + x1 - str_width) / 2, y1 + MARGIN, name.c_str(), BLACK, hFont);
}
