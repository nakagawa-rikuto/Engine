#pragma once
/// ===Includ=== ///
#include "application/Game/Scene/IScene.h"
#include "application/3d/Model.h"
#include "application/3d/Camera.h"

#include "application/2d/Sprite.h"

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

	/// ===Sprite=== /// 

	std::unique_ptr<Sprite> sprite_;

	// マウス用変数
	bool TriggerLeft_ = false;
	Vector2 mousePosition_ = { 0.0f, 0.0f };

	GlobalVariables* globalVariables;
	std::vector<int32_t> cardAnswers;

	std::unique_ptr<CardManager> cardManager_ = nullptr;

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

private: /// ===チュートリアル=== ///
	// 内容スプライトを何枚か見た後にチュートリアル用のステージで遊べるようにする。
	enum class Tutorial {
		Sprite,
		Play
	};
	// チュートリアル
	Tutorial mode_ = Tutorial::Sprite;

	// チュートリアル用スプライト
	std::unique_ptr<Sprite> tutorialSprite_;
	std::unique_ptr<Sprite> tutorialArrowSprite_;
	std::unique_ptr<Sprite> tutorialbgSprite_;

	// チュートリアル用の当たり判定処理
	bool ChaekCollisisonTutorial();
};