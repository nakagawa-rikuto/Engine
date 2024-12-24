#pragma once
/// ===Includ=== ///
#include "Game/Scene/IScene.h"

// Game
#include "Game/Entity/Player.h"
#include "Game/Entity/PlayerBullet.h"
#include "Game/Entity/Enemy.h"

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
	
	std::unique_ptr<Player> player_;
	std::unique_ptr<Enemy> enemy_;
};

