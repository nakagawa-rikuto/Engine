#pragma once
/// ===Includ=== ///
#include "application/Game/Scene/IScene.h"
#include "application/3d/Model.h"
#include "application/3d/Camera.h"

#include "application/Game/Objects/CardManager.h"

#include <memory>
#include <vector>

#include "application/Game/GlobalVariables/GlobalVariables.h"

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

	int xIndex = 0;
	int zIndex = 0;

	const int gridSize = 5;

	// マウス用変数
	bool TriggerLeft_ = false;
	Vector2 mousePosition_ = { 0.0f, 0.0f };

	GlobalVariables* globalVariables;
	std::vector<int32_t> cardAnswers;

	std::unique_ptr<CardManager> cardManager_ = nullptr;

	std::vector<std::vector<int>> cardData1x3 = 
	{
		{1,2,1} 
	};

	std::vector<std::vector<int>> cardData3x3 =
	{
		{1,2,1},
		{0,0,0},
		{1,1,1},
	};

	std::vector<std::vector<int>> cardData5x5 =
	{
		{0,0,0,0,0},
		{1,1,1,1,1},
		{2,2,2,2,2},
		{3,3,3,3,3},
		{4,4,4,4,4},
	};
};

