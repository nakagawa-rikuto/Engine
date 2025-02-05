#pragma once
/// ===Include=== ///
// IScene
#include "application/Game/Scene/IScene.h"

#include "application/2d/Sprite.h"
#include "application/Game/Fade/Fade.h"

///=====================================================///
/// セレクトシーン
///=====================================================///
class SelectScene : public IScene {
public: /// ===メンバ関数=== ///
	SelectScene() = default;
	~SelectScene();

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

private:
	// スプライトの当たり判定チェック関数
	void CheckMouseCollision();

private: /// ===メンバ変数=== ///
	/// <summary>
	/// シーン用
	/// </summary>

	std::unique_ptr<Sprite> stageSprite_[11];
	std::unique_ptr<Sprite> bgSprite_;

	// マウス座標
	Vector2 mousePosition_;

	// マウスクリック検出用
	bool isMousePressed_ = false;

	// フェード
	std::unique_ptr<Fade> fade_ = nullptr;
	// 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

	// フェードタイマー
	const float fadeTimer_ = 1.0f;
};