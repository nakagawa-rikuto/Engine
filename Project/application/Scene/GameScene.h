#pragma once
/// ===Includ=== ///
#include "application/Scene/IScene.h"
// Game
#include "application/Game/Entity/Player/Player.h"
#include "application/Game/Camera/Camera.h"

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
	
	/// ===Classの宣言=== /// 
	// Player
	std::unique_ptr<Player> player_;
	// Camera
	std::shared_ptr<Camera> camera_;

	/// ===変数の宣言=== ///
	struct CameraInfo {
		Vector3 translate = { 0.0f, 0.0f, -10.0f };
		Vector3 rotate = { 0.0f, 0.0f, 0.0f };
	};
	CameraInfo cameraInfo_;
};

