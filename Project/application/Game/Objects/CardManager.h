#pragma once
#include "Engine/Core/Mii.h"
#include "application/Game/Objects/Card.h"

#include <memory>



class CardManager
{
public:

	~CardManager() = default;

	// 初期化
	void Initialize(std::vector<std::vector<int>> cardData,CameraManager* cameraManager);

	// 更新
	void Update(Vector2 mousePosition);

	// 描画
	void Darw();

	std::vector<std::vector<std::shared_ptr<Card>>> GetCards() { return cards_; }

	size_t GetRows() { return rows; }
	size_t GetCols() { return cols; }

	// 全てのカードが消えたか
	bool AllCardsObtained();

	void CardDataRefresh(std::vector<std::vector<int>> cardData);

	int GetStepCount() { return stepCount; }
	int GetEraseCardMaxCount() { return eraseCardMaxCount; }
	int GetEraseCardCount() { return eraseCardCount; }
	int GetEraseCount() { return eraseCount; }
	int GetAllObtainedCardCount() { return allObtainedCardCount; }

	int step10obtainedCount = 0;

	int step15obtainedCount = 0;

	// 詰み処理
	bool Checkmate();

private:


	// 表のカードがそろっているか
	void CheckFrontPair();

	// マウスカーソルとカードの当たり判定
	void CheckCursorCardCollision(Vector2 mousePosition);

	bool CountStateCard(Card::CardState state);

	int EightDirectionCheck(int yIndex[2], int xIndex[2]);

	int CountState(Card::CardState state);

	
private:

	std::vector<std::vector<std::shared_ptr<Card>>> cards_;

	bool triggerLeft_ = false;

	size_t rows = 0;
	size_t cols = 0;

	CameraManager* cameraManager_ = nullptr;

	// 手数
	int stepCount = 0;

	// 同時に挟んで消した数
	int eraseCardMaxCount = 0;

	// 挟んで消したカード
	int eraseCardCount = 0;

	int eraseCount = 0;

	// 
	int allObtainedCardCount = 0;
	
	
};