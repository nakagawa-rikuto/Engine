#pragma once
/// ===Includ=== ///
#include "Game/Scene/IScene.h"

#include "Game/3d/Model.h"
#include <list>

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
	
	//Camera
	std::shared_ptr<Camera> camera3D_;
	std::shared_ptr<Camera> camera2D_;

	Vector3 cameraPos = { 0.0f, 0.0f, -10.0f };  // Camera
	Vector3 cameraRotate = { 0.0f, 0.0f, 0.0f }; // Canera
	bool SetCamera = false;

	std::list<std::shared_ptr<Model>> block_;
};

