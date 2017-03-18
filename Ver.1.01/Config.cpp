#include "DxLib.h"
#include "Config.h"
#include "Voice.h"

#define BLACK	0x000000

extern std::ofstream ferr;

static int hFont_main;			// 設定画面で使う主要フォント
static int hFont_choice;		// 選択肢を表示するフォント

void CfgData::Default()
{
	auto_repro = true;
	h_form     = Form24h;
	sound_year = true;
	sound_week = true;
	for_prg    = false;
	tail_date  = None;
	tail_time  = None;
	chr_name   = KureiKei;
}

void CfgData::Setup()
{
	sound = true;
	auto_repro = true;
	h_form = Form24h;
	sound_year = true;
	sound_week = true;
	for_prg = false;
	tail_date = None;
	tail_time = None;
	chr_name = KureiKei;
}

int CfgData::GetCfg(eData data)
{
	switch (data) {
	case Sound:		return sound;
	case AutoRepro:	return auto_repro;
	case HForm:		return h_form;
	case SoundYear:	return sound_year;
	case SoundWeek:	return sound_week;
	case ForPrg:	return for_prg;
	case TailDate:	return tail_date;
	case TailTime:	return tail_time;
	case ChrName:	return chr_name;
	case FirstTime:	return first_time;
	}

	return -1;
}

void CfgData::SetCfg(eData data, int value)
{
	switch (data) {
	case Sound:		sound = (value != 0);		break;
	case AutoRepro:	auto_repro = (value != 0);	break;
	case HForm:		h_form = (eHourForm)value;	break;
	case SoundYear:	sound_year = (value != 0);	break;
	case SoundWeek:	sound_week = (value != 0);	break;
	case ForPrg:	for_prg = (value != 0);		break;
	case TailDate:	tail_date = (eTail)value;	break;
	case TailTime:	tail_time = (eTail)value;	break;
	case ChrName:	chr_name = (eWho)value;		break;
	case FirstTime:	first_time = (value != 0);	break;
	}
}

Button::Button(int x, int y, int w, string str) : Area::Area(x, y, w, FSIZE_MAIN + MARGIN*2)
{
	name = str;
	str_width = GetDrawStringWidthToHandle(str.c_str(), str.length(), hFont_main);
}

void Button::Draw()
{
	// カーソルが上にあるとき影を表示
	// 他の場所をクリックしながら移動してきた場合は表示しない
	if ((!pre_lclick_status && IsMouseOver()) || IsClicking())
		DrawBox(x1, y1, x2, y2, VIOLET3, TRUE);

	DrawStringToHandle((x2 + x1 - str_width) / 2, y1 + MARGIN, name.c_str(), BLACK, hFont_main);
}

Item::Item(int x0, int y0, string str, eItemType t, CfgData &tmp) : Area(x0 + FSIZE_MAIN/2, y0 + FSIZE_MAIN/2), temp(tmp)
{
	x = x0;
	y = y0;
	type = t;
	name = str;

	switch (t) {
	case Item::Type_HForm:
		pChoice.push_back(new Choice(x1 + 20,  y1 + 16, 100, "0-24 表示"));
		pChoice.push_back(new Choice(x1 + 160, y1 + 16, 100, "0-12 表示"));
		SetWidth(280);
		SetHeight(65);
		break;
	case Item::Type_AutoRepro:
	case Item::Type_SoundYear:
	case Item::Type_SoundWeek:
		pChoice.push_back(new Choice(x1 + 20,  y1 + 16, 100, "ON"));
		pChoice.push_back(new Choice(x1 + 160, y1 + 16, 100, "OFF"));
		SetWidth(280);
		SetHeight(65);
		break;
	case Item::Type_TailDate:
	case Item::Type_TailTime:
		pChoice.push_back(new Choice(x1 + 20,  y1 + 15, 100, "…だね！"));
		pChoice.push_back(new Choice(x1 + 20,  y1 + 52, 100, "…だよ！"));
		pChoice.push_back(new Choice(x1 + 145, y1 + 15, 130, "…になったよ！"));
		pChoice.push_back(new Choice(x1 + 145, y1 + 52, 130, "…だって！"));
		pChoice.push_back(new Choice(x1 + 20,  y1 + 89, 100, "なし"));
		SetWidth(280);
		SetHeight(130);
		break;
	case Item::Type_ForPrg:;
		pChoice.push_back(new Choice(x1 + 20,  y1 + 15, 100, "ON"));
		pChoice.push_back(new Choice(x1 + 160, y1 + 15, 100, "OFF"));
		SetWidth(280);
		SetHeight(57);
		break;
	}
}

Item::~Item()
{
	for (int i = 0; i < (int)pChoice.size(); i++)
		delete pChoice[i];
}

void Item::Update()
{
	switch(type){
	case Type_HForm:
		if (pChoice[0]->IsClicked())
			temp.SetCfg(CfgData::HForm, CfgData::Form24h);
		else if (pChoice[1]->IsClicked())
			temp.SetCfg(CfgData::HForm, CfgData::Form12h);
		break;
	case Type_AutoRepro:
		if (pChoice[0]->IsClicked())
			temp.SetCfg(CfgData::AutoRepro, true);
		else if (pChoice[1]->IsClicked())
			temp.SetCfg(CfgData::AutoRepro, false);
		break;
	case Type_SoundYear:
		if (pChoice[0]->IsClicked())
			temp.SetCfg(CfgData::SoundYear, true);
		else if (pChoice[1]->IsClicked())
			temp.SetCfg(CfgData::SoundYear, false);
		break;
	case Type_SoundWeek:
		if (pChoice[0]->IsClicked())
			temp.SetCfg(CfgData::SoundWeek, true);
		else if (pChoice[1]->IsClicked())
			temp.SetCfg(CfgData::SoundWeek, false);
		break;
	case Type_TailDate:
		if (pChoice[0]->IsClicked())
			temp.SetCfg(CfgData::TailDate, CfgData::Dane);
		else if (pChoice[1]->IsClicked())
			temp.SetCfg(CfgData::TailDate, CfgData::Dayo);
		else if (pChoice[2]->IsClicked())
			temp.SetCfg(CfgData::TailDate, CfgData::Ninattayo);
		else if (pChoice[3]->IsClicked())
			temp.SetCfg(CfgData::TailDate, CfgData::Datte);
		else if (pChoice[4]->IsClicked())
			temp.SetCfg(CfgData::TailDate, CfgData::None);
		break;
	case Type_TailTime:
		if (pChoice[0]->IsClicked())
			temp.SetCfg(CfgData::TailTime, CfgData::Dane);
		else if (pChoice[1]->IsClicked())
			temp.SetCfg(CfgData::TailTime, CfgData::Dayo);
		else if (pChoice[2]->IsClicked())
			temp.SetCfg(CfgData::TailTime, CfgData::Ninattayo);
		else if (pChoice[3]->IsClicked())
			temp.SetCfg(CfgData::TailTime, CfgData::Datte);
		else if (pChoice[4]->IsClicked())
			temp.SetCfg(CfgData::TailTime, CfgData::None);
		break;
	case Type_ForPrg:
		if (pChoice[0]->IsClicked())
			temp.SetCfg(CfgData::ForPrg, true);
		else if (pChoice[1]->IsClicked())
			temp.SetCfg(CfgData::ForPrg, false);
		break;
	}
}

void Item::Draw()
{
	// 項目を囲む枠を表示
	DrawBox(x1, y1, x2, y2, 0x80a0ff, FALSE);

	// 項目名の背景
	DrawBox(x, y, x + GetDrawStringWidthToHandle(name.c_str(), name.length(), hFont_main), y + FSIZE_MAIN, VIOLET1, TRUE);

	// 設定項目名を表示
	DrawStringToHandle(x, y, name.c_str(), BLACK, hFont_main);

	// 選択肢の選択されているときの影を表示
	switch (type){
	case Type_HForm:
		if (temp.GetCfg(CfgData::HForm) == CfgData::Form24h)
			pChoice[0]->DrawShade();
		else
			pChoice[1]->DrawShade();
		break;
	case Type_AutoRepro:
		if (temp.GetCfg(CfgData::AutoRepro))
			pChoice[0]->DrawShade();
		else
			pChoice[1]->DrawShade();
		break;
	case Type_SoundYear:
		if (temp.GetCfg(CfgData::SoundYear))
			pChoice[0]->DrawShade();
		else
			pChoice[1]->DrawShade();
		break;
	case Type_SoundWeek:
		if (temp.GetCfg(CfgData::SoundWeek))
			pChoice[0]->DrawShade();
		else
			pChoice[1]->DrawShade();
		break;
	case Type_TailDate:
		switch (temp.GetCfg(CfgData::TailDate)) {
		case CfgData::Dane:
			pChoice[0]->DrawShade();
			break;
		case CfgData::Dayo:
			pChoice[1]->DrawShade();
			break;
		case CfgData::Ninattayo:
			pChoice[2]->DrawShade();
			break;
		case CfgData::Datte:
			pChoice[3]->DrawShade();
			break;
		case CfgData::None:
			pChoice[4]->DrawShade();
			break;
		}
		break;
	case Type_TailTime:
		switch (temp.GetCfg(CfgData::TailTime)) {
		case CfgData::Dane:
			pChoice[0]->DrawShade();
			break;
		case CfgData::Dayo:
			pChoice[1]->DrawShade();
			break;
		case CfgData::Ninattayo:
			pChoice[2]->DrawShade();
			break;
		case CfgData::Datte:
			pChoice[3]->DrawShade();
			break;
		case CfgData::None:
			pChoice[4]->DrawShade();
			break;
		}
		break;
	case Type_ForPrg:
		if (temp.GetCfg(CfgData::ForPrg))
			pChoice[0]->DrawShade();
		else
			pChoice[1]->DrawShade();
		break;
	}

	// 選択肢(の文字)を表示
	for (int i = 0; i < (int)pChoice.size(); i++)
		pChoice[i]->Draw();
}

Choice::Choice(int x, int y, int w, string str) : 
	Area::Area(x, y, w, FSIZE_CHOICE + MARGIN * 2)
{
	name = str;
	str_width = GetDrawStringWidthToHandle(str.c_str(), str.length(), hFont_choice);
}

void Choice::Draw()
{
	// カーソルが上にあるとき影を表示
	// 他の場所をクリックしながら移動してきた場合は表示しない
	if ((!pre_lclick_status && IsMouseOver()) || IsClicking())
		DrawBox(x1, y1, x2, y2, VIOLET4, FALSE);

	DrawStringToHandle((x2 + x1 - str_width)/2, y1 + MARGIN, name.c_str(), BLACK, hFont_choice);
}

void Choice::DrawShade()
{
	// 選択されていることを示す影を表示
	DrawBox(x1, y1, x2, y2, VIOLET2, TRUE);
}

FontMaker::FontMaker()
{
	hFont_main = CreateFontToHandle(NULL, FSIZE_MAIN, 5, DX_FONTTYPE_ANTIALIASING_4X4);
	hFont_choice = CreateFontToHandle(NULL, FSIZE_CHOICE, 3, DX_FONTTYPE_ANTIALIASING_4X4);
}

Config::Config(ISceneChanger *changer) : BaseScene(changer), FontMaker(),
h_form    (CFGAREA_X + 15,               CFGAREA_Y + 15,  "時刻表示形式", Item::Type_HForm, temp),
auto_repro(CFGAREA_X + 15,               CFGAREA_Y + 109, "自動再生",     Item::Type_AutoRepro, temp),
sound_year(CFGAREA_X + 15,               CFGAREA_Y + 203, "年読み上げ",   Item::Type_SoundYear, temp),
sound_week(CFGAREA_X + 15,               CFGAREA_Y + 297, "曜日読み上げ", Item::Type_SoundWeek, temp),
tail_date (CFGAREA_X + CFGAREA_W/2 + 15, CFGAREA_Y + 15,  "語尾(日付)",   Item::Type_TailDate, temp),
tail_time (CFGAREA_X + CFGAREA_W/2 + 15, CFGAREA_Y + 165, "語尾(時刻)",   Item::Type_TailTime, temp),
for_prg   (CFGAREA_X + CFGAREA_W/2 + 15, CFGAREA_Y + 315, "プログラマ用", Item::Type_ForPrg, temp),

Reset(RESET_X, RESET_Y, RESET_W, "既定値に戻す"),
Complete(COMP_X, COMP_Y, COMP_W, "完了"),
Cancel(CANCEL_X, CANCEL_Y, CANCEL_W, "キャンセル")
{
	temp = *pCfgData;
}

Config::~Config()
{
	DeleteFontToHandle(hFont_main);
	DeleteFontToHandle(hFont_choice);
}

void Config::Initialize()
{
	pVoice->SoundSys(2);
}

void Config::Update()
{
	h_form.Update();
	auto_repro.Update();
	sound_year.Update();
	sound_week.Update();
	tail_date.Update();
	tail_time.Update();
	for_prg.Update();

	// 既定値に戻すボタンが押されたとき
	if (Reset.IsClicked())
		temp.Default();								// 設定をデフォルトに戻す
	// 完了ボタンが押されたとき
	else if (Complete.IsClicked()) {
		*pCfgData = temp;							// 設定を反映
		pScene_changer->ChangeScene(eScene_Clock);	// シーンをメイン画面に変更
	}
	// キャンセルボタンが押されたとき
	else if (Cancel.IsClicked())
		pScene_changer->ChangeScene(eScene_Clock);	// シーンをメイン画面に変更
}

void Config::Draw()
{
	DrawBox(CFGAREA_X, 5, 77, CFGAREA_Y, VIOLET1, TRUE);
	DrawBox(CFGAREA_X, CFGAREA_Y, CFGAREA_X + CFGAREA_W, CFGAREA_Y + CFGAREA_H, VIOLET1, TRUE);
	DrawStringToHandle(20, 13, "設定", BLACK, hFont_main);
	
	h_form.Draw();
	auto_repro.Draw();
	sound_year.Draw();
	sound_week.Draw();
	tail_date.Draw();
	tail_time.Draw();
	for_prg.Draw();

	Reset.Draw();
	Complete.Draw();
	Cancel.Draw();
}
