#pragma once
/// ===Include=== ///
// IScene
#include "application/3d/Camera.h"
#include "application/3d/Model.h"
#include "application/Game/Scene/IScene.h"

#include <memory>

///=====================================================///
/// タイトルシーン
///=====================================================///
class TitleScene : public IScene {
public: /// ===メンバ関数=== ///
	TitleScene() = default;
	~TitleScene();

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

	// タイトル文字モデル
	std::unique_ptr<Model> titleModel_ = nullptr;
};