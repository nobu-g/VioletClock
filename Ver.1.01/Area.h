#ifndef SCREEN_H
#define SCREEN_H

#include <array>

// �L�����N�^�֌W�̒萔
#define CHR_X		10		// �L�����N�^�����x���W
#define CHR_Y		170		// �L�����N�^�����y���W
#define CHR_W		122		// �L�����N�^�̕�
#define CHR_H		268		// �L�����N�^�̍���
#define CHR_NUM		1		// �L�����N�^�̎��

// �o�b�e���[�֌W�̒萔
#define BAT_LOW		20		// �o�b�e���[���������Ƃ݂Ȃ��c��(%)
#define BAT_CRISIS	10		// �o�b�e���[���E�Ƃ݂Ȃ��c��(%)

// �L�[�{�[�h�֌W�̒萔
#define KEY_INPUT_WIN	(0xDA)
#define KEY_INPUT_CTRL	(0x5D)
#define KEY_INPUT_ALT	(0x78)
#define KEY_INPUT_SHIFT	(0x29)

// ������x�Đ��{�^���֌W�̒萔
#define RPT_X		500		// ������x�Đ��{�^�������x���W
#define RPT_Y		400		// ������x�Đ��{�^�������y���W
#define RPT_W		40		// ������x�Đ��{�^���̕�
#define RPT_H		40	 	// ������x�Đ��{�^���̍���

// �ݒ�{�^���֌W�̒萔
#define CFG_X		570		// �ݒ�{�^�������x���W
#define CFG_Y		400		// �ݒ�{�^�������y���W
#define CFG_W		40		// �ݒ�{�^���̕�
#define CFG_H		40		// �ݒ�{�^���̍���

// �~���[�g�{�^���֌W�̒萔
#define MUTE_X		430		// �~���[�g�{�^�������x���W
#define MUTE_Y		400		// �~���[�g�{�^�������y���W
#define MUTE_W		40		// �~���[�g�{�^���̕�
#define MUTE_H		40		// �~���[�g�{�^���̍���

// ���C����ʂ̍\���v�f�̊�{�N���X
class Area {
	bool is_clicking;				// �G���A���ŃN���b�N����A�������̏�Ԃ��ǂ���
protected:
	int x1, y1, x2, y2;				// �I�u�W�F�N�g�̍��W
	static bool pre_lclick_status;	// �O���[�v�ō��N���b�N����Ă������ǂ���
public:
	Area(int x, int y, int w = 0, int h = 0)
	{
		x1 = x; y1 = y; x2 = x + w; y2 = y + h;
	}
	void SetWidth(int w) { x2 = x1 + w; }		// ����w�ƂȂ�悤x2��ύX
	void SetHeight(int h) { y2 = y1 + h; }		// ������h�ƂȂ�悤y2��ύX
	static void Update();						// �N���b�N��Ԃ̍X�V�D1���[�v��1�񂵂����s���Ă͂����Ȃ�
	bool IsMouseOver();							// �I�u�W�F�N�g��Ƀ}�E�X�J�[�\�������邩
	bool IsClicked();							// �I�u�W�F�N�g��ō��N���b�N�����ꂽ�u�Ԃ̂�true��Ԃ�
	bool IsClicking();							// �G���A���ŃN���b�N����A�������̏�Ԃ��ǂ���
};

// �L�����N�^�N���X
class Chr : public Area {
	std::array<int, 25> hChr;
public:
	Chr();
	void Draw();
	void Action();			// �N���b�N���ꂽ�Ƃ��̓���
};

// ������x�Đ��{�^���N���X
class RptButton : public Area {
	int hImage;
public:
	RptButton();
	void Draw();
	void Action();
};

// �ݒ�{�^���N���X
class CfgButton : public Area {
	int hImage;
public:
	CfgButton();
	void Draw();
};

// �~���[�g�{�^���N���X
class MuteButton : public Area {
	int hSnd_img;
	int hMute_img;
public:
	MuteButton();
	void Draw();
	void Action();			// �N���b�N���ꂽ�Ƃ��̓���
};

extern Chr *pChr;
extern RptButton *pRptButton;
extern CfgButton *pCfgButton;
extern MuteButton *pMuteButton;

#endif
