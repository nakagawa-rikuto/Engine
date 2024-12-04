#pragma once
/// ===Include=== ///
// Engine
#include "Engine/Scene/Framework.h"
#include "Engine/Core/System.h"
// Game
#include "Game/2d/Sprite.h"
#include "Game/3d/Model.h"
#include "Game/3d/Camera.h"
#include "Game/Manager/CameraManager.h"

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

	/// ===クラス=== ///
	// CameraManager
	//NOTE:後々しーんManager等に移動
	std::unique_ptr<CameraManager> cameraManager_;
	// スプライト
	std::unique_ptr<Sprite> sprite;
	//Camera
	std::shared_ptr<Camera> camera;
	// モデル
	std::unique_ptr<Model> model;

	/// ===変数=== ///
	Vector3 rotate = { 0.0f, 0.0f, 0.0f };       // model
	bool isRotate = false;                       // model
	Vector2 size = { 100.0f, 100.0f }; 
	// sprite
	Vector3 cameraPos = { 0.0f, 0.0f, -10.0f };  // Camera
	Vector3 cameraRotate = { 0.0f, 0.0f, 0.0f }; // Canera
	bool SetCamera = false;

	bool playAudio = false;
	float volume = 1.0f;
	float pitch = 1.0f;
};

