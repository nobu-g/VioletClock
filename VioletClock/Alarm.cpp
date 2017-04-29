#include "DxLib.h"
#include "Alarm.h"
#include <cmath>

extern int hFont_main;
extern int hFont_choice;

AlarmTime::AlarmTime(AlarmCfg &ref) : Area(ALMTIME_X, ALMTIME_Y, FSIZE_T / 2 * 5, FSIZE_T), temp(ref)
{
    status = Invariable;
    changing = 0;
    hFont = CreateFontToHandle(NULL, FSIZE_T, -1, DX_FONTTYPE_ANTIALIASING_4X4);
}

AlarmTime::~AlarmTime()
{
    DeleteFontToHandle(hFont);
}

void AlarmTime::Update()
{
    if (IsClicked()) {
        switch (status) {
        case Invariable:    status = HVariable;     break;
        case HVariable:     status = MVariable;     break;
        case MVariable:     status = Invariable;    break;
        }
    }

    if (changing > 30)      changing = 0;
    else if (changing > 0)  changing++;

    int val = 0;
    switch (status) {
    case Invariable:
        GetMouseWheelRotVol();
        break;
    case HVariable:
        val = GetMouseWheelRotVol();
        if (!Area::pre_hitkey_status) {
            if (CheckHitKey(KEY_INPUT_UP))      val++;
            if (CheckHitKey(KEY_INPUT_DOWN))    val--;
        }
        if (val != 0)       changing = 1;

        temp.alarm_h += val;
        if (temp.alarm_h < 0)           temp.alarm_h += 24;
        else if (temp.alarm_h >= 24)    temp.alarm_h -= 24;
        break;
    case MVariable:
        val = GetMouseWheelRotVol();
        if (!Area::pre_hitkey_status) {
            if (CheckHitKey(KEY_INPUT_UP))      val++;
            if (CheckHitKey(KEY_INPUT_DOWN))    val--;
        }
        if (val != 0)       changing = 1;

        temp.alarm_m += val;
        if (temp.alarm_m < 0)           temp.alarm_m += 60;
        else if (temp.alarm_m >= 60)    temp.alarm_m -= 60;
        break;
    }
}

void AlarmTime::Draw()
{
    static int cnt;     // 初期位相は周期の1/4にしておく

    DrawFormatStringToHandle(x1, y1, BLACK, hFont, "%2d:%02d", temp.alarm_h, temp.alarm_m);
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
            DrawBox(x1, y1, x1 + FSIZE_T, y2, VIOLET1, TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            break;
        case MVariable:
            cnt++;
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(255 / (exp(4 * sin(cnt * PI / 30)) + 1)));
            DrawBox(x2 - FSIZE_T, y1, x2, y2, VIOLET1, TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            break;
        }
    }
}
FontMakerAlarm::FontMakerAlarm()
{
    hFont_main = CreateFontToHandle(NULL, FSIZE_MAIN, 5, DX_FONTTYPE_ANTIALIASING_4X4);
    hFont_choice = CreateFontToHandle(NULL, FSIZE_CHOICE, 3, DX_FONTTYPE_ANTIALIASING_4X4);
};

AlarmScreen::AlarmScreen(ISceneChanger *changer) : BaseScene(changer), FontMakerAlarm(),
temp(pAlarm->GetCfg()),
alarm_time(temp),
alarm_valid(ALARMVALID_X, ALARMVALID_Y, ALARMVALID_W, ALARMVALID_H, "アラーム", temp.is_alarm_valid),
mode(MODE_X, MODE_Y, MODE_W, MODE_H, "モード", temp.is_wakeup_mode),

complete(COMP_X, COMP_Y, COMP_W, "完了", hFont_main),
cancel(CANCEL_X, CANCEL_Y, CANCEL_W, "キャンセル", hFont_main)
{
}

AlarmScreen::~AlarmScreen()
{
    DeleteFontToHandle(hFont_main);
    DeleteFontToHandle(hFont_choice);
}

void AlarmScreen::Initialize()
{
    alarm_valid.PushBack(new Choice<bool>(ALARMVALID_X + SCALEX(30),  ALARMVALID_Y + SCALEY(26), SCALEX(100), "ON", true));
    alarm_valid.PushBack(new Choice<bool>(ALARMVALID_X + SCALEX(170), ALARMVALID_Y + SCALEY(26), SCALEX(100), "OFF", false));

    mode.PushBack(new Choice<bool>(MODE_X + SCALEX(30),  MODE_Y + SCALEY(26), SCALEX(100), "目覚まし", true));
    mode.PushBack(new Choice<bool>(MODE_X + SCALEX(170), MODE_Y + SCALEY(26), SCALEX(100), "ノーマル", false));
}

void AlarmScreen::Update()
{
    // 完了ボタンが押されたとき
    if (complete.IsClicked()) {
        pVoice->StopSound();
        pVoice->SoundSys(3);
        Reflect();                                  // 設定を反映
        pScene_changer->ChangeScene(eScene_Clock);  // シーンをメイン画面に変更
    }
    // キャンセルボタンが押されたとき
    else if (cancel.IsClicked())
        pScene_changer->ChangeScene(eScene_Clock);  // シーンをメイン画面に変更

    alarm_time.Update();

    alarm_valid.Update();
    mode.Update();
}

void AlarmScreen::Draw()
{
    DrawBox(MAINAREA_X, SCALEY(5), SCALEX(157), MAINAREA_Y, VIOLET1, TRUE);
    DrawBox(MAINAREA_X, MAINAREA_Y, MAINAREA_X + MAINAREA_W, MAINAREA_Y + MAINAREA_H, VIOLET1, TRUE);
    DrawStringToHandle(SCALEX(20), SCALEY(13), "アラーム設定", BLACK, hFont_main);

    alarm_time.Draw();

    complete.Draw();
    cancel.Draw();

    alarm_valid.Draw();
    mode.Draw();
}

void AlarmScreen::Reflect()
{
    pAlarm->SetCfg(Alarm::AlarmH, temp.alarm_h);
    pAlarm->SetCfg(Alarm::AlarmM, temp.alarm_m);
    pAlarm->SetCfg(Alarm::IsAlarmValid, temp.is_alarm_valid);
    pAlarm->SetCfg(Alarm::IsWakeupMode, temp.is_wakeup_mode);
}

Alarm::Alarm()
{
    is_ringing = false;
    config.alarm_h = 0;
    config.alarm_m = 0;
    config.is_alarm_valid = true;
    config.is_wakeup_mode = true;
}

void Alarm::Update()
{
    if (!is_ringing) {
        if (config.is_alarm_valid)
            if (pTime->Geth() == config.alarm_h && pTime->Getm() == config.alarm_m) {
                pVoice->StopSound();
                is_ringing = true;
            }
    }

    if (is_ringing) {
        if (config.is_wakeup_mode)
            pVoice->SoundWakeUp();      // 目覚ましアラームを鳴らす
        else
            pVoice->SoundAlarm();       // 通常アラームを鳴らす

        // 画面のどこかがクリックされるか、いずれかのキーが押されたとき
        if ((!Area::pre_lclick_status && (GetMouseInput() & MOUSE_INPUT_LEFT)) ||
            (!Area::pre_hitkey_status && CheckHitKeyAll())) {
            pVoice->StopSound();        // アラームを止める
            is_ringing = false;
            SetCfg(IsAlarmValid, false);
        }
    }
}

void Alarm::SetCfg(eCfg cfg, int value)
{
    switch (cfg) {
    case AlarmH:
        config.alarm_h = value;
        break;
    case AlarmM:
        config.alarm_m = value;
        break;
    case IsAlarmValid: config.is_alarm_valid = (value != 0); break;
    case IsWakeupMode: config.is_wakeup_mode = (value != 0); break;
    }
    config.Check();
}
