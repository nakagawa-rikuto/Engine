#pragma once
/// ===Includ=== ///
#include "application/Game/Scene/IScene.h"
// Game
#include "application/2d/Sprite.h"
#include "application/3d/Model.h"
#include "application/3d/Camera.h"

///=====================================================/// 
/// ゲームシーン
///=====================================================///
class DebugScene : public IScene {
public:/// ===メンバ関数=== ///

	DebugScene() = default;
	~DebugScene();

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

	Vector3 axis_;
	float angle_;
	Matrix4x4 rotateMatrix_;
};

