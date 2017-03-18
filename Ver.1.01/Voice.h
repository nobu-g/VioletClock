#ifndef VOICE_H
#define VOICE_H

#include <array>
#include <map>
using std::array;
using std::map;

enum eTmData { None, Year, Month, Day, Week, Noon, Hour, TimeSignal, Minute, Tail };

// Voice�֌W�̒萔
#define YEAR		7		// �N�̉����̐�(2014�`2020)
#define MONTH		12		// ���̉����̐�(1�`12��)
#define DAY			31		// ���̉����̐�(1�`31��)
#define WEEK		7		// �j���̉����̐�(���`�y�j��)
#define NOON		2		// �ߑO�E�ߌ�̉����̐�
#define HOUR		24		// ���̉����̐�(0�`23��)
#define MINUTE		60		// ���̉����̐�(0�`59��)
#define TAIL		4		// ����̎��(�c���ˁI�c����!�c�ɂȂ�����!)
#define SEASON		10		// �L������������t�ˑ��̑䎌�̎��
#define SYSTEM		3		// �V�X�e�������̎��
#define WORDS		132		// �L�����̑䎌�̎��

// �L�����̑䎌�N���X
class Words {
	int hWord;				// �䎌�����ւ̃n���h��

	// min_hour��min_minute������Amax_hour��max_minute���̊ԁA�䎌�𒝂邱�Ƃ��ł���
	int min_hour;
	int min_minute;
	int max_hour;
	int max_minute;

	bool programmer;		// �v���O���}�p�̑䎌���ǂ���
	bool kurei_kei;			// �L���������d���ǂ���
public:
	Words() {}
	Words(int minh, int maxh, bool prg, int hw);
	Words(double minh, double maxh, bool prg, int hw);
	Words(int minh, int maxh, bool prg, bool k, int hw);
	int GetHandle() { return hWord; }
	// ��������min_hour��min_minute������Amax_hour��max_minute���̊Ԃɂ��邩
	bool IsAppropriate();
};

// �����Ǘ��N���X
class Voice {
	// ���ꂼ�ꉹ���̃n���h�����i�[
	array<int, YEAR>	hYear;
	array<int, MONTH>	hMonth;
	array<int, DAY>		hDay;
	array<int, WEEK>	hWeek;
	array<int, NOON>	hNoon;
	array<int, HOUR>	hHour;
	array<int, HOUR>	hTime_signal;
	array<int, MINUTE>	hMinute;
	array<int, TAIL>	hTail;

	array<int, SEASON>	hSeason;
	array<int, SYSTEM>	hSystem;

	typedef unsigned char KeyCode;
	map<KeyCode, int>	keyboard;

	array<Words, WORDS>	words;

	int hPlaying;				// �Ō�ɍĐ����������̃n���h��
public:
	Voice();									// �����t�@�C���̃��[�h�Ȃ�
	bool IsPlaying()							// ���݉������Đ�����
	{
		return CheckSoundMem(hPlaying) && hPlaying != -1;
	}
	void SoundTime(eTmData data, int index);	// ���Ԋ֘A�̉����Đ�����
	void SoundWords();							// �L�����̃Z���t���Đ�����
	void SoundSys(int index);					// �V�X�e���������Đ�����
	void SoundKey(KeyCode key);					// �L�[��ǂݏグ��
	void Repeat();								// ���O�ɍĐ�����������������x�Đ�
	void StopSound();							// ���ݖ��Ă���T�E���h���~�߂�
};

extern Voice *pVoice;

#endif
