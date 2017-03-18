#ifndef CONFIG_H
#define CONFIG_H

#include "Main.h"
#include "Voice.h"
#include "Clock.h"
#include "Button.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

// �{�^���̍��W�֌W�̒萔
#define COMP_X              370         // �����{�^����x���W
#define COMP_Y              435         // �����{�^����y���W
#define COMP_W              120         // �����{�^���̕�
#define CANCEL_X            500         // �L�����Z���{�^����x���W
#define CANCEL_Y            435         // �L�����Z���{�^����y���W
#define CANCEL_W            120         // �L�����Z���{�^���̕�
#define RESET_X             25          // ����l�ɖ߂��{�^����x���W
#define RESET_Y             435         // ����l�ɖ߂��{�^����y���W
#define RESET_W             150         // ����l�ɖ߂��{�^���̕�

// �I�����ڂ̍��W�֌W�̒萔
#define HFORM_X             (MAINAREA_X + 15)
#define HFORM_Y             (MAINAREA_Y + 15)
#define AUTOREPRO_X         (MAINAREA_X + 15)
#define AUTOREPRO_Y         (MAINAREA_Y + 109)
#define SOUNDYEAR_X         (MAINAREA_X + 15)
#define SOUNDYEAR_Y         (MAINAREA_Y + 203)
#define SOUNDWEEK_X         (MAINAREA_X + 15)
#define SOUNDWEEK_Y         (MAINAREA_Y + 297)
#define TAILDATE_X          (MAINAREA_X + MAINAREA_W/2 + 15)
#define TAILDATE_Y          (MAINAREA_Y + 15)
#define TAILTIME_X          (MAINAREA_X + MAINAREA_W/2 + 15)
#define TAILTIME_Y          (MAINAREA_Y + 165)
#define FORPRG_X            (MAINAREA_X + MAINAREA_W/2 + 15)
#define FORPRG_Y            (MAINAREA_Y + 315)

// �ݒ��ʓ��̍�Ɨ̈�(�F�������G���A)�֌W�̒萔
#define MAINAREA_X          5           // ��Ɨ̈��x���W
#define MAINAREA_Y          37          // ��Ɨ̈��y���W
#define MAINAREA_W          630         // ��Ɨ̈�̕�
#define MAINAREA_H          389         // ��Ɨ̈�̍���

// �f�U�C���֌W�̒萔
#define FSIZE_CHOICE        17          // �I�����{�^���̃t�H���g�T�C�Y
#define FSIZE_MAIN          20          // �ݒ��ʓ��Ŏg����v�t�H���g�T�C�Y

// �a�����ݒ�֌W�̒萔
#define BIRTHDAY_X          95          // �\�����ꂽ�a������x���W
#define BIRTHDAY_Y          181         // �\�����ꂽ�a������y���W
#define FSIZE_BD            100         // �a�����̃t�H���g�T�C�Y

// �ݒ�f�[�^�ꎞ�ۑ��N���X
class CfgData {
public:
    VoiceCfg vcfg;
    DateCfg dcfg;
    TimeCfg tcfg;

    CfgData();                  // ���݂̐ݒ��ǂݍ���
    void Default();             // (�a�����ȊO��)�����o���f�t�H���g�l�ɖ߂�
    void DefaultBd();           // �a�������f�t�H���g�l�ɖ߂�
};

template <class DataType> class Choice;
// �ݒ荀�ڃN���X
template <class DataType>
class Item : public Area {
    int x, y;                               // ���ږ���\��������W
    string name;                            // ���ږ�
    DataType &data;                         // ���̍��ڂ��Q�Ƃ���ݒ�f�[�^
    vector<Choice<DataType> *> pChoice;     // �I����
public:
    Item(int x0, int y0, int w, int h, string str, DataType &ref);
    ~Item();
    void PushBack(Choice<DataType> *pCh);
    void Update();
    void Draw();
};

// �ݒ荀�ڂ̑I�����N���X
template <class DataType>
class Choice : public Area {
    string name;                    // �I������(ON/OFF�Ȃ�)
    int str_width;                  // �\�������I�������̕�
    DataType value;                 // �I�����ŗL�̒l
public:
    Choice(int x, int y, int w, string str, DataType val);
    void Set(DataType &data);       // data��lvalue�ɐݒ肷��                  
    void Draw(DataType &data);      // �I�����̕�����}�[�J�[��\���v
};

// ���������N���X
class ConfigTime : public TimeSrc {
    enum eTimeCfgStatus { Invariable, HVariable, MinVariable, YVariable, MonVariable, DVariable };
    eTimeCfgStatus status;
    int changing;       // ���l�ύX����30�t���[���𐔂���
public:
    ConfigTime();
    void Update();
    void Draw();
    void ResetStatus() { status = Invariable; }
};

// �a�����ݒ�N���X
class SetBirthday : public Area {
    DateCfg &dcfg;

    enum eDateCfgStatus { Invariable, MVariable, DVariable };
    eDateCfgStatus status;
    int changing;       // ���l�ύX����30�t���[��������
public:
    SetBirthday(DateCfg &ref);
    void Update();
    void Draw();
    void ResetStatus() { status = Invariable; }
};

// �t�H���g�f�[�^����邽�߂̃N���X
class FontMaker {
public:
    FontMaker();
};

// �ݒ��ʃN���X
class Config : public BaseScene, public FontMaker {
    enum eTab { Tab_MainCfg, Tab_TimeAdj, Tab_SetBirthday };
    eTab tab;                   // ���ݕ\�����Ă���^�u
    CfgData temp;               // �ύX�f�[�^���ꎞ�ۑ�

    ConfigTime cfg_time;        // �����ݒ�p�I�u�W�F�N�g
    SetBirthday set_bd;         // �a�����ݒ�p�I�u�W�F�N�g

    Item<eHourForm> h_form;
    Item<bool> auto_repro;
    Item<bool> sound_year;
    Item<bool> sound_week;
    Item<eTail> tail_date;
    Item<eTail> tail_time;
    Item<bool> for_prg;

    Button reset;               // ����l�ɖ߂��{�^��
    Button complete;            // �ݒ芮���{�^��
    Button cancel;              // �L�����Z���{�^��
public:
    Config(ISceneChanger *changer);
    ~Config();
    void Initialize();
    void Finalize() {}
    void Update();
    void Draw();
    void Reflect();             // temp�����ݒ�𔽉f������
};

#endif // !CONFIG_H
