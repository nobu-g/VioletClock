#ifndef ALARM_H
#define ALARM_H

#include "Main.h"
#include "Button.h"
#include "Config.h"

#define ALMTIME_X       SCALEX(170)     // �ݒ莞����x���W
#define ALMTIME_Y       SCALEY(150)     // �ݒ莞����y���W
#define FSIZE_T         SCALEY(120)     // �ݒ莞���̃t�H���g�T�C�Y

#define ALARMVALID_X    (MAINAREA_X + SCALEX(15))
#define ALARMVALID_Y    (MAINAREA_Y + SCALEY(297))
#define ALARMVALID_W    SCALEX(280)
#define ALARMVALID_H    SCALEY(65)
#define MODE_X          (MAINAREA_X + MAINAREA_W/2 + SCALEX(15))
#define MODE_Y          (MAINAREA_Y + SCALEY(297))
#define MODE_W          SCALEX(280)
#define MODE_H          SCALEY(65)

// �A���[���ݒ�ۑ��\����
struct AlarmCfg {
    int alarm_h;                    // �A���[����炷����
    int alarm_m;                    // �A���[����炷��
    bool is_alarm_valid;            // �A���[�����I����Ԃ�
    bool is_wakeup_mode;            // true:�ڊo�܂����[�h�Cfalse:�ʏ탂�[�h
    void Check()                    // �����o���s���Ȓl�̎��C������
    {
        if (alarm_h < 0 || alarm_h > 23)    alarm_h = 0;
        if (alarm_m < 0 || alarm_m > 59)    alarm_m = 0;
    }
};

// �����\�������̐���
class AlarmTime : public Area {
    AlarmCfg &temp;

    enum eTimeCfgStatus { Invariable, HVariable, MVariable };
    eTimeCfgStatus status;
    int changing;           // �ݒ莞���ύX����30�t���[���𐔂���
    int hFont;              // �ݒ莞����\������t�H���g
public:
    AlarmTime(AlarmCfg &ref);
    ~AlarmTime();
    void Update();
    void Draw();
};

// �A���[����ʂŎg���t�H���g�쐬�N���X
class FontMakerAlarm {
public:
    FontMakerAlarm();
};

// �A���[����ʃN���X
class AlarmScreen : public BaseScene, public FontMakerAlarm {
    AlarmCfg temp;                  // �A���[���ݒ�ꎞ�ۑ��I�u�W�F�N�g

    AlarmTime alarm_time;           // �ݒ莞���I�u�W�F�N�g
    Item<bool> alarm_valid;         // �A���[��ON/OFF�؂�ւ�UI
    Item<bool> mode;                // �A���[���ݒ�؂�ւ�UI

    Button complete;                // �ݒ芮���{�^��
    Button cancel;                  // �L�����Z���{�^��
public:
    AlarmScreen(ISceneChanger *changer);
    ~AlarmScreen();
    void Initialize();
    void Finalize() {}
    void Update();
    void Draw();
    void Reflect();                 // temp�����ݒ�𔽉f������
};

// �A���[���̃o�b�N�O���E���h�������s��
class Alarm : public IOBase<AlarmCfg> {
    bool is_ringing;            // �A���[�������ݖ��Ă��邩
public:
    Alarm();
    void Update();                                              // �A���[����炷�����Ȃ�
    enum eCfg { AlarmH, AlarmM, IsAlarmValid, IsWakeupMode };
    void SetCfg(eCfg cfg, int value);                           // �ݒ��ύX����
};

extern Alarm *pAlarm;

#endif // !ALARM_H
