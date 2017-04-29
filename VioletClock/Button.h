#ifndef BUTTON_H
#define BUTTON_H

#include "Base.h"
#include <string>
using std::string;

#define PI 3.141592653589793        // �~����

// �ݒ�{�^���֌W�̒萔
#define CFG_X           SCALEX(570)         // �ݒ�{�^�������x���W
#define CFG_Y           SCALEY(400)         // �ݒ�{�^�������y���W
#define CFG_W           SCALEX(40)          // �ݒ�{�^���̕�
#define CFG_H           SCALEY(40)          // �ݒ�{�^���̍���

// ������x�Đ��{�^���֌W�̒萔
#define RPT_X           SCALEX(500)         // ������x�Đ��{�^�������x���W
#define RPT_Y           SCALEY(400)         // ������x�Đ��{�^�������y���W
#define RPT_W           SCALEX(40)          // ������x�Đ��{�^���̕�
#define RPT_H           SCALEY(40)          // ������x�Đ��{�^���̍���

// �~���[�g�{�^���֌W�̒萔
#define MUTE_X          SCALEX(430)         // �~���[�g�{�^�������x���W
#define MUTE_Y          SCALEY(400)         // �~���[�g�{�^�������y���W
#define MUTE_W          SCALEX(40)          // �~���[�g�{�^���̕�
#define MUTE_H          SCALEY(40)          // �~���[�g�{�^���̍���

// �A���[���{�^���֌W�̒萔
#define ALM_X           SCALEX(360)         // �A���[���{�^�������x���W
#define ALM_Y           SCALEY(400)         // �A���[���{�^�������y���W
#define ALM_W           SCALEX(40)          // �A���[���{�^���̕�
#define ALM_H           SCALEY(40)          // �A���[���{�^���̍���

// �^�C�}�[�֌W�̒萔
#define TMR_X           SCALEX(290)         // �^�C�}�[�{�^�������x���W
#define TMR_Y           SCALEY(400)         // �^�C�}�[�{�^�������y���W
#define TMR_W           SCALEX(40)          // �^�C�}�[�{�^���̕�
#define TMR_H           SCALEY(40)          // �^�C�}�[�{�^���̍���
#define TMRSTART_X      SCALEX(400)         // �X�^�[�g�{�^�������x���W
#define TMRSTART_Y      SCALEY(300)         // �X�^�[�g�{�^�������y���W
#define TMRSTART_W      SCALEX(100)         // �X�^�[�g�{�^���̕�
#define TMRRESET_X      SCALEX(140)         // ���Z�b�g�{�^�������x���W
#define TMRRESET_Y      SCALEY(300)         // ���Z�b�g�{�^�������y���W
#define TMRRESET_W      SCALEX(100)         // ���Z�b�g�{�^���̕�
#define TMRCHANGE_X     SCALEX(560)         // �{�C�X�ύX�{�^����x���W
#define TMRCHANGE_Y     SCALEY(388)         // �{�C�X�ύX�{�^����y���W
#define TMRCHANGE_W     SCALEX(48)          // �{�C�X�ύX�{�^���̕�
#define TMRCHANGE_H     SCALEY(20)          // �{�C�X�ύX�{�^���̍���

#define MARGIN          SCALEY(8)           // �{�^���̕����̏㉺�̃X�y�[�X

// ������x�Đ��{�^���N���X
class RptButton : public Area {
    static int hImage;
public:
    RptButton();
    void Draw();
    void Update();
};

// �ݒ�{�^���N���X
class CfgButton : public Area {
    static int hImage;
public:
    CfgButton();
    void Draw();
};

// �~���[�g�{�^���N���X
class MuteButton : public Area {
    static int hSnd_img;
    static int hMute_img;
public:
    MuteButton();
    void Draw();
    void Update();
};

// �A���[���{�^���N���X
class AlmButton : public Area {
    static int hBase;
    static int hHand;
public:
    AlmButton();
    void Draw();
};

// �^�C�}�[�{�^���N���X
class TmrButton : public Area {
    static int hImage;
public:
    TmrButton();
    void Draw();
};

// �^�C�}�[��ʂ̃X�^�[�g/�ꎞ��~�{�^���N���X
class TmrStartButton : public Area {
    int hFont;                  // �\�����镶����̃t�H���g
public:
    TmrStartButton(int hF);
    void Draw();
};

// �^�C�}�[��ʂ̃��Z�b�g�{�^���N���X
class TmrResetButton : public Area {
    int hFont;                  // �\�����镶����̃t�H���g
public:
    TmrResetButton(int hF);
    void Draw();
};

// �^�C�}�[��ʂ̏I���������ύX�{�^��
class TmrVoiceChgButton : public Area {
    int hFont;                  // �\�����镶����̃t�H���g
public:
    TmrVoiceChgButton();
    ~TmrVoiceChgButton();
    void Draw();
};

// �ݒ��ʂ��̑��̃{�^���N���X
class Button : public Area {
    string name;                // �{�^���ɕ\�����镶����
    int str_width;              // �\������镶����̕�
    int hFont;                  // �\�����镶����̃t�H���g
public:
    Button(int x, int y, int w, string str, int hF);
    Button() : Area(0, 0, 0, 0) {}
    void Draw();
};

#endif // !BUTTON_H
