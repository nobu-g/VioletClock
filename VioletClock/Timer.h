#ifndef TIMER_H
#define TIMER_H

#include "Base.h"
#include "Config.h"
#include "Button.h"

// �t�H���g�T�C�Y
#define FSIZE_HMS       SCALEY(100)
#define FSIZE_VSELECT   SCALEY(16)

// ���ԕ\�������֌W�̒萔
#define TMRTIME_X       SCALEX(145)
#define TMRTIME_Y       SCALEY(150)
#define TMRTIME_W       (FSIZE_HMS * 7 / 2)
#define TMRTIME_H       FSIZE_HMS

// �߂�{�^���֌W�̒萔
#define BACK_X          COMP_X
#define BACK_Y          COMP_Y
#define BACK_W          COMP_W

// �^�C�}�[�I�����̉����̎��
#define OVER_VOICES     8

// �^�C�}�[�ݒ�ۑ��\����
struct TimerCfg {
    int hour;
    int minute;
    int second;
    int voice_index;    // �^�C�}�[�I�����ǂ̉������Đ����邩
    void Check()
    {
        if (hour < 0 || hour > 23)  hour = 0;
        if (minute < 0 || minute > 59)  minute = 0;
        if (second < 0 || second > 59)  second = 0;
        if (voice_index < 0 || voice_index >= OVER_VOICES)   voice_index = 0;
    }
};

// ���ԕ\�������̐���
class TimerTime : public Area {
    enum eTimerCfgStatus { Invariable, HVariable, MVariable, SVariable };
    eTimerCfgStatus status;                         // ���A���A�b�A�ǂ̕������ύX�\��
    int changing;                                   // �ݒ莞�ԕύX����30�t���[���𐔂���
public:
    TimerTime(TimerCfg &ref);
    void ResetStatus() { status = Invariable; }     // �X�e�[�^�X��Invariable�ɐݒ肷��
    void Update();
    void Draw();
};

// �I���������I��UI
class VoiceSelect {
    array<string, OVER_VOICES> words;
    TmrVoiceChgButton chg_btn;
public:
    VoiceSelect();
    void Update();
    void Draw();
};

// �^�C�}�[��ʂŎg���t�H���g�쐬�N���X
class FontMakerTimer {
public:
    FontMakerTimer();
};

// �^�C�}�[��ʃN���X
class TimerScreen : public BaseScene, public FontMakerTimer {
    TimerTime timer_time;
    TmrStartButton start_btn;
    TmrResetButton reset_btn;
    VoiceSelect vselect;
    Button back;
public:
    TimerScreen(ISceneChanger *changer);
    ~TimerScreen();
    void Initialize() {}
    void Finalize() {}
    void Update();
    void Draw();
};

// �^�C�}�[�̃J�E���g���s���N���X
class Timer : public IOBase<TimerCfg> {
public:
    enum eTimerStatus { Idle, Counting, Pause };
private:
    int base_ms;            // 1�b�o�߂��������ׂ邽�߂̊(�~���b)
    int stop_ms;            // �^�C�}�[���ꎞ��~��������(�~���b)
    int remaining_h;        // �c�莞��(��)
    int remaining_m;        // �c�莞��(��)
    int remaining_s;        // �c�莞��(�b)
    eTimerStatus status;
public:
    Timer();
    void Start();
    void Stop();
    void Restart();
    void Reset();
    int Geth() { return remaining_h; }
    int Getm() { return remaining_m; }
    int Gets() { return remaining_s; }
    eTimerStatus GetStatus() { return status; }
    void SoundCount();
    void Update();
    enum eCfg { Hour, Minute, Second, Voice };
    void SetCfg(eCfg cfg, int value);
};

extern Timer *pTimer;

#endif // !TIMER_H
