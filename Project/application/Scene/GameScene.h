#pragma once
/// ===Includ=== ///
#include "Engine/Scene/IScene.h"
// Animation
#include "Engine/Debug/DebugAnimationModel.h"
// SkyBox
#include "Engine/Graphics/3d/SkyBox/SkyBox.h"

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
	
	/// ===クラス=== ///
	// DebugAnimationModel
	std::unique_ptr<DebugAnimationModel> debugAnimationModel_;
	// SkyBox
	std::unique_ptr<SkyBox> skyBox_;
	// Camera
	std::shared_ptr<Camera> camera_;
};

