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

	void RefreshCardData();

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

	enum class StageNum
	{
		tutorial,
		card3x3_1,
		card3x3_2,
		card3x3_3,
		card3x3_4,
		card4x4_1,
		card4x4_2,
		card4x4_3,
		card5x5_1,
		card5x5_2,
		card5x5_3,
		card5x5_4,

		COUNT,
	};

	StageNum stage = StageNum::tutorial;

	const std::list<std::vector<std::vector<int>>> cardDatas_ =
	{
		{
			{1,2,1}
		},
		{
			{1,3,2},
			{2,1,2},
			{2,1,3}
		},
		{
			{1,2,2},
			{2,1,2},
			{3,2,3}
		},
		{
			{1,2,1},
			{1,1,2},
			{2,2,2}
		},
		{
			{1,3,1},
			{2,3,2},
			{1,3,1}
		},
		{
			{1,2,1,3},
			{2,2,3,2},
			{1,3,1,3},
			{2,1,2,3}
		},
		{
			{1,2,2,4},
			{4,3,4,1},
			{1,1,2,1},
			{4,2,3,2}
		},
		{
			{1,3,2,1},
		    {2,3,4,1},
		    {4,4,2,3},
		    {2,3,4,1}
		},
		{
			{1,3,2,4,1},
		    {3,1,3,3,2},
		    {4,3,4,2,2},
		    {2,2,2,4,1},
		    {1,4,3,4,2}
        },
		{
			{1,4,2,1,2},
		    {3,1,4,3,3},
		    {1,3,3,4,1},
		    {4,2,1,3,1},
		    {2,4,4,2,2}
        },
		{
			{1,3,2,4,1},
		    {3,1,3,3,2},
		    {4,3,4,2,2},
		    {2,2,2,4,1},
		    {1,4,3,4,2}
        },
		{
			{2,4,1,3,2},
		    {3,1,4,2,1},
		    {4,2,3,1,4},
		    {1,3,2,4,3},
		    {2,4,1,3,2}
        }
	};

	/*const std::vector<std::vector<int>> tutorial =
	{
		{1,2,1}
	};*/

	/*const std::vector<std::vector<int>> card3x3_1 =
	{
		{1,3,2},
		{2,1,2},
		{2,1,3},
	};*/

	/*const std::vector<std::vector<int>> card3x3_2 =
	{
		{1,2,2},
		{2,1,2},
		{3,2,3}
	};*/

	/*const std::vector<std::vector<int>> card3x3_3 =
	{
		{1,2,1},
		{1,1,2},
		{2,2,2}
	};*/

	/*const std::vector<std::vector<int>> card3x3_4 =
	{
		{1,3,1},
		{2,3,2},
		{1,3,1}
	};*/

	/*const std::vector<std::vector<int>> card4x4_1 =
	{
		{1,2,1,3},
		{2,2,3,2},
		{1,3,1,3},
		{2,1,2,3}
	};*/

	/*const std::vector<std::vector<int>> card4x4_2 =
	{
		
	};*/

	/*const std::vector<std::vector<int>> card4x4_3 =
	{
		{1,3,2,1},
		{2,3,4,1},
		{4,4,2,3},
		{2,3,4,1}
	};*/

	/*const std::vector<std::vector<int>> card5x5_1 =
	{
		{1,3,2,4,1},
		{3,1,3,3,2},
		{4,3,4,2,2},
		{2,2,2,4,1},
		{1,4,3,4,2}
	};*/

	/*const std::vector<std::vector<int>> card5x5_2 =
	{
		{1,4,2,1,2},
		{3,1,4,3,3},
		{1,3,3,4,1},
		{4,2,1,3,1},
		{2,4,4,2,2}
	};*/

	/*const std::vector<std::vector<int>> card5x5_3
	{
		{1,3,2,4,1},
		{3,1,3,3,2},
		{4,3,4,2,2},
		{2,2,2,4,1},
		{1,4,3,4,2}
	};*/

	/*const std::vector<std::vector<int>> card5x5_4 =
	{
		{2,4,1,3,2},
		{3,1,4,2,1},
		{4,2,3,1,4},
		{1,3,2,4,3},
		{2,4,1,3,2}
	};*/
};