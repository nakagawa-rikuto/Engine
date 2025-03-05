#pragma once
/// ===Includ=== ///
#include "application/Scene/IScene.h"
// application
#include "application/Drawing/3d/Camera.h"
// Game
#include "application/Game/Entity/Player/Player.h"
#include "application/Game/Field/Ground.h"
#include "application/Game/Field/SkyDome.h"

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
	 
	/// ===クラス=== ///
	std::shared_ptr<Camera> FPSCamera_; // FPSカメラ
	std::shared_ptr<Camera> fixedPointCamera_; // 定点カメラ

	std::unique_ptr<Player> player_;
	std::unique_ptr<Ground> ground_;
	std::unique_ptr<SkyDome> skyDome_;

	bool SetFPSCamera_ = true;
};

