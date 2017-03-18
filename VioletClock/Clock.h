#ifndef CLOCK_H
#define CLOCK_H

#include "Main.h"
#include "Base.h"
#include "Button.h"
#include "Voice.h"
#include <ctime>

// Time, Date�֌W�̒萔
#define DATE_X      270     // ���t�����x���W
#define DATE_Y      330     // ���t�����y���W
#define TIME_X      150     // ���������x���W
#define TIME_Y      150     // ���������y���W
#define FSIZE_D     30      // ���t�\���p�t�H���g�̃T�C�Y
#define FSIZE_HM    150     // ���A���p�t�H���g�̃T�C�Y
#define FSIZE_S     50      // �b�p�t�H���g�̃T�C�Y
#define FSIZE_AP    35      // AM/PM�p�t�H���g�̃T�C�Y
#define SPACE_MS    10      // ���ƕb�̊Ԃ̊Ԋu(�s�N�Z��)

enum eHourForm { Form12h, Form24h };
enum eTail { Tail_Dane, Tail_Dayo, Tail_Ninattayo, Tail_Datte, Tail_None };

// ���C����ʃN���X
class Clock : public BaseScene {
    Chr character;
    RptButton rpt_btn;
    CfgButton cfg_btn;
    MuteButton mute_btn;
    AlmButton alm_btn;
    TmrButton tmr_btn;
public:
    Clock(ISceneChanger *changer) : BaseScene(changer) {}
    void Initialize();
    void Finalize();
    void Update();
    void Draw();
};

// �����ݒ�ۑ��\����
struct TimeSrcCfg {
    time_t offset;          // �W�����Ƃ̂���(�b)
    void Check()            // �����o���s���Ȓl�̎��C������
    {
        // 1970�N����3000�N�܂ł̕b��
        if (offset < -32535244799 || offset > 32535244799)
            offset = 0;
    }
};

// �W������񋟂��鎞�ԃN���X
class TimeSrc : public IOBase<TimeSrcCfg> {
protected:
    tm local;           // ���Ԃ��i�[����\����
public:
    TimeSrc();
    void Update();                  // ���ݎ������X�V
    void SetCfg(time_t t);          // �ݒ��ύX����
    friend class Date;              // local�ɃA�N�Z�X������
    friend class Time;              // local�ɃA�N�Z�X������
};
extern TimeSrc *pTimeSrc;

// ���t�ݒ�ۑ��\����
struct DateCfg {
    bool sound_year;        // �N��ǂݏグ�邩�ǂ���
    bool sound_week;        // �j����ǂݏグ�邩�ǂ���
    eTail tail;             // ���t�ǂݏグ���̌��
    int birthmonth;         // �a����
    int birthday;           // �a����
    void Check()            // �����o���s���Ȓl�̎��C������
    {
        switch (tail) {
        case Tail_Dane:  case Tail_Dayo:  case Tail_Ninattayo:  case Tail_Datte:  case Tail_None:  break;
        default:        tail = Tail_None; break;
        }
        if (birthmonth < 1 || birthmonth > 12)  birthmonth = 12;
        if (birthday   < 1 || birthday   > 31)  birthday = 19;
    }
};

// ���t�N���X
class Date : public Area, public IOBase<DateCfg> {
    int hFont_d;                // ���t�\���p�t�H���g
    eTmData speaking;           // ���ݍĐ�����Ă��鉹��
public:
    Date();
    int Gety() { return pTimeSrc->local.tm_year + 1900; }   // �N��Ԃ�
    int Getm() { return pTimeSrc->local.tm_mon + 1; }       // ����Ԃ�
    int Getd() { return pTimeSrc->local.tm_mday; }          // ����Ԃ�
    int Getw() { return pTimeSrc->local.tm_wday; }          // �j����Ԃ�
    int Gett() { return config.tail; }                      // �����Ԃ�
    void Draw();
    void Update();                          // �N���b�N���ꂽ�Ƃ��̓���
    void ResetSpeaking();                   // ���t�̓ǂݏグ�����Z�b�g
    bool IsNewYear() {                      // ����������(1/1)���ǂ���
        return Getm() == 1 && Getd() == 1;
    }
    bool IsHalloween() {                    // �������n���E�B��(10/31)���ǂ���
        return Getm() == 10 && Getd() == 31;
    }
    bool IsChristmas() {                    // �������N���X�}�X(12/25)���ǂ���
        return Getm() == 12 && Getd() == 25;
    }
    bool IsBirthday() {                     // �������a������
        return Getm() == config.birthmonth && Getd() == config.birthday;
    }
    bool IsSummer() {                       // ������(6���`8��)���ǂ���
        return Getm() >= 6 && Getm() <= 8;
    }
    bool IsWinter() {                       // �����~(12���`3��)���ǂ���
        return Getm() >= 12 || Getm() <= 3;
    }
    enum eCfg { SoundYear, SoundWeek, TailDate, BirthMonth, BirthDay };
    void SetCfg(eCfg cfg, int value);                               // �ݒ��ύX����
};

// �����ݒ�ۑ��\����
struct TimeCfg {
    eHourForm h_form;       // ������0-12�`��/0-24�`���̂ǂ���ŕ\�����邩
    eTail tail;             // �����ǂݏグ���̌��
    void Check()            // �����o���s���Ȓl�̎��C������
    {
        if (h_form != Form12h && h_form != Form24h) h_form = Form24h;
        switch (tail) {
        case Tail_Dane:  case Tail_Dayo:  case Tail_Ninattayo:  case Tail_Datte:  case Tail_None:  break;
        default:        tail = Tail_None; break;
        }
    }
};

// �����N���X
class Time : public Area, public IOBase<TimeCfg> {
    int hFont_hm;               // ���A���p�t�H���g
    int hFont_s;                // �b�p�t�H���g
    int hFont_ap;               // AM/PM�p�t�H���g

    eTmData speaking;           // ���ݍĐ�����Ă��鉹��
    bool time_signal_flag;      // �����炳�Ȃ���΂Ȃ�Ȃ���
public:
    Time();
    void Init() { time_signal_flag = false; }                       // ����������
    int Getn() { return (pTimeSrc->local.tm_hour < 12) ? 0 : 1; }   // �ߑO:0�A�ߌ�:1��Ԃ�
    int Geth() { return pTimeSrc->local.tm_hour; }                  // ����Ԃ�
    int Getm() { return pTimeSrc->local.tm_min; }                   // ����Ԃ�
    int Gets() { return pTimeSrc->local.tm_sec; }                   // �b��Ԃ�
    int Gett() { return config.tail; }                              // �����Ԃ�
    void Draw();
    void Update();                                                  // �N���b�N���ꂽ�Ƃ��̓���
    void ResetSpeaking();                                           // �����̓ǂݏグ�����Z�b�g
    enum eCfg { HForm, TailTime };
    void SetCfg(eCfg cfg, int value);                               // �ݒ��ύX����
};

extern Date *pDate;
extern Time *pTime;

#endif // !CLOCK_H
