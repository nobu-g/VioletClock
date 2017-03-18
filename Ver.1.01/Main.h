#ifndef MAIN_H
#define MAIN_H

enum eScene {
	eScene_Clock,		// ���C�����
	eScene_Config,		// �ݒ���
	eScene_None			// ����
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
	BaseScene(ISceneChanger *pChanger){ pScene_changer = pChanger; }
	virtual ~BaseScene(){}
	virtual void Initialize() {}
	virtual void Finalize() {}
	virtual void Update() {}
	virtual void Draw() {}
};

// �V�[���Ǘ��N���X
class SceneMgr : public ISceneChanger {
	BaseScene *pScene;		// �V�[���Ǘ��ϐ�
	eScene next_scene;		// ���̃V�[���Ǘ��ϐ�
public:
	SceneMgr();
	void Initialize();
	void Finalize();
	void Update();
	void Draw() { pScene->Draw(); }

	void ChangeScene(eScene next) { next_scene = next;	/* ���̃V�[�����Z�b�g���� */ }
};

// ���C���N���X
class Main {
public:
	void Init();
	void Run();
	void End();
};

#endif
