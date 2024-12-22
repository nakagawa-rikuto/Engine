#pragma once
/// ===Includ=== ///
#include "Game/Scene/IScene.h"
#include "Command.h"
#include "Game/2d/Sprite.h"
#include <memory>

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
	
	Object object_;
	std::unique_ptr<LeftCommand> leftCommand_;
	std::unique_ptr<RightCommand> rightCommand_;

	std::shared_ptr<Sprite> box_;
};

