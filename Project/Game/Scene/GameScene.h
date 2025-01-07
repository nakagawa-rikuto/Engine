#pragma once
/// ===Includ=== ///
#include "Game/Scene/IScene.h"

#include "Game/Stage/Block.h"
#include "Game/Entity/Player.h"

#include <vector>

///=====================================================/// 
/// ゲームモード
///=====================================================///
enum GameMode {
	
};

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
	
	/// ===Camera=== ///
	// Camera
	std::shared_ptr<Camera> camera_;
	// Camera情報
	Vector3 cameraPos_;
	Vector3 cameraRotate_;
	Vector3 cameraScale_;
	bool SetCamera = true;

	/// ===Plaeyr=== ///
	std::unique_ptr<Player> player_;

	/// ===Block=== ///
	std::vector<std::shared_ptr<Block>> blocks_;

private:

	/// ===当たり判定=== ///
	void IsCollisison2D();
	void IsCollisison3D();
};

