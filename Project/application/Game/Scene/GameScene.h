#pragma once
/// ===Includ=== ///
#include "application/Game/Scene/IScene.h"
#include "application/3d/Model.h"
#include "application/3d/Camera.h"

#include "application/Game/Objects/Card.h"

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

	// 表のカードがそろっているか
	void CheckFrontPair();

	void CheckCursorCardCollision();

	bool CountStateCard(Card::CardState state);

	void EightDirectionCheck(int yIndex[2], int xIndex[2]);

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

	const int gridSize = 5;

	// マウス用変数
	bool TriggerLeft_ = false;
	Vector2 mousePosition_ = { 0.0f, 0.0f };


	GlobalVariables* globalVariables;
	std::vector<int32_t> cardAnswers;
};

