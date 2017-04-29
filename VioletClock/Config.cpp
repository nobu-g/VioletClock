#include "DxLib.h"
#include "Config.h"
#include "Voice.h"

extern std::ofstream ferr;
extern const array<char *, WEEK> week_name;

int hFont_main;         // �ݒ��ʂŎg����v�t�H���g
int hFont_choice;       // �I������\������t�H���g
int hFont_d;            // ���t�\���p�t�H���g
int hFont_hm;           // ���A���p�t�H���g
int hFont_bd;           // �a�����p�t�H���g

CfgData::CfgData()
{
    // ���݂̐ݒ���R�s�[
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
    // ���ڂ��͂ޘg��\��
    DrawBox(x1, y1, x2, y2, 0x80a0ff, FALSE);

    // ���ږ��̔w�i
    DrawBox(x, y, x + GetDrawStringWidthToHandle(name.c_str(), name.length(), hFont_main), y + FSIZE_MAIN, VIOLET1, TRUE);

    // �ݒ荀�ږ���\��
    DrawStringToHandle(x, y, name.c_str(), BLACK, hFont_main);

    // �e�I������\��
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
        DrawBox(x1, y1, x2, y2, VIOLET2, TRUE);     // �I������Ă��邱�Ƃ������e��\��

    // �J�[�\������ɂ���Ƃ��e��\��
    // ���̏ꏊ���N���b�N���Ȃ���ړ����Ă����ꍇ�͕\�����Ȃ�
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
    TimeSrc::Update();      // �������X�V

    // ���t���N���b�N���ꂽ�Ƃ�
    if (Area(DATE_X, DATE_Y, FSIZE_D / 2 * (16 + (local.tm_mon >= 9) + (local.tm_mday >= 10)), FSIZE_D).IsClicked())
        switch (status) {
        case Invariable:
        case HVariable:
        case MinVariable:   status = YVariable;     break;
        case YVariable:     status = MonVariable;   break;
        case MonVariable:   status = DVariable;     break;
        case DVariable:     status = Invariable;    break;
        }
    // �������N���b�N���ꂽ�Ƃ�
    else if (Area(TIME_X, TIME_Y, FSIZE_HM * 5 / 2, FSIZE_HM).IsClicked())
        switch (status) {
        case Invariable:    status = HVariable;     break;
        case HVariable:     status = MinVariable;   break;
        case MinVariable:   status = Invariable;    break;
        case YVariable:
        case MonVariable:
        case DVariable:     status = HVariable;     break;
        }

    // changing��0�łȂ��Ƃ���30�܂ŃJ�E���g
    if (changing > 30)      changing = 0;
    else if (changing > 0)  changing++;

    int val = GetMouseWheelRotVol();
    if (!Area::pre_hitkey_status) {
        if (CheckHitKey(KEY_INPUT_UP))      val++;
        if (CheckHitKey(KEY_INPUT_DOWN))    val--;
    }
    if (status != Invariable && val != 0)
        changing = 1;

    // �}�E�X�z�C�[����]�ʂɉ����ē�����ύX
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
        if (local.tm_year < 70)         local.tm_year = 70;
        else if (local.tm_year > 1100)  local.tm_year = 1100;
        break;
    case MonVariable:
        local.tm_mon += val;
        if (local.tm_mon < 0)           local.tm_mon += 12;
        else if (local.tm_mon >= 12)    local.tm_mon -= 12;
        break;
    case DVariable:
        int day_max;        // �����������܂ł��邩
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
    if (t < 0)  t = 0;                  // ��`�O�ɂ͂ݏo�����ꍇ��0s�ɐݒ�
    TimeSrc::SetCfg(t - time(NULL));
}

void ConfigTime::Draw()
{
    static int cnt = 15;    // �����ʑ��͎�����1/4�ɂ��Ă���

    // ���t��\��
    DrawFormatStringToHandle(DATE_X, DATE_Y, BLACK, hFont_d, "%4d�N%d��%d��(%s)", local.tm_year + 1900, local.tm_mon + 1, local.tm_mday, week_name[local.tm_wday]);
    // ���A����\��
    DrawFormatStringToHandle(TIME_X, TIME_Y, BLACK, hFont_hm, "%2d:%02d", local.tm_hour, local.tm_min);

    // �ύX���͓_�ł����Ȃ�
    if (changing == 0) {
        // �ύX���镔����_�ł�����
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

        int day_max;        // �a�����������܂ł��邩
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
    static int cnt;     // �����ʑ��͎�����1/4�ɂ��Ă���

    DrawFormatStringToHandle(x1, y1, BLACK, hFont_bd, "%2d��%d��", dcfg.birthmonth, dcfg.birthday);
    // �ύX���͓_�ł����Ȃ�
    if (changing == 0) {
        // �ύX���镔����_�ł�����
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
    hFont_main   = CreateFontToHandle(NULL, FSIZE_MAIN,   5, DX_FONTTYPE_ANTIALIASING_4X4);
    hFont_choice = CreateFontToHandle(NULL, FSIZE_CHOICE, 3, DX_FONTTYPE_ANTIALIASING_4X4);
    hFont_d      = CreateFontToHandle(NULL, FSIZE_D,     -1, DX_FONTTYPE_ANTIALIASING_4X4);
    hFont_hm     = CreateFontToHandle(NULL, FSIZE_HM,    -1, DX_FONTTYPE_ANTIALIASING_4X4);
    hFont_bd     = CreateFontToHandle(NULL, FSIZE_BD,    -1, DX_FONTTYPE_ANTIALIASING_4X4);
}

Config::Config(ISceneChanger *changer) : BaseScene(changer), tab(Tab_MainCfg), set_bd(temp.dcfg),
h_form(HFORM_X, HFORM_Y, HFORM_W, HFORM_H, "�����\���`��", temp.tcfg.h_form),
auto_repro(AUTOREPRO_X, AUTOREPRO_Y, AUTOREPRO_W, AUTOREPRO_H, "�����Đ�",     temp.vcfg.auto_repro),
sound_year(SOUNDYEAR_X, SOUNDYEAR_Y, SOUNDYEAR_W, SOUNDYEAR_H, "�N�ǂݏグ",   temp.dcfg.sound_year),
sound_week(SOUNDWEEK_X, SOUNDWEEK_Y, SOUNDWEEK_W, SOUNDWEEK_H, "�j���ǂݏグ", temp.dcfg.sound_week),
tail_date (TAILDATE_X,  TAILDATE_Y,  TAILDATE_W,  TAILDATE_H,  "���(���t)",   temp.dcfg.tail),
tail_time (TAILTIME_X,  TAILTIME_Y,  TAILTIME_W,  TAILTIME_H,  "���(����)",   temp.tcfg.tail),
for_prg   (FORPRG_X,    FORPRG_Y,    FORPRG_W,    FORPRG_H,    "�v���O���}�p", temp.vcfg.for_prg),

reset(RESET_X, RESET_Y, RESET_W, "����l�ɖ߂�", hFont_main),
complete(COMP_X, COMP_Y, COMP_W, "����", hFont_main),
cancel(CANCEL_X, CANCEL_Y, CANCEL_W, "�L�����Z��", hFont_main)
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
    pVoice->SoundSys(2);                        // �ݒ��ʂֈڂ�Ƃ��̉������Đ�

    // �e���ڂɑI������o�^
    h_form.PushBack(new Choice<eHourForm>(HFORM_X    + SCALEX(30),  HFORM_Y     + SCALEY(26), SCALEX(100), "0-24 �\��", Form24h));
    h_form.PushBack(new Choice<eHourForm>(HFORM_X    + SCALEX(170), HFORM_Y     + SCALEY(26), SCALEX(100), "0-12 �\��", Form12h));

    auto_repro.PushBack(new Choice<bool>(AUTOREPRO_X + SCALEX(30),  AUTOREPRO_Y + SCALEY(26), SCALEX(100), "ON",  true));
    auto_repro.PushBack(new Choice<bool>(AUTOREPRO_X + SCALEX(170), AUTOREPRO_Y + SCALEY(26), SCALEX(100), "OFF", false));

    sound_year.PushBack(new Choice<bool>(SOUNDYEAR_X + SCALEX(30),  SOUNDYEAR_Y + SCALEY(26), SCALEX(100), "ON",  true));
    sound_year.PushBack(new Choice<bool>(SOUNDYEAR_X + SCALEX(170), SOUNDYEAR_Y + SCALEY(26), SCALEX(100), "OFF", false));

    sound_week.PushBack(new Choice<bool>(SOUNDWEEK_X + SCALEX(30),  SOUNDWEEK_Y + SCALEY(26), SCALEX(100), "ON",  true));
    sound_week.PushBack(new Choice<bool>(SOUNDWEEK_X + SCALEX(170), SOUNDWEEK_Y + SCALEY(26), SCALEX(100), "OFF", false));

    tail_date.PushBack(new Choice<eTail>(TAILDATE_X  + SCALEX(30),  TAILDATE_Y  + SCALEY(25), SCALEX(100), "�c���ˁI", Tail_Dane));
    tail_date.PushBack(new Choice<eTail>(TAILDATE_X  + SCALEX(30),  TAILDATE_Y  + SCALEY(62), SCALEX(100), "�c����I", Tail_Dayo));
    tail_date.PushBack(new Choice<eTail>(TAILDATE_X  + SCALEX(155), TAILDATE_Y  + SCALEY(25), SCALEX(130), "�c�ɂȂ�����I", Tail_Ninattayo));
    tail_date.PushBack(new Choice<eTail>(TAILDATE_X  + SCALEX(155), TAILDATE_Y  + SCALEY(62), SCALEX(130), "�c�����āI", Tail_Datte));
    tail_date.PushBack(new Choice<eTail>(TAILDATE_X  + SCALEX(30),  TAILDATE_Y  + SCALEY(99), SCALEX(100), "�Ȃ�", Tail_None));

    tail_time.PushBack(new Choice<eTail>(TAILTIME_X  + SCALEX(30),  TAILTIME_Y  + SCALEY(25), SCALEX(100), "�c���ˁI", Tail_Dane));
    tail_time.PushBack(new Choice<eTail>(TAILTIME_X  + SCALEX(30),  TAILTIME_Y  + SCALEY(62), SCALEX(100), "�c����I", Tail_Dayo));
    tail_time.PushBack(new Choice<eTail>(TAILTIME_X  + SCALEX(155), TAILTIME_Y  + SCALEY(25), SCALEX(130), "�c�ɂȂ�����I", Tail_Ninattayo));
    tail_time.PushBack(new Choice<eTail>(TAILTIME_X  + SCALEX(155), TAILTIME_Y  + SCALEY(62), SCALEX(130), "�c�����āI", Tail_Datte));
    tail_time.PushBack(new Choice<eTail>(TAILTIME_X  + SCALEX(30),  TAILTIME_Y  + SCALEY(99), SCALEX(100), "�Ȃ�", Tail_None));

    for_prg.PushBack(new Choice<bool>(FORPRG_X       + SCALEX(30),  FORPRG_Y    + SCALEY(25), SCALEX(100), "ON",  true));
    for_prg.PushBack(new Choice<bool>(FORPRG_X       + SCALEX(170), FORPRG_Y    + SCALEY(25), SCALEX(100), "OFF", false));
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

        // �^�u�؂�ւ�
        if (Area(TAB_TIMEADG_X, TAB_Y, TAB_TIMEADG_W, TAB_H).IsClicked())
            tab = Tab_TimeAdj;
        else if (Area(TAB_SETBIRTHDAY_X, TAB_Y, TAB_SETBIRTHDAY_W, TAB_H).IsClicked())
            tab = Tab_SetBirthday;
        break;
    case Tab_TimeAdj:
        cfg_time.Update();

        // �^�u�؂�ւ�
        if (Area(TAB_SETBIRTHDAY_X, TAB_Y, TAB_SETBIRTHDAY_W, TAB_H).IsClicked()) {
            cfg_time.ResetStatus();
            tab = Tab_SetBirthday;
        }
        else if (Area(TAB_X, TAB_Y, TAB_MAINCFG_W, TAB_H).IsClicked()) {
            cfg_time.ResetStatus();
            tab = Tab_MainCfg;
        }
        break;
    case Tab_SetBirthday:
        set_bd.Update();

        // �^�u�؂�ւ�
        if (Area(TAB_X, TAB_Y, TAB_MAINCFG_W, TAB_H).IsClicked()) {
            set_bd.ResetStatus();
            tab = Tab_MainCfg;
        }
        else if (Area(TAB_TIMEADG_X, TAB_Y, TAB_TIMEADG_W, TAB_H).IsClicked()) {
            set_bd.ResetStatus();
            tab = Tab_TimeAdj;
        }
    }

    // ����l�ɖ߂��{�^���������ꂽ�Ƃ�
    if (reset.IsClicked())
        switch (tab) {
        case Tab_MainCfg:
            temp.Default();                 // �ݒ���f�t�H���g�ɖ߂�
            break;
        case Tab_TimeAdj:
            cfg_time.SetCfg(0);             // �������[���ɖ߂�
            break;
        case Tab_SetBirthday:
            temp.DefaultBd();               // �ݒ���f�t�H���g�ɖ߂�
            break;
        }
    // �����{�^���������ꂽ�Ƃ�
    else if (complete.IsClicked()) {
        pVoice->StopSound();
        pVoice->SoundSys(3);
        Reflect();                                  // �ݒ�𔽉f
        pScene_changer->ChangeScene(eScene_Clock);  // �V�[�������C����ʂɕύX
    }
    // �L�����Z���{�^���������ꂽ�Ƃ�
    else if (cancel.IsClicked())
        pScene_changer->ChangeScene(eScene_Clock);  // �V�[�������C����ʂɕύX
}

void Config::Draw()
{
    // �^�u�S�̂̔w�i��`��
    DrawBox(TAB_X, TAB_Y, TAB_SETBIRTHDAY_X + TAB_SETBIRTHDAY_W, MAINAREA_Y, 0xC4A9E8, TRUE);
    DrawLine(TAB_TIMEADG_X,     TAB_Y, TAB_TIMEADG_X,     MAINAREA_Y, 0xD9BEF8, SCALEX(1.5));     // ���̐F���VIOLET1 0xD3BEED 0x82709a
    DrawLine(TAB_SETBIRTHDAY_X, TAB_Y, TAB_SETBIRTHDAY_X, MAINAREA_Y, 0xD9BEF8, SCALEX(1.5));
    DrawStringToHandle(TAB_X             + TAB_SPACE, TAB_STRING_Y, "�ݒ�",       GetColor(70, 70, 70), hFont_main);
    DrawStringToHandle(TAB_TIMEADG_X     + TAB_SPACE, TAB_STRING_Y, "�����ݒ�",   GetColor(70, 70, 70), hFont_main);
    DrawStringToHandle(TAB_SETBIRTHDAY_X + TAB_SPACE, TAB_STRING_Y, "�a�����ݒ�", GetColor(70, 70, 70), hFont_main);
    DrawBox(MAINAREA_X, MAINAREA_Y, MAINAREA_X + MAINAREA_W, MAINAREA_Y + MAINAREA_H, VIOLET1, TRUE);

    switch (tab) {
    case Tab_MainCfg:
        // �w�i��`��
        DrawBox(TAB_MAINCFG_X, TAB_Y - SCALEY(2), TAB_MAINCFG_X + TAB_MAINCFG_W + SCALEX(1.5), MAINAREA_Y, VIOLET1, TRUE);
        DrawStringToHandle(TAB_MAINCFG_X + TAB_SPACE, TAB_STRING_Y - SCALEY(2), "�ݒ�", BLACK, hFont_main);

        h_form.Draw();
        auto_repro.Draw();
        sound_year.Draw();
        sound_week.Draw();
        tail_date.Draw();
        tail_time.Draw();
        for_prg.Draw();
        break;
    case Tab_TimeAdj:
        // �w�i��`��
        DrawBox(TAB_TIMEADG_X, TAB_Y - SCALEY(2), TAB_TIMEADG_X + TAB_TIMEADG_W + SCALEX(1.5), MAINAREA_Y, VIOLET1, TRUE);
        DrawStringToHandle(TAB_TIMEADG_X + TAB_SPACE, TAB_STRING_Y - SCALEY(2), "�����ݒ�", BLACK, hFont_main);

        cfg_time.Draw();
        break;
    case Tab_SetBirthday:
        // �w�i��`��
        DrawBox(TAB_SETBIRTHDAY_X, TAB_Y - SCALEY(2), TAB_SETBIRTHDAY_X + TAB_SETBIRTHDAY_W + SCALEX(1.5), MAINAREA_Y, VIOLET1, TRUE);
        DrawStringToHandle(TAB_SETBIRTHDAY_X + TAB_SPACE, TAB_STRING_Y - SCALEY(2), "�a�����ݒ�", BLACK, hFont_main);

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
