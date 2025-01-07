#pragma once
/// ===Includ=== ///
#include "Game/Scene/IScene.h"
#include "Game/2d/Sprite.h"

#include "Game/Stage/Block.h"
#include "Game/Stage/Goal.h"
#include "Game/Entity/Player.h"

#include <vector>

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

	/// ===モード=== ///
	bool Mode3D_ = true;

	/// ===Plaeyr=== ///
	std::unique_ptr<Player> player_;

	/// ===Block=== ///
	std::vector<std::shared_ptr<Block>> blocks_;

	/// ===Goal=== ///
	std::unique_ptr<Goal> goal_;

	/// ===Sprite=== ///
	std::unique_ptr<Sprite> sprite_;

private:

	/// ===当たり判定=== ///
	void IsCollisison2D();
	void IsCollisison3D();

	/// ===モード変更時の座標変換=== ///
	void SwithcTo3DMode();
	
	/// ===ゴールのBlockとの当たり判定=== ///
	bool IsCollisionGoalBlock();
};

