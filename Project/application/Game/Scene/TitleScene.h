#pragma once
/// ===Include=== ///
// IScene
#include "application/3d/Camera.h"
#include "application/2d/Sprite.h"
#include "application/Game/Scene/IScene.h"
#include "application/Game/Fade/Fade.h"

#include <memory>


///=====================================================///
/// タイトルシーン
///=====================================================///
class TitleScene : public IScene {
public: /// ===メンバ関数=== ///
	TitleScene() = default;
	~TitleScene();

	// ゲームのフェーズ
	enum class Phase {
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;

private: /// ===メンバ変数=== ///
	/// <summary>
	/// シーン用
	/// </summary>
	
	/// ===Camera=== ///
	std::shared_ptr<Camera> camera_;
	// Camera情報
	Vector3 cameraPos_;
	Vector3 cameraRotate_;
	Vector3 cameraScale_;

	// タイトル文字スプライト
	std::unique_ptr<Sprite> titleSprite_ = nullptr;

	// フェード
	std::unique_ptr<Fade> fade_ = nullptr;
	// 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

	// フェードタイマー
	const float fadeTimer_ = 1.0f;
};