#pragma once
/// ===Includ=== ///
#include "application/3d/Camera.h"
#include "application/3d/Model.h"
#include "application/Game/Scene/IScene.h"

#include "application/2d/Sprite.h"

#include "application/Game/Fade/Fade.h"
#include "application/Game/Objects/CardManager.h"

#include <memory>
#include <vector>

#include "application/Game/GlobalVariables/GlobalVariables.h"

struct Mission {
	// 手数のクリア定数
	int kStepCount;

	// 挟んで消したカードのクリア定数
	int kEraseCardCount1;
	int kEraseCardCount2;
	// 一度に挟んで消したカードのクリア定数
	int kMaxEraseCardCount1;
	int kMaxEraseCardCount2;

	// 挟んで消した回数のクリア定数
	int kEraseCount1;
	int kEraseCount2;

	// 消した全てのカードのクリア定数
	int kEraseAllCount1;
	int kEraseAllCount2;
};

///=====================================================///
/// ゲームシーン
///=====================================================///
class GameScene : public IScene {
public: /// ===メンバ関数=== ///
	GameScene() = default;
	~GameScene();

	// ゲームのフェーズ
	enum class Phase {
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;

private: /// ===メンバ関数=== ///
	void RefreshCardData();

	void CheckStarFlag();

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

	/// ===Model=== ///
	// std::vector<std::shared_ptr<Card>> cards_;

	int xIndex = 0;
	int zIndex = 0;

	const int gridSize = 5;

	/// ===Sprite=== ///

	std::unique_ptr<Sprite> sprite_;

	// マウス用変数
	bool TriggerLeft_ = false;
	Vector2 mousePosition_ = {0.0f, 0.0f};

	GlobalVariables* globalVariables;
	std::vector<int32_t> cardAnswers;

	std::unique_ptr<CardManager> cardManager_ = nullptr;

	std::unique_ptr<Sprite> missionSprite_;

	bool star1Flag = false;

	bool star2Flag = false;

	const std::list<std::vector<std::vector<int>>> cardDatas_ = {
	    {{1, 2, 1}},
	    {{1, 3, 2}, {2, 1, 2}, {2, 1, 3}},
	    {{1, 2, 2}, {2, 1, 2}, {3, 2, 3}},
	    {{1, 2, 1}, {1, 1, 2}, {2, 2, 2}},
	    {{1, 3, 1}, {2, 3, 2}, {1, 3, 1}},
	    {{1, 2, 1, 3}, {2, 2, 3, 2}, {1, 3, 1, 3}, {2, 1, 2, 3}},
	    {{1, 2, 2, 4}, {4, 3, 4, 1}, {1, 1, 2, 1}, {4, 2, 3, 2}},
	    {{1, 3, 2, 1}, {2, 3, 4, 1}, {4, 4, 2, 3}, {2, 3, 4, 1}},
	    {{1, 3, 2, 4, 1}, {3, 1, 3, 3, 2}, {4, 3, 4, 2, 2}, {2, 2, 2, 4, 1}, {1, 4, 3, 4, 2}},
	    {{1, 4, 2, 1, 2}, {3, 1, 4, 3, 3}, {1, 3, 3, 4, 1}, {4, 2, 1, 3, 1}, {2, 4, 4, 2, 2}},
	    {{1, 3, 2, 4, 1}, {3, 1, 3, 3, 2}, {4, 3, 4, 2, 2}, {2, 2, 2, 4, 1}, {1, 4, 3, 4, 2}},
	    {{2, 4, 1, 3, 2}, {3, 1, 4, 2, 1}, {4, 2, 3, 1, 4}, {1, 3, 2, 4, 3}, {2, 4, 1, 3, 2}}
    };

	std::unordered_map<int, Mission> stageMissions{
	    {1,  {7, 0, 0, 0, 0, 1, 0, 9, 0}   },
        {2,  {7, 0, 0, 0, 0, 1, 0, 9, 0}   },
        {3,  {0, 0, 0, 0, 0, 1, 2, 0, 0}   },
        {4,  {0, 0, 0, 0, 0, 2, 3, 0, 0}   },
	    {5,  {0, 0, 0, 2, 0, 2, 0, 0, 0}   },
        {6,  {10, 0, 0, 0, 0, 2, 0, 12, 0} },
        {7,  {0, 3, 0, 0, 0, 3, 0, 0, 16}  },
        {8,  {0, 0, 0, 3, 0, 5, 0, 0, 0}   },
	    {9,  {0, 0, 0, 3, 0, 0, 0, 0, 25}  },
        {10, {15, 0, 0, 0, 0, 0, 0, 15, 25}},
        {11, {15, 0, 0, 0, 0, 5, 0, 15, 0} },
	};

private: /// ===チュートリアル=== ///
	// 内容スプライトを何枚か見た後にチュートリアル用のステージで遊べるようにする。
	enum class Tutorial { Sprite, Play };
	// チュートリアル
	Tutorial mode_ = Tutorial::Sprite;

	// チュートリアル用スプライト
	std::unique_ptr<Sprite> tutorialSprite_;
	std::unique_ptr<Sprite> tutorialArrowSprite_;
	std::unique_ptr<Sprite> tutorialbgSprite_;

	// チュートリアル用の当たり判定処理
	bool ChaekCollisisonTutorial();

private: /// ===フェード=== ///
	// フェード
	std::unique_ptr<Fade> fade_ = nullptr;
	// 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

	// フェードタイマー
	const float fadeTimer_ = 1.0f;
};