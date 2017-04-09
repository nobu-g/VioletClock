#ifndef VOICE_H
#define VOICE_H

#include "Base.h"
#include "Character.h"
#include <array>
#include <unordered_map>
using std::ofstream;
using std::ifstream;
using std::array;
using std::unordered_map;

enum eTmData { None, Year, Month, Day, Week, Noon, Hour, TimeSignal, Minute, Tail };

// Voice�֌W�̒萔
#define YEAR        7       // �N�̉����̐�(2014�`2020)
#define MONTH       12      // ���̉����̐�(1�`12��)
#define DAY         31      // ���̉����̐�(1�`31��)
#define WEEK        7       // �j���̉����̐�(���`�y�j��)
#define NOON        2       // �ߑO�E�ߌ�̉����̐�
#define HOUR        24      // ���̉����̐�(0�`23��)
#define MINUTE      60      // ���̉����̐�(0�`59��)
#define TAIL        4       // ����̎��(�c���ˁI�c����!�c�ɂȂ�����!)
#define SEASON      10      // �L������������t�ˑ��̑䎌�̎��
#define SYSTEM      4       // �V�X�e�������̎��
#define ALARM       7       // �A���[���֌W�̉����̎��
#define TIMER       22      // �^�C�}�[�֌W�̉����̎��
#define WORDS       132     // �L�����̑䎌�̎��

// �L�����̑䎌�N���X
class Words {
    int hWord;              // �䎌�����ւ̃n���h��

    // min_hour��min_minute������Amax_hour��max_minute���̊ԁA�䎌�𒝂邱�Ƃ��ł���
    int min_hour;
    int min_minute;
    int max_hour;
    int max_minute;

    bool programmer;        // �v���O���}�p�̑䎌���ǂ���
    eFace face;             // ���̑䎌�ɍ������\��
public:
    Words() {}
    Words(eFace f, int hw);
    Words(int minh, int maxh, bool prg, eFace f, int hw);
    Words(double minh, double maxh, bool prg, eFace f, int hw);
    int GetHandle() { return hWord; }
    eFace GetFace() { return face; }
    // ��������min_hour��min_minute������Amax_hour��max_minute���̊Ԃɂ��邩
    bool IsAppropriate();
};

// �T�E���h�ݒ�ۑ��\����
struct VoiceCfg {
    bool sound;                 // �T�E���hON/OFF
    bool auto_repro;            // �����Đ�ON/OFF
    bool for_prg;               // �v���O���}�p�{�C�X�𗬂����ǂ���
    bool first_time;            // ����N�����ǂ���
    void Check() {}             // �����o���s���Ȓl�̎��C������
};

// �����Ǘ��N���X
class Voice : public IOBase<VoiceCfg> {
    // ���ꂼ�ꉹ���̃n���h�����i�[
    array<int, YEAR>    hYear;
    array<int, MONTH>   hMonth;
    array<int, DAY>     hDay;
    array<int, WEEK>    hWeek;
    array<int, NOON>    hNoon;
    array<int, HOUR>    hHour;
    array<int, HOUR>    hTime_signal;
    array<int, MINUTE>  hMinute;
    array<int, TAIL>    hTail;

    array<int, SEASON>  hSeason;
    array<int, SYSTEM>  hSystem;
    array<int, ALARM>   hAlarm;
    array<int, TIMER>   hTimer;

    typedef unsigned char KeyCode;
    unordered_map<KeyCode, int> keyboard;

    array<Words, WORDS> words;

    int hPlaying;               // �Ō�ɍĐ����������̃n���h��
    eFace face;                 // �Ō�ɍĐ����������ɍ������\��
public:
    Voice();                                    // �����t�@�C���̃��[�h�Ȃ�

    void SoundTime(eTmData data, int index);    // ���Ԋ֘A�̉����Đ�����
    void SoundWords();                          // �L�����̃Z���t���Đ�����
    void SoundSys(int index);                   // �V�X�e���������Đ�����
    void SoundKey(KeyCode key);                 // �L�[��ǂݏグ��
    void SoundAlarm();                          // �ʏ�̃A���[�������Đ�����
    void SoundWakeUp();                         // �ڊo�܂��̃A���[�������Đ�����

    enum eTimerVoice {
        Start1, Start2,
        Last1m, Last30, Last1, Last2, Last3, Last4, Last5, Last6, Last7, Last8, Last9, Last10,
        Over1, Over2, Over3, Over4, Over5, Over6, Over7, Over8
    };
    void SoundTimer(eTimerVoice voice);         // �^�C�}�[�֌W�̉����Đ�����

    bool IsPlaying();                           // ���݉������Đ�����
    bool IsTimerPlaying();                      // ���݃^�C�}�[�֌W�̉������Đ�����
    void Repeat();                              // ���O�ɍĐ�����������������x�Đ�
    void StopSound();                           // ���ݖ��Ă���T�E���h���~�߂�
    enum eCfg { Sound, AutoRepro, ForPrg, FirstTime };
    void SetCfg(eCfg cfg, int value);           // �ݒ��ύX����
    eFace GetFace() { return face; }            // ���Ă���T�E���h�ɂӂ��킵���L�����̕\���Ԃ�
};

extern Voice *pVoice;

#endif // !VOICE_H
