#include "DxLib.h"
#include "Config.h"
#include "Voice.h"

extern std::ofstream ferr;
extern const array<char *, WEEK> week_name;

int hFont_main;         // 設定画面で使う主要フォント
int hFont_choice;       // 選択肢を表示するフォント
int hFont_d;            // 日付表示用フォント
int hFont_hm;           // 時、分用フォント
int hFont_bd;           // 誕生日用フォント

CfgData::CfgData()
{
    // 現在の設定をコピー
    vcfg = pVoice->GetCfg();
    dcfg = pDate->GetCfg();
    tcfg = pTime->GetCfg();
}

void CfgData::Default()
{
    vcfg.auto_repro = true;
    vcfg.for_prg = false;
    dcfg.sound_year = true;
    dcfg.sound_week = true;
    dcfg.tail = Tail_None;
    tcfg.h_form = Form24h;
    tcfg.tail = Tail_None;
}

void CfgData::DefaultBd()
{
    dcfg.birthmonth = 12;
    dcfg.birthday = 19;
}

template <class DataType>
Item<DataType>::Item(int x0, int y0, int w, int h, string str, DataType &ref) : Area(x0 + FSIZE_MAIN / 2, y0 + FSIZE_MAIN / 2, w, h), data(ref)
{
    x = x0;
    y = y0;
    name = str;
}

template <class DataType>
Item<DataType>::~Item()
{
    for (int i = 0; i < (int)pChoice.size(); i++)
        delete pChoice[i];
}

template <class DataType>
void Item<DataType>::PushBack(Choice<DataType> *pCh)
{
    pChoice.push_back(pCh);
}

template <class DataType>
void Item<DataType>::Update()
{
    for (int i = 0; i < (int)pChoice.size(); i++)
        if (pChoice[i]->IsClicked())
            pChoice[i]->Set(data);
}

template <class DataType>
void Item<DataType>::Draw()
{
    // 項目を囲む枠を表示
    DrawBox(x1, y1, x2, y2, 0x80a0ff, FALSE);

    // 項目名の背景
    DrawBox(x, y, x + GetDrawStringWidthToHandle(name.c_str(), name.length(), hFont_main), y + FSIZE_MAIN, VIOLET1, TRUE);

    // 設定項目名を表示
    DrawStringToHandle(x, y, name.c_str(), BLACK, hFont_main);

    // 各選択肢を表示
    for (int i = 0; i < (int)pChoice.size(); i++)
        pChoice[i]->Draw(data);
}

template <class DataType>
Choice<DataType>::Choice(int x, int y, int w, string str, DataType val) :
    Area::Area(x, y, w, FSIZE_CHOICE + MARGIN * 2)
{
    name = str;
    str_width = GetDrawStringWidthToHandle(str.c_str(), str.length(), hFont_choice);
    value = val;
}

template <class DataType>
void Choice<DataType>::Set(DataType &data)
{
    data = value;
}

template <class DataType>
void Choice<DataType>::Draw(DataType &data)
{
    if (data == value)
        DrawBox(x1, y1, x2, y2, VIOLET2, TRUE);     // 選択されていることを示す影を表示

                                                    // カーソルが上にあるとき影を表示
                                                    // 他の場所をクリックしながら移動してきた場合は表示しない
    if ((!pre_lclick_status && IsMouseOver()) || IsClicking())
        DrawBox(x1, y1, x2, y2, VIOLET4, FALSE);

    DrawStringToHandle((x2 + x1 - str_width) / 2, y1 + MARGIN, name.c_str(), BLACK, hFont_choice);
}

ConfigTime::ConfigTime() : TimeSrc::TimeSrc()
{
    TimeSrc::SetCfg(pTimeSrc->GetCfg().offset);
    status = Invariable;
    changing = 0;
}

