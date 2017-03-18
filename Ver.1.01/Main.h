#ifndef MAIN_H
#define MAIN_H

enum eScene {
	eScene_Clock,		// メイン画面
	eScene_Config,		// 設定画面
	eScene_None			// 無し
};

// シーンを変更するためのインターフェイスクラス
class ISceneChanger {
public:
	virtual void ChangeScene(eScene next) = 0;
};

// シーンの基底クラス
class BaseScene {
protected:
	// クラス所有元にシーン切り替えを伝えるインターフェイス
	ISceneChanger *pScene_changer;
public:
	BaseScene(ISceneChanger *pChanger){ pScene_changer = pChanger; }
	virtual ~BaseScene(){}
	virtual void Initialize() {}
	virtual void Finalize() {}
	virtual void Update() {}
	virtual void Draw() {}
};

// シーン管理クラス
class SceneMgr : public ISceneChanger {
	BaseScene *pScene;		// シーン管理変数
	eScene next_scene;		// 次のシーン管理変数
public:
	SceneMgr();
	void Initialize();
	void Finalize();
	void Update();
	void Draw() { pScene->Draw(); }

	void ChangeScene(eScene next) { next_scene = next;	/* 次のシーンをセットする */ }
};

// メインクラス
class Main {
public:
	void Init();
	void Run();
	void End();
};

#endif
