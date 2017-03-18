#ifndef MAIN_H
#define MAIN_H

#include "Base.h"

// シーン管理クラス
class SceneMgr : public ISceneChanger {
    BaseScene *pScene;      // シーン管理変数
    eScene next_scene;      // 次のシーン管理変数
public:
    SceneMgr();
    void Initialize();
    void Finalize();
    void Update();
    void Draw() { pScene->Draw(); }

    void ChangeScene(eScene next) { next_scene = next;  /* 次のシーンをセットする */ }
};

// メインクラス
class Main {
public:
    void Init();
    void Run();
    void End();
};

#endif // !MAIN_H
