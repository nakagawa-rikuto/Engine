#pragma once
/// ===Includ=== ///
#include "application/Game/Scene/IScene.h"
#include "application/3d/Model.h"
#include "application/3d/Camera.h"

#include "application/Game/Objects/Card.h"

#include <memory>
#include <vector>

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

private:/// ===メンバ関数=== ///

	// 表のカードがそろっているか
	void CheckFrontPair();

private:/// ===メンバ変数=== ///
	/// <summary>
	/// シーン用
	/// </summary>
	 
	/// ===Camera=== ///
	std::shared_ptr<Camera> camera_;
	// Camera情報
	Vector3 cameraPos_;
	Vector3 cameraRotate_;
	Vector3 cameraScale_;

	/// ===Model=== ///
	//std::vector<std::shared_ptr<Card>> cards_;

	Card::Cards cards_;

	int xIndex = 0;
	int zIndex = 0;

	

};

