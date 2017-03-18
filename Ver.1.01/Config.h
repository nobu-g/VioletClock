#ifndef CONFIG_H
#define CONFIG_H

#include "Main.h"
#include "Area.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

// ���W�֌W�̒萔
#define CFGAREA_X		5			// �ݒ�G���A��x���W
#define CFGAREA_Y		37			// �ݒ�G���A��y���W
#define CFGAREA_W		630			// �ݒ�G���A�̕�
#define CFGAREA_H		389			// �ݒ�G���A�̍���
#define COMP_X			370			// �����{�^����x���W
#define COMP_Y			435			// �����{�^����y���W
#define COMP_W			120			// �����{�^���̕�
#define CANCEL_X		500			// �L�����Z���{�^����x���W
#define CANCEL_Y		435			// �L�����Z���{�^����y���W
#define CANCEL_W		120			// �L�����Z���{�^���̕�
#define RESET_X			25			// ����l�ɖ߂��{�^����x���W
#define RESET_Y			435			// ����l�ɖ߂��{�^����y���W
#define RESET_W			150			// ����l�ɖ߂��{�^���̕�

// �f�U�C���֌W�̒萔
#define FSIZE_MAIN		20			// �ݒ��ʎg����v�t�H���g�T�C�Y
#define FSIZE_CHOICE	17			// �I�����{�^���̃t�H���g�T�C�Y
#define MARGIN			8			// �{�^���̕����̏㉺�̃X�y�[�X
#define VIOLET1			0xe5D7F7	// �ݒ�G���A�̔w�i�F
#define VIOLET2			0xD1B2F7	// �ݒ荀�ڃ{�^���w�i�F
#define VIOLET3			0xC399F7	// ��v�{�^���̔w�i�F
#define VIOLET4			0x9442F7	// �J�[�\���̏ꏊ�������g�̐F
/*�F�R�[�h�}�N���͈ȉ��̃T�C�g���Q�l�ɁA�������ɕ���ł���
http://www.color-sample.com/colors/d9bef8/
*/

// �ݒ�f�[�^�ۑ��N���X
class CfgData{
public:
	enum eHourForm { Form12h, Form24h };
	enum eTail { Dane, Dayo, Ninattayo, Datte, None };
	enum eWho { KureiKei };
	enum eData { Sound, AutoRepro, HForm, SoundYear, SoundWeek, ForPrg, TailDate, TailTime, ChrName, FirstTime };
private:
	bool sound;					// �T�E���hON/OFF
	bool auto_repro;			// �����Đ�ON/OFF
	eHourForm h_form;			// ������0-12�`��/0-24�`���̂ǂ���ŕ\�����邩
	bool sound_year;			// �N��ǂݏグ�邩�ǂ���
	bool sound_week;			// �j����ǂݏグ�邩�ǂ���
	bool for_prg;				// �v���O���}�p���ǂ���
	eTail tail_date;			// ���t�ǂݏグ���̌��
	eTail tail_time;			// �����ǂݏグ���̌��
	eWho chr_name;				// �\�������L�����N�^�[(���݂͕��d�̂�)
	bool first_time;			// ����N�����ǂ���
public:
	int GetCfg(eData data);
	void SetCfg(eData data, int value);
	void Default();				// �����o���f�t�H���g�l�ɖ߂�
	void Setup();				// �ݒ�t�@�C�����Ȃ��ꍇ�A�f�t�H���g�ɐݒ�
};

// �ݒ��ʂ̃{�^���N���X
class Button : public Area {
	string name;				// �{�^���ɕ\�����镶����
	int str_width;				// �\�������I�������̕�
public:
	Button(int x, int y, int w, string str);
	void Draw();
};

class Choice;
// �ݒ荀�ڃN���X
class Item : public Area {
public:
	enum eItemType { Type_HForm, Type_AutoRepro, Type_SoundYear, Type_SoundWeek, Type_TailDate, Type_TailTime, Type_ForPrg };
private:
	int x, y;					// ���ږ���\��������W
	eItemType type;				// �ǂ̐ݒ荀�ڂ�
	string name;				// ���ږ�
	vector<Choice *> pChoice;	// �I����
	CfgData &temp;				// ���̐ݒ�f�[�^�ւ̎Q��
public:
	Item(int x0, int y0, string str, eItemType t, CfgData &tmp);
	~Item();
	void Update();
	void Draw();
};

// �ݒ荀�ڂ̑I�����N���X
class Choice : public Area {
	string name;				// �I������
	int str_width;				// �\�������I�������̕�
public:
	Choice(int x, int y, int w, string str);
	void Draw();				// �����y�сA�}�E�X�I�[�o�[�̂Ƃ��̉e��\��
	void DrawShade();			// �I������Ă���Ƃ��̉e��\��
};

// �t�H���g�f�[�^����邽�߂̃N���X
class FontMaker {
public:
	FontMaker();
};

// �ݒ��ʃN���X
class Config : public BaseScene, public FontMaker {
	CfgData temp;				// �ύX�f�[�^���ꎞ�ۑ�

	Item h_form;
	Item auto_repro;
	Item sound_year;
	Item sound_week;
	Item tail_date;
	Item tail_time;
	Item for_prg;

	Button Reset;				// ����l�ɖ߂��{�^��
	Button Complete;			// �ݒ芮���{�^��
	Button Cancel;				// �L�����Z���{�^��
public:
	Config(ISceneChanger *changer);
	~Config();
	void Initialize();
	void Finalize() {}
	void Update();
	void Draw();
};

extern CfgData *pCfgData;

#endif
