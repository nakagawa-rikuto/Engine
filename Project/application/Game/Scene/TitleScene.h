#pragma once
/// ===Include=== ///
// IScene
#include "application/Game/Scene/IScene.h"
#include "application/2d/Sprite.h"

///=====================================================/// 
/// タイトルシーン
///=====================================================///
class TitleScene : public IScene {
public:/// ===メンバ関数=== ///

	TitleScene() = default;
	~TitleScene();

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

