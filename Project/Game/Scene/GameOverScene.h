#pragma once
/// ===Include=== ///
// IScene
#include "Game/Scene/IScene.h"
#include "Game/2d/Sprite.h"

///=====================================================/// 
/// ゲームオーバーシーン
///=====================================================///
class GameOverScene : public IScene {
public:/// ===メンバ関数=== ///

	GameOverScene() = default;
	~GameOverScene();

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
	
	// スプライト
	std::unique_ptr<Sprite> sprite_;
};