void ConfigTime::Update()
{
    TimeSrc::Update();      // 時刻を更新

                            // 日付がクリックされたとき
    if (Area(DATE_X, DATE_Y, FSIZE_D / 2 * (16 + (local.tm_mon >= 9) + (local.tm_mday >= 10)), FSIZE_D).IsClicked())
        switch (status) {
        case Invariable:
        case HVariable:
        case MinVariable:   status = YVariable;     break;
        case YVariable:     status = MonVariable;   break;
        case MonVariable:   status = DVariable;     break;
        case DVariable:     status = Invariable;    break;
        }
    // 時刻がクリックされたとき
    else if (Area(TIME_X, TIME_Y, FSIZE_HM * 5 / 2, FSIZE_HM).IsClicked())
        switch (status) {
        case Invariable:    status = HVariable;     break;
        case HVariable:     status = MinVariable;   break;
        case MinVariable:   status = Invariable;    break;
        case YVariable:
        case MonVariable:
        case DVariable:     status = HVariable;     break;
        }

    // changingが0でないときは30までカウント
    if (changing > 30)      changing = 0;
    else if (changing > 0)  changing++;

    int val = GetMouseWheelRotVol();
    if (!Area::pre_hitkey_status) {
        if (CheckHitKey(KEY_INPUT_UP))      val++;
        if (CheckHitKey(KEY_INPUT_DOWN))    val--;
    }
    if (status != Invariable && val != 0)
        changing = 1;

    // マウスホイール回転量に応じて日時を変更
    switch (status) {
    case Invariable:
        break;
    case HVariable:
        local.tm_hour += val;
        if (local.tm_hour < 0)          local.tm_hour += 24;
        else if (local.tm_hour >= 24)   local.tm_hour -= 24;
        break;
    case MinVariable:
        local.tm_min += val;
        if (local.tm_min < 0)           local.tm_min += 60;
        else if (local.tm_min >= 60)    local.tm_min -= 60;
        break;
    case YVariable:
        local.tm_year += val;
        if (local.tm_year < 70)     local.tm_year = 70;
        else if (local.tm_year > 1100)  local.tm_year = 1100;
        break;
    case MonVariable:
        local.tm_mon += val;
        if (local.tm_mon < 0)           local.tm_mon += 12;
        else if (local.tm_mon >= 12)    local.tm_mon -= 12;
        break;
    case DVariable:
        int day_max;        // 今月が何日まであるか
        switch (local.tm_mon + 1) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            day_max = 31;
            break;
        case 4: case 6: case 9: case 11:
            day_max = 30;
            break;
        case 2: {
            int year = local.tm_year + 1900;
            day_max = 28 + ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
            break; }
        default:
            day_max = 30;
            break;
        }

        local.tm_mday += val;
        if (local.tm_mday < 1)              local.tm_mday += day_max;
        else if (local.tm_mday > day_max)   local.tm_mday -= day_max;
    }

    time_t t = mktime(&local);
    if (t < 0)  t = 0;                  // 定義外にはみ出した場合は0sに設定
    TimeSrc::SetCfg(t - time(NULL));
}

void ConfigTime::Draw()
{
    static int cnt = 15;    // 初期位相は周期の1/4にしておく

                            // 日付を表示
    DrawFormatStringToHandle(DATE_X, DATE_Y, BLACK, hFont_d, "%4d年%d月%d日(%s)", local.tm_year + 1900, local.tm_mon + 1, local.tm_mday, week_name[local.tm_wday]);
    // 時、分を表示
    DrawFormatStringToHandle(TIME_X, TIME_Y, BLACK, hFont_hm, "%2d:%02d", local.tm_hour, local.tm_min);

    // 変更中は点滅させない
    if (changing == 0) {
        // 変更する部分を点滅させる
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(255 / (exp(4 * sin(cnt * PI / 30)) + 1)));
        switch (status) {
        case Invariable:
            cnt = 15;
            break;
        case HVariable:
            DrawBox(TIME_X, TIME_Y, TIME_X + FSIZE_HM, TIME_Y + FSIZE_HM, VIOLET1, TRUE);
            break;
        case MinVariable:
            DrawBox(TIME_X + FSIZE_HM * 3 / 2, TIME_Y, TIME_X + FSIZE_HM * 5 / 2, TIME_Y + FSIZE_HM, VIOLET1, TRUE);
            break;
        case YVariable:
            DrawBox(DATE_X, DATE_Y, DATE_X + FSIZE_D * 2, DATE_Y + FSIZE_D, VIOLET1, TRUE);
            break;
        case MonVariable:
            DrawBox(DATE_X + FSIZE_D * 3,
                DATE_Y,
                DATE_X + FSIZE_D / 2 * (7 + (local.tm_mon >= 9)),
                DATE_Y + FSIZE_D, VIOLET1, TRUE);
            break;
        case DVariable:
            DrawBox(DATE_X + FSIZE_D / 2 * (9 + (local.tm_mon >= 9)),
                DATE_Y,
                DATE_X + FSIZE_D / 2 * (10 + (local.tm_mon >= 9) + (local.tm_mday >= 10)),
                DATE_Y + FSIZE_D, VIOLET1, TRUE);
            break;
        }
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        cnt++;
    }
}

SetBirthday::SetBirthday(DateCfg &ref) : Area(BIRTHDAY_X, BIRTHDAY_Y, 0, FSIZE_BD), dcfg(ref)
{
    status = Invariable;
    changing = 0;
}

void SetBirthday::Update()
{
    SetWidth((7 + (dcfg.birthday >= 10)) * FSIZE_BD / 2);

    if (IsClicked()) {
        switch (status) {
        case Invariable:    status = MVariable;     break;
        case MVariable:     status = DVariable;     break;
        case DVariable:     status = Invariable;    break;
        }
    }

    if (changing > 30)      changing = 0;
    else if (changing > 0)  changing++;

    int val = 0;
    switch (status) {
    case Invariable:
        GetMouseWheelRotVol();
        break;
    case MVariable:
        val = GetMouseWheelRotVol();
        if (!Area::pre_hitkey_status) {
            if (CheckHitKey(KEY_INPUT_UP))      val++;
            if (CheckHitKey(KEY_INPUT_DOWN))    val--;
        }
        if (val != 0)       changing = 1;

        dcfg.birthmonth += val;
        if (dcfg.birthmonth < 1)        dcfg.birthmonth += 12;
        else if (dcfg.birthmonth > 12)  dcfg.birthmonth -= 12;
        break;
    case DVariable:
        val = GetMouseWheelRotVol();
        if (!Area::pre_hitkey_status) {
            if (CheckHitKey(KEY_INPUT_UP))      val++;
            if (CheckHitKey(KEY_INPUT_DOWN))    val--;
        }
        if (val != 0)       changing = 1;

        int day_max;        // 誕生月が何日まであるか
        switch (dcfg.birthmonth) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            day_max = 31;
            break;
        case 4: case 6: case 9: case 11:
            day_max = 30;
            break;
        case 2:
            day_max = 29;
            break;
        default:
            day_max = 30;
            break;
        }

        dcfg.birthday += val;
        if (dcfg.birthday < 1)              dcfg.birthday += day_max;
        else if (dcfg.birthday > day_max)   dcfg.birthday -= day_max;
        break;
    }
}

void SetBirthday::Draw()
{
    static int cnt;     // 初期位相は周期の1/4にしておく

    DrawFormatStringToHandle(x1, y1, BLACK, hFont_bd, "%2d月%d日", dcfg.birthmonth, dcfg.birthday);
    // 変更中は点滅させない
    if (changing == 0) {
        // 変更する部分を点滅させる
        switch (status) {
        case Invariable:
            cnt = 15;
            break;
        case MVariable:
            cnt++;
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(255 / (exp(4 * sin(cnt * PI / 30)) + 1)));
            DrawBox(x1, y1, x1 + FSIZE_BD, y2, VIOLET1, TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            break;
        case DVariable:
            cnt++;
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(255 / (exp(4 * sin(cnt * PI / 30)) + 1)));
            DrawBox(x1 + FSIZE_BD * 2, y1, x2 - FSIZE_BD, y2, VIOLET1, TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            break;
        }
    }
}

FontMaker::FontMaker()
{
    hFont_main = CreateFontToHandle(NULL, FSIZE_MAIN, 5, DX_FONTTYPE_ANTIALIASING_4X4);
    hFont_choice = CreateFontToHandle(NULL, FSIZE_CHOICE, 3, DX_FONTTYPE_ANTIALIASING_4X4);
    hFont_d = CreateFontToHandle(NULL, FSIZE_D, -1, DX_FONTTYPE_ANTIALIASING_4X4);
    hFont_hm = CreateFontToHandle(NULL, FSIZE_HM, -1, DX_FONTTYPE_ANTIALIASING_4X4);
    hFont_bd = CreateFontToHandle(NULL, FSIZE_BD, -1, DX_FONTTYPE_ANTIALIASING_4X4);
}

Config::Config(ISceneChanger *changer) : BaseScene(changer), tab(Tab_MainCfg), set_bd(temp.dcfg),
h_form(HFORM_X, HFORM_Y, 280, 65, "時刻表示形式", temp.tcfg.h_form),
auto_repro(AUTOREPRO_X, AUTOREPRO_Y, 280, 65, "自動再生", temp.vcfg.auto_repro),
sound_year(SOUNDYEAR_X, SOUNDYEAR_Y, 280, 65, "年読み上げ", temp.dcfg.sound_year),
sound_week(SOUNDWEEK_X, SOUNDWEEK_Y, 280, 65, "曜日読み上げ", temp.dcfg.sound_week),
tail_date(TAILDATE_X, TAILDATE_Y, 280, 130, "語尾(日付)", temp.dcfg.tail),
tail_time(TAILTIME_X, TAILTIME_Y, 280, 130, "語尾(時刻)", temp.tcfg.tail),
for_prg(FORPRG_X, FORPRG_Y, 280, 57, "プログラマ用", temp.vcfg.for_prg),

reset(RESET_X, RESET_Y, RESET_W, "既定値に戻す", hFont_main),
complete(COMP_X, COMP_Y, COMP_W, "完了", hFont_main),
cancel(CANCEL_X, CANCEL_Y, CANCEL_W, "キャンセル", hFont_main)
{
}

Config::~Config()
{
    DeleteFontToHandle(hFont_main);
    DeleteFontToHandle(hFont_choice);
    DeleteFontToHandle(hFont_d);
    DeleteFontToHandle(hFont_hm);
    DeleteFontToHandle(hFont_bd);
}

void Config::Initialize()
{
    pVoice->SoundSys(2);                        // 設定画面へ移るときの音声を再生

                                                // 各項目に選択肢を登録
    h_form.PushBack(new Choice<eHourForm>(HFORM_X + 30, HFORM_Y + 26, 100, "0-24 表示", Form24h));
    h_form.PushBack(new Choice<eHourForm>(HFORM_X + 170, HFORM_Y + 26, 100, "0-12 表示", Form12h));

    auto_repro.PushBack(new Choice<bool>(AUTOREPRO_X + 30, AUTOREPRO_Y + 26, 100, "ON", true));
    auto_repro.PushBack(new Choice<bool>(AUTOREPRO_X + 170, AUTOREPRO_Y + 26, 100, "OFF", false));

    sound_year.PushBack(new Choice<bool>(SOUNDYEAR_X + 30, SOUNDYEAR_Y + 26, 100, "ON", true));
    sound_year.PushBack(new Choice<bool>(SOUNDYEAR_X + 170, SOUNDYEAR_Y + 26, 100, "OFF", false));

    sound_week.PushBack(new Choice<bool>(SOUNDWEEK_X + 30, SOUNDWEEK_Y + 26, 100, "ON", true));
    sound_week.PushBack(new Choice<bool>(SOUNDWEEK_X + 170, SOUNDWEEK_Y + 26, 100, "OFF", false));

    tail_date.PushBack(new Choice<eTail>(TAILDATE_X + 30, TAILDATE_Y + 25, 100, "…だね！", Tail_Dane));
    tail_date.PushBack(new Choice<eTail>(TAILDATE_X + 30, TAILDATE_Y + 62, 100, "…だよ！", Tail_Dayo));
    tail_date.PushBack(new Choice<eTail>(TAILDATE_X + 155, TAILDATE_Y + 25, 130, "…になったよ！", Tail_Ninattayo));
    tail_date.PushBack(new Choice<eTail>(TAILDATE_X + 155, TAILDATE_Y + 62, 130, "…だって！", Tail_Datte));
    tail_date.PushBack(new Choice<eTail>(TAILDATE_X + 30, TAILDATE_Y + 99, 100, "なし", Tail_None));

    tail_time.PushBack(new Choice<eTail>(TAILTIME_X + 30, TAILTIME_Y + 25, 100, "…だね！", Tail_Dane));
    tail_time.PushBack(new Choice<eTail>(TAILTIME_X + 30, TAILTIME_Y + 62, 100, "…だよ！", Tail_Dayo));
    tail_time.PushBack(new Choice<eTail>(TAILTIME_X + 155, TAILTIME_Y + 25, 130, "…になったよ！", Tail_Ninattayo));
    tail_time.PushBack(new Choice<eTail>(TAILTIME_X + 155, TAILTIME_Y + 62, 130, "…だって！", Tail_Datte));
    tail_time.PushBack(new Choice<eTail>(TAILTIME_X + 30, TAILTIME_Y + 99, 100, "なし", Tail_None));

    for_prg.PushBack(new Choice<bool>(FORPRG_X + 30, FORPRG_Y + 25, 100, "ON", true));
    for_prg.PushBack(new Choice<bool>(FORPRG_X + 170, FORPRG_Y + 25, 100, "OFF", false));
}

void Config::Update()
{
    switch (tab) {
    case Tab_MainCfg:
        h_form.Update();
        auto_repro.Update();
        sound_year.Update();
        sound_week.Update();
        tail_date.Update();
        tail_time.Update();
        for_prg.Update();

        // タブ切り替え
        if (Area(77, 7, 189 - 77, MAINAREA_Y - 7).IsClicked())
            tab = Tab_TimeAdj;
        else if (Area(189, 7, 321 - 189, MAINAREA_Y - 7).IsClicked())
            tab = Tab_SetBirthday;
        break;
    case Tab_TimeAdj:
        cfg_time.Update();

        // タブ切り替え
        if (Area(189, 7, 321 - 189, MAINAREA_Y - 7).IsClicked()) {
            cfg_time.ResetStatus();
            tab = Tab_SetBirthday;
        }
        else if (Area(MAINAREA_X, 7, 77 - MAINAREA_X, MAINAREA_Y - 7).IsClicked()) {
            cfg_time.ResetStatus();
            tab = Tab_MainCfg;
        }
        break;
    case Tab_SetBirthday:
        set_bd.Update();

        // タブ切り替え
        if (Area(MAINAREA_X, 7, 77 - MAINAREA_X, MAINAREA_Y - 7).IsClicked()) {
            set_bd.ResetStatus();
            tab = Tab_MainCfg;
        }
        else if (Area(77, 7, 189 - 77, MAINAREA_Y - 7).IsClicked()) {
            set_bd.ResetStatus();
            tab = Tab_TimeAdj;
        }
    }

    // 既定値に戻すボタンが押されたとき
    if (reset.IsClicked())
        switch (tab) {
        case Tab_MainCfg:
            temp.Default();                         // 設定をデフォルトに戻す
            break;
        case Tab_TimeAdj:
            cfg_time.SetCfg(0);                     // 時差をゼロに戻す
            break;
        case Tab_SetBirthday:
            temp.DefaultBd();                       // 設定をデフォルトに戻す
            break;
        }
    // 完了ボタンが押されたとき
    else if (complete.IsClicked()) {
        pVoice->StopSound();
        pVoice->SoundSys(3);
        Reflect();                                  // 設定を反映
        pScene_changer->ChangeScene(eScene_Clock);  // シーンをメイン画面に変更
    }
    // キャンセルボタンが押されたとき
    else if (cancel.IsClicked())
        pScene_changer->ChangeScene(eScene_Clock);  // シーンをメイン画面に変更
}

void Config::Draw()
{
    // 背景を描画
    DrawBox(MAINAREA_X, 7, 321, MAINAREA_Y, 0xC4A9E8, TRUE);
    DrawLine(77, 7, 77, MAINAREA_Y, 0xD9BEF8);     // 他の色候補VIOLET1 0xD3BEED 0x82709a
    DrawLine(189, 7, 189, MAINAREA_Y, 0xD9BEF8);
    DrawStringToHandle(20, 14, "設定", GetColor(50, 50, 50), hFont_main);
    DrawStringToHandle(93, 14, "時刻設定", GetColor(50, 50, 50), hFont_main);
    DrawStringToHandle(205, 14, "誕生日設定", GetColor(50, 50, 50), hFont_main);
    DrawBox(MAINAREA_X, MAINAREA_Y, MAINAREA_X + MAINAREA_W, MAINAREA_Y + MAINAREA_H, VIOLET1, TRUE);

    switch (tab) {
    case Tab_MainCfg:
        // 背景を描画
        DrawBox(MAINAREA_X, 5, 78, MAINAREA_Y, VIOLET1, TRUE);
        DrawStringToHandle(20, 13, "設定", BLACK, hFont_main);

        h_form.Draw();
        auto_repro.Draw();
        sound_year.Draw();
        sound_week.Draw();
        tail_date.Draw();
        tail_time.Draw();
        for_prg.Draw();
        break;
    case Tab_TimeAdj:
        // 背景を描画
        DrawBox(77, 5, 190, MAINAREA_Y, VIOLET1, TRUE);
        DrawStringToHandle(93, 13, "時刻設定", BLACK, hFont_main);

        cfg_time.Draw();
        break;
    case Tab_SetBirthday:
        // 背景を描画
        DrawBox(189, 5, 322, MAINAREA_Y, VIOLET1, TRUE);
        DrawStringToHandle(205, 13, "誕生日設定", BLACK, hFont_main);

        set_bd.Draw();
        break;
    }

    reset.Draw();
    complete.Draw();
    cancel.Draw();
}

void Config::Reflect()
{
    pVoice->SetCfg(Voice::AutoRepro, temp.vcfg.auto_repro);
    pVoice->SetCfg(Voice::ForPrg, temp.vcfg.for_prg);
    pDate->SetCfg(Date::SoundYear, temp.dcfg.sound_year);
    pDate->SetCfg(Date::SoundWeek, temp.dcfg.sound_week);
    pDate->SetCfg(Date::TailDate, temp.dcfg.tail);
    pDate->SetCfg(Date::BirthMonth, temp.dcfg.birthmonth);
    pDate->SetCfg(Date::BirthDay, temp.dcfg.birthday);
    pTime->SetCfg(Time::HForm, temp.tcfg.h_form);
    pTime->SetCfg(Time::TailTime, temp.tcfg.tail);
    pTimeSrc->SetCfg(cfg_time.GetCfg().offset);
}
