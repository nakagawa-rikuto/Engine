#pragma once
/// ===Includ=== ///
#include "application/Game/Scene/IScene.h"
#include "application/3d/Model.h"

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

	// モデル
	std::unique_ptr<Model> model_;
	/// ===変数=== ///
	// モデル
	Vector3 modelTranslate_ = { 0.0f, 0.0f, 0.0f };
	Vector3 modelRotate_ = { 0.0f, 0.0f, 0.0f };
	Vector3 modelScale_ = { 1.0f, 1.0f, 1.0f };
	Vector4 modelColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	// ライト
	Vector3 lightDirection_ = { 0.0f, 1.0f, 0.0f };
	float lightIntensity_ = 1.0f;
	Vector4 lightColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	float lightShininess_ = 48.5f;
	
};

