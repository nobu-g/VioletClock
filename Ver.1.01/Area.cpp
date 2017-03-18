#include "DxLib.h"
#include "Area.h"
#include "Config.h"
#include "Voice.h"
#include <Windows.h>

bool Area::pre_lclick_status = true;
void Area::Update()
{
	pre_lclick_status = GetMouseInput() & MOUSE_INPUT_LEFT;
}

bool Area::IsMouseOver()
{
	int mouse_x, mouse_y;

	GetMousePoint(&mouse_x, &mouse_y);
	if (x1 < mouse_x && mouse_x < x2 && y1 < mouse_y && mouse_y < y2)
		return true;

	return false;
}

bool Area::IsClicked()
{
	if (!pre_lclick_status && IsMouseOver() && (GetMouseInput() & MOUSE_INPUT_LEFT)) {
		is_clicking = true;
		return true;
	}

	return false;
}

bool Area::IsClicking()
{
	return is_clicking = is_clicking && IsMouseOver();
}

Chr::Chr() : Area::Area(CHR_X, CHR_Y, CHR_W, CHR_H)
{
	hChr[CfgData::KureiKei] = LoadGraph("image\\kurei_kei.png");
}

void Chr::Draw()
{
	DrawGraph(x1, y1, hChr[pCfgData->GetCfg(CfgData::ChrName)], TRUE);
}

// ���݂̃o�b�e���[�c�ʂ�%�ŕԂ�
int BatteryPercent()
{
	SYSTEM_POWER_STATUS pw_stat;

	GetSystemPowerStatus(&pw_stat);

	return pw_stat.BatteryLifePercent;
}

void Chr::Action()
{
	if (!pVoice->IsPlaying()) {
		// �N���b�N���ꂽ�特���𗬂�
		if (IsClicked())
			pVoice->SoundWords();


		if (pCfgData->GetCfg(CfgData::Sound) && pCfgData->GetCfg(CfgData::AutoRepro)) {
			// �����d�����Ȃ�������V�X�e���T�E���h�𗬂�
			static int bat_charge;
			static int pre_bat_charge = 100;

			bat_charge = BatteryPercent();
			if (bat_charge <= BAT_CRISIS && pre_bat_charge > BAT_CRISIS)
				pVoice->SoundSys(1);
			else if (bat_charge <= BAT_LOW && pre_bat_charge > BAT_LOW)
				pVoice->SoundSys(0);
			pre_bat_charge = bat_charge;
		}


		// �����L�[�{�[�h��������Ă����炻�̃L�[���Đ�����
		char key_state[256];

		GetHitKeyStateAll(key_state);
		if (key_state[KEY_INPUT_SPACE])		pVoice->SoundKey(KEY_INPUT_SPACE);
		if (key_state[KEY_INPUT_RETURN])	pVoice->SoundKey(KEY_INPUT_RETURN);
		if (key_state[KEY_INPUT_BACK])		pVoice->SoundKey(KEY_INPUT_BACK);
		if (key_state[KEY_INPUT_ESCAPE])	pVoice->SoundKey(KEY_INPUT_ESCAPE);
		if (key_state[KEY_INPUT_LWIN]     ||
			key_state[KEY_INPUT_RWIN])		pVoice->SoundKey(KEY_INPUT_WIN);
		if (key_state[KEY_INPUT_LCONTROL] ||
			key_state[KEY_INPUT_RCONTROL])	pVoice->SoundKey(KEY_INPUT_CTRL);
		if (key_state[KEY_INPUT_LALT]     ||
			key_state[KEY_INPUT_RALT])		pVoice->SoundKey(KEY_INPUT_ALT);
		if (key_state[KEY_INPUT_LSHIFT]   ||
			key_state[KEY_INPUT_RSHIFT])	pVoice->SoundKey(KEY_INPUT_SHIFT);
		if (key_state[KEY_INPUT_TAB])		pVoice->SoundKey(KEY_INPUT_TAB);
		if (key_state[KEY_INPUT_INSERT])	pVoice->SoundKey(KEY_INPUT_INSERT);
		if (key_state[KEY_INPUT_DELETE])	pVoice->SoundKey(KEY_INPUT_DELETE);
		if (key_state[KEY_INPUT_SYSRQ])		pVoice->SoundKey(KEY_INPUT_SYSRQ);
		if (key_state[KEY_INPUT_HOME])		pVoice->SoundKey(KEY_INPUT_HOME);
		if (key_state[KEY_INPUT_END])		pVoice->SoundKey(KEY_INPUT_END);
		if (key_state[KEY_INPUT_PGUP])		pVoice->SoundKey(KEY_INPUT_PGUP);
		if (key_state[KEY_INPUT_PGDN])		pVoice->SoundKey(KEY_INPUT_PGDN);
		if (key_state[KEY_INPUT_UP])		pVoice->SoundKey(KEY_INPUT_UP);
		if (key_state[KEY_INPUT_DOWN])		pVoice->SoundKey(KEY_INPUT_DOWN);
		if (key_state[KEY_INPUT_RIGHT])		pVoice->SoundKey(KEY_INPUT_RIGHT);
		if (key_state[KEY_INPUT_LEFT])		pVoice->SoundKey(KEY_INPUT_LEFT);
	}
}

RptButton::RptButton() : Area(RPT_X, RPT_Y, RPT_W, RPT_H)
{
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

void RptButton::Action()
{
	if (!pVoice->IsPlaying() && IsClicked())
		pVoice->Repeat();
}

CfgButton::CfgButton() : Area(CFG_X, CFG_Y, CFG_W, CFG_H)
{
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
	hSnd_img = LoadGraph("image\\sound_icon.png");
	hMute_img = LoadGraph("image\\mute_icon.png");
}

void MuteButton::Draw()
{
	// �J�[�\������ɂ���Ƃ��e��\��
	// ���̏ꏊ���N���b�N���Ȃ���ړ����Ă����ꍇ�͕\�����Ȃ�
	if ((!pre_lclick_status && IsMouseOver()) || IsClicking())
		DrawBox(x1, y1, x2, y2, VIOLET3, TRUE);

	// �~���[�g�{�^����\��
	if (pCfgData->GetCfg(CfgData::Sound))
		DrawGraph(x1, y1, hSnd_img, TRUE);
	else
		DrawGraph(x1, y1, hMute_img, TRUE);
}

void MuteButton::Action()
{
	// �N���b�N���ꂽ���A�~���[�g/�~���[�g������؂�ւ�
	if (IsClicked()) {
		if (pCfgData->GetCfg(CfgData::Sound)) {
			pVoice->StopSound();
			pCfgData->SetCfg(CfgData::Sound, false);
		}
		else
			pCfgData->SetCfg(CfgData::Sound, true);
	}
}
