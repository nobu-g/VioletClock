#ifndef CLOCK_H
#define CLOCK_H

#include "Main.h"
#include "Config.h"
#include <ctime>

// Time, Date�֌W�̒萔
#define DATE_X		270		// ���t�����x���W
#define DATE_Y		330		// ���t�����y���W
#define TIME_X		150		// ���������x���W
#define TIME_Y		150		// ���������y���W
#define FSIZE_D		30		// ���t�\���p�t�H���g�̃T�C�Y
#define FSIZE_HM	150		// ���A���p�t�H���g�̃T�C�Y
#define FSIZE_S		50		// �b�p�t�H���g�̃T�C�Y
#define FSIZE_AP	35		// AM/PM�p�t�H���g�̃T�C�Y
#define SPACE_MS	10		// ���ƕb�̊Ԃ̊Ԋu(�s�N�Z��)

// ���C����ʃN���X
class Clock : public BaseScene {
public:
	Clock(ISceneChanger *changer) : BaseScene(changer){}
	void Initialize();
	void Finalize();
	void Update();
	void Draw();
};

// �W������񋟂��鎞�ԃN���X
class TimeSrc {
protected:
	static struct tm *local;	// ���Ԃ��i�[����\����
public:
	static void Update();		// ���ݎ������X�V
};

// ���t�N���X
class Date : public Area, public TimeSrc {
	int hFont_d;				// ���t�\���p�t�H���g
public:
	Date();
	int Gety() { return local->tm_year + 1900; }				// �N��Ԃ�
	int Getm() { return local->tm_mon + 1; }					// ����Ԃ�
	int Getd() { return local->tm_mday; }						// ����Ԃ�
	int Getw() { return local->tm_wday; }						// �j����Ԃ�
	int Gett() { return pCfgData->GetCfg(CfgData::TailDate); }	// �����Ԃ�
	void Draw();
	void Action();					// �N���b�N���ꂽ�Ƃ��̓���
	bool IsNewYear()				// ����������(1/1)���ǂ���
	{
		return local->tm_mon == 0 && local->tm_mday == 1;
	}
	bool IsHalloween()				// �������n���E�B��(10/31)���ǂ���				
	{
		return local->tm_mon == 9 && local->tm_mday == 31;
	}
	bool IsChristmas()				// �������N���X�}�X(12/25)���ǂ���
	{
		return local->tm_mon == 11 && local->tm_mday == 25;
	}
	bool IsBirthday()				// ������CV:��₷�݂�̒a����(12/19)��
	{
		return local->tm_mon == 11 && local->tm_mday == 19;
	}
	bool IsSummer()					// ������(6���`8��)���ǂ���
	{
		return local->tm_mon >= 6 && local->tm_mon <= 8;
	}
	bool IsWinter()					// �����~(12���`3��)���ǂ���
	{
		return local->tm_mon >= 11 && local->tm_mon <= 2;
	}
};

// �����N���X
class Time : public Area, public TimeSrc {
	int hFont_hm;				// ���A���p�t�H���g
	int hFont_s;				// �b�p�t�H���g
	int hFont_ap;				// AM/PM�p�t�H���g
	
	bool time_signal_flag;		// �����炳�Ȃ���΂Ȃ�Ȃ���
public:
	Time();
	void Init() { time_signal_flag = false; }					// ����������
	int Getn() { return (local->tm_hour < 12) ? 0 : 1; }		// �ߑO:0�A�ߌ�:1��Ԃ�
	int Geth() { return local->tm_hour; }						// ����Ԃ�
	int Getm() { return local->tm_min; }						// ����Ԃ�
	int Gets() { return local->tm_sec; }						// �b��Ԃ�
	int Gett() { return pCfgData->GetCfg(CfgData::TailTime); }	// �����Ԃ�
	void Draw();
	void Action();			// �N���b�N���ꂽ�Ƃ��̓���
};

extern Date *pDate;
extern Time *pTime;

#endif
