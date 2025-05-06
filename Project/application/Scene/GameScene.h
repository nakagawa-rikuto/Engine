#pragma once
/// ===Includ=== ///
#include "application/Scene/IScene.h"

// application
#include "application/Drawing/3d/Camera.h"

// Game
#include "application/Game/Entity/Player/Player.h"

///=====================================================/// 
/// ゲームシーン
///=====================================================///
class GameScene : public IScene {
public:/// ===メンバ関数=== ///

	GameScene() = default;
	~GameScene();

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;

private:/// ===メンバ変数=== ///
	/// <summary>
	/// シーン用
	/// </summary>
	

	// Player
	std::unique_ptr<Player> player_; 

	// Camera
	std::shared_ptr<Camera> camera_;
};

