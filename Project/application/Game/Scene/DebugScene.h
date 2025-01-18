#pragma once
/// ===Includ=== ///
#include "application/Game/Scene/IScene.h"
// Game
#include "MT4/MT4Math.h"
// c++
#include <memory>

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

	std::unique_ptr<MT4> mt4_;
};

