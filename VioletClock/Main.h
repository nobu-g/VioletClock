#ifndef MAIN_H
#define MAIN_H

#include "Base.h"

// �V�[���Ǘ��N���X
class SceneMgr : public ISceneChanger {
    BaseScene *pScene;      // �V�[���Ǘ��ϐ�
    eScene next_scene;      // ���̃V�[���Ǘ��ϐ�
public:
    SceneMgr();
    void Initialize();
    void Finalize();
    void Update();
    void Draw() { pScene->Draw(); }

    void ChangeScene(eScene next) { next_scene = next;  /* ���̃V�[�����Z�b�g���� */ }
};

// ���C���N���X
class Main {
public:
    void Init();
    void Run();
    void End();
};

#endif
