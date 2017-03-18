#ifndef CHARACTER_H
#define CHARACTER_H

#include "Base.h"
#include <map>
using std::map;

// �L�����N�^�֌W�̒萔
#define CHR_X               10          // �L�����N�^�����x���W
#define CHR_Y               148         // �L�����N�^�����y���W
#define CHR_W               120         // �L�����N�^�̕�
#define CHR_H               290         // �L�����N�^�̍���

// �o�b�e���[�֌W�̒萔
#define BAT_LOW             20          // �o�b�e���[���������Ƃ݂Ȃ��c��(%)
#define BAT_CRISIS          10          // �o�b�e���[���E�Ƃ݂Ȃ��c��(%)

// �L�[�{�[�h�֌W�̒萔
#define KEY_INPUT_WIN       (0xDA)      // ���EWindows�L�[
#define KEY_INPUT_CTRL      (0x5D)      // ���EControl�L�[
#define KEY_INPUT_ALT       (0x78)      // ���EAlt�L�[
#define KEY_INPUT_SHIFT     (0x29)      // ���EShift�L�[

// �L�����̕\��
enum eFace { Confident, Doya, Excited, Kyoton, Laugh, Normal, Shocked, Speaking, Tearful, Wink, Blink };

// �L�����N�^�N���X
class Chr : public Area {
    static int hBase;                   // �x�[�X�摜�ւ̃n���h��
    static map<eFace, int> hFace;       // �\��̉摜�ւ̃n���h��
public:
    Chr();
    void Draw();
    void Update();
};

extern Chr *pChr;

#endif // !CHARACTER_H
