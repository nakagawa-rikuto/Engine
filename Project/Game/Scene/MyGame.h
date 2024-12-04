#pragma once
/// ===Include=== ///
// Engine
#include "Engine/Scene/Framework.h"
// Scene
#include "Game/Scene/GameScene.h"
#include "Game/Manager/SceneManager.h"

///=====================================================/// 
/// ゲーム全体
///=====================================================///
class MyGame : public Framework {
public:

	MyGame();
	~MyGame();

	// 初期化
	void Initialize(const wchar_t* title) override;
	// 終了
	void Finalize() override;
	// 毎フレーム更新
	void Update() override;
	// 描画
	void Draw() override;

private:/// ====== ///
	// ゲーム終了フラグ
	bool endRequst_ = false;

	/// ===シーン=== ///
	// ゲームシーン
	std::unique_ptr<GameScene> scene_;
	// シーンマネージャ
	std::unique_ptr<SceneManager> sceneManager_;

	
};

