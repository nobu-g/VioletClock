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

// �f�U�C���֌W�̒萔
#define FSIZE_CHOICE        SCALEY(17)          // �I�����{�^���̃t�H���g�T�C�Y
#define FSIZE_MAIN          SCALEY(20)          // �ݒ��ʓ��Ŏg����v�t�H���g�T�C�Y

// �{�^���̍��W�֌W�̒萔
#define COMP_X              SCALEX(370)         // �����{�^����x���W
#define COMP_Y              SCALEY(435)         // �����{�^����y���W
#define COMP_W              SCALEX(120)         // �����{�^���̕�
#define CANCEL_X            SCALEX(500)         // �L�����Z���{�^����x���W
#define CANCEL_Y            SCALEY(435)         // �L�����Z���{�^����y���W
#define CANCEL_W            SCALEX(120)         // �L�����Z���{�^���̕�
#define RESET_X             SCALEX(25)          // ����l�ɖ߂��{�^����x���W
#define RESET_Y             SCALEY(435)         // ����l�ɖ߂��{�^����y���W
#define RESET_W             SCALEX(150)         // ����l�ɖ߂��{�^���̕�

// �ݒ��ʓ��̍�Ɨ̈�(�F�������G���A)�֌W�̒萔
#define MAINAREA_X          SCALEX(5)           // ��Ɨ̈��x���W
#define MAINAREA_Y          SCALEY(37)          // ��Ɨ̈��y���W
#define MAINAREA_W          SCALEX(630)         // ��Ɨ̈�̕�
#define MAINAREA_H          SCALEY(389)         // ��Ɨ̈�̍���

// �I�����ڂ̍��W�֌W�̒萔
#define HFORM_X             (MAINAREA_X + SCALEX(15))
#define HFORM_Y             (MAINAREA_Y + SCALEY(15))
#define HFORM_W             SCALEX(280)
#define HFORM_H             SCALEY(65)
#define AUTOREPRO_X         (MAINAREA_X + SCALEX(15))
#define AUTOREPRO_Y         (MAINAREA_Y + SCALEY(109))
#define AUTOREPRO_W         SCALEX(280)
#define AUTOREPRO_H         SCALEY(65)
#define SOUNDYEAR_X         (MAINAREA_X + SCALEX(15))
#define SOUNDYEAR_Y         (MAINAREA_Y + SCALEY(203))
#define SOUNDYEAR_W         SCALEX(280)
#define SOUNDYEAR_H         SCALEY(65)
#define SOUNDWEEK_X         (MAINAREA_X + SCALEX(15))
#define SOUNDWEEK_Y         (MAINAREA_Y + SCALEY(297))
#define SOUNDWEEK_W         SCALEX(280)
#define SOUNDWEEK_H         SCALEY(65)
#define TAILDATE_X          (MAINAREA_X + MAINAREA_W/2 + SCALEX(15))
#define TAILDATE_Y          (MAINAREA_Y + SCALEY(15))
#define TAILDATE_W          SCALEX(280)
#define TAILDATE_H          SCALEY(130)
#define TAILTIME_X          (MAINAREA_X + MAINAREA_W/2 + SCALEX(15))
#define TAILTIME_Y          (MAINAREA_Y + SCALEY(165))
#define TAILTIME_W          SCALEX(280)
#define TAILTIME_H          SCALEY(130)
#define FORPRG_X            (MAINAREA_X + MAINAREA_W/2 + SCALEX(15))
#define FORPRG_Y            (MAINAREA_Y + SCALEY(315))
#define FORPRG_W            SCALEX(280)
#define FORPRG_H            SCALEY(57)

// �^�u�֌W�̒萔
#define TAB_X               MAINAREA_X
#define TAB_Y               SCALEY(7)
#define TAB_H               (MAINAREA_Y - TAB_Y)
#define TAB_SPACE           SCALEX(16)
#define TAB_MAINCFG_X       TAB_X
#define TAB_MAINCFG_W       (TAB_SPACE * 2 + FSIZE_MAIN * 2)
#define TAB_TIMEADG_X       (TAB_MAINCFG_X + TAB_MAINCFG_W)
#define TAB_TIMEADG_W       (TAB_SPACE * 2 + FSIZE_MAIN * 4)
#define TAB_SETBIRTHDAY_X   (TAB_TIMEADG_X + TAB_TIMEADG_W)
#define TAB_SETBIRTHDAY_W   (TAB_SPACE * 2 + FSIZE_MAIN * 5)
#define TAB_STRING_Y        SCALEY(14)

// �a�����ݒ�֌W�̒萔
#define BIRTHDAY_X          SCALEX(95)          // �\�����ꂽ�a������x���W
#define BIRTHDAY_Y          SCALEY(181)         // �\�����ꂽ�a������y���W
#define FSIZE_BD            SCALEY(100)         // �a�����̃t�H���g�T�C�Y

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
