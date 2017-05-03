#ifndef AREA_H
#define AREA_H

#include <fstream>
using std::ofstream;
using std::ifstream;

// DPI�ɉ����ăp�[�c���X�P�[�����O
extern double scaleX;
extern double scaleY;
#define SCALEX(argX)    ((int) ((argX) * scaleX))
#define SCALEY(argY)    ((int) ((argY) * scaleY))

// �t�H���g�T�C�Y�����p������
#define STOW(arg)       ((int) (((arg) + 1) / 2))

// �F�w��
#define BLACK               0x000000    // �����F
#define VIOLET1             0xE5D7F7    // �ݒ�G���A�̔w�i�F(229, 215, 247)
#define VIOLET2             0xD1B2F7    // �ݒ荀�ڃ{�^���w�i�F
#define VIOLET3             0xC399F7    // ��v�{�^���̔w�i�F
#define VIOLET4             0x9442F7    // �J�[�\���̏ꏊ�������g�̐F
/*�F�R�[�h�}�N���͈ȉ��̃T�C�g���Q�l�ɁA�������ɕ���ł���
http://www.color-sample.com/colors/d9bef8/
*/

enum eScene {
    eScene_Clock,       // ���C�����
    eScene_Config,      // �ݒ���
    eScene_Alarm,       // �A���[���ݒ���
    eScene_Timer,       // �^�C�}�[���
    eScene_None         // ����
};

// �V�[����ύX���邽�߂̃C���^�[�t�F�C�X�N���X
class ISceneChanger {
public:
    virtual void ChangeScene(eScene next) = 0;
};

// �V�[���̊��N���X
class BaseScene {
protected:
    // �N���X���L���ɃV�[���؂�ւ���`����C���^�[�t�F�C�X
    ISceneChanger *pScene_changer;
public:
    BaseScene(ISceneChanger *pChanger) { pScene_changer = pChanger; }
    virtual ~BaseScene() {}
    virtual void Initialize() {}
    virtual void Finalize() {}
    virtual void Update() {}
    virtual void Draw() {}
};

// ��ʂ̍\���v�f�̊�{�N���X
class Area {
    bool is_clicking;               // �G���A���ŃN���b�N����A�������̏�Ԃ��ǂ���
protected:
    int x1, y1, x2, y2;             // �I�u�W�F�N�g�̍��W
    static bool pre_lclick_status;  // �O���[�v�ō��N���b�N����Ă������ǂ���
    static bool pre_hitkey_status;  // �O���[�v�ŃL�[�{�[�h��������Ă������ǂ���
public:
    Area(int x, int y, int w = 0, int h = 0)
    {
        x1 = x; y1 = y; x2 = x + w; y2 = y + h;
    }
    void SetWidth(int w) { x2 = x1 + w; }       // ����w�ƂȂ�悤x2��ύX
    void SetHeight(int h) { y2 = y1 + h; }      // ������h�ƂȂ�悤y2��ύX
    static void Update();                       // �N���b�N��Ԃ̍X�V�D1���[�v��1�񂵂����s���Ă͂����Ȃ�
    bool IsMouseOver();                         // �I�u�W�F�N�g��Ƀ}�E�X�J�[�\�������邩
    bool IsClicked();                           // �I�u�W�F�N�g��ō��N���b�N�����ꂽ�u�Ԃ̂�true��Ԃ�
    bool IsClicking();                          // �G���A���ŃN���b�N����A�������̏�Ԃ��ǂ���
    friend class Alarm;                         // Alarm�N���X���ł�pre_lclick_status���g����悤��
    friend class ConfigTime;                    // ConfigTime�N���X���ł�pre_hitkey_status���g����悤��
};

// �}���q�ƒ��o�q���e���v���[�g�֐��Ƃ��Đ錾����
template<class CfgType> class IOBase;
template<class CfgType>
ofstream& operator << (ofstream &stream, const IOBase<CfgType> &rhs);
template<class CfgType>
ifstream& operator >> (ifstream &stream, IOBase<CfgType> &rhs);

// �t�@�C�����o�͂̂��߂̑}���q�E���o�q��񋟂����{�N���X
template<class CfgType> class IOBase {
protected:
    CfgType config;
public:
    friend ofstream& operator << <CfgType> (ofstream &stream, const IOBase<CfgType> &rhs);
    friend ifstream& operator >> <CfgType> (ifstream&, IOBase<CfgType>&);
    CfgType GetCfg() { return config; }                                 // �ݒ��Ԃ�
};

template<class CfgType>
ofstream &operator << (ofstream& stream, const IOBase<CfgType>& rhs)
{
    stream.write((char *)&rhs.config, sizeof(CfgType));

    if (stream.bad())
        throw Exception(Exception::OutputError, "ios::badbit");
    else if (stream.fail()) {
        Exception(Exception::OutputError, "ios::failbit").ExportError();
        stream.clear();
    }

    return stream;
}

template<class CfgType>
ifstream &operator >> (ifstream& stream, IOBase<CfgType>& rhs)
{
    stream.read((char *)&rhs.config, sizeof(CfgType));
    rhs.config.Check();     // �ǂݍ��񂾒l���K�����`�F�b�N�y�яC������

    if (stream.bad())
        throw Exception(Exception::InputError, "ios::badbit");
    else if (stream.fail()) {
        Exception(Exception::InputError, "ios::failbit").ExportError();
        stream.clear();
    }

    return stream;
}

#endif // !AREA_H
