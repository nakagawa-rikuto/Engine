#pragma once
/// ===Include=== ///
// IScene
#include "application/Game/Scene/IScene.h"

#include "application/2d/Sprite.h"

///=====================================================///
/// セレクトシーン
///=====================================================///
class SelectScene : public IScene {
public: /// ===メンバ関数=== ///
	SelectScene() = default;
	~SelectScene();

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
};