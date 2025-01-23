#pragma once
#include "Engine/Core/Mii.h"
#include "application/Game/Objects/Card.h"

class CardManager
{
public:

	// 初期化
	void Initialize(std::vector<std::vector<int>> cardData,std::list<std::string> cardModels,CameraManager* cameraManager);

	// 更新
	void Update(Vector2 mousePosition);

	// 描画
	void Darw();

	std::vector<std::vector<std::shared_ptr<Card>>> GetCards() { return cards_; }

	size_t GetRows() { return rows; }
	size_t GetCols() { return cols; }

private:

	// 表のカードがそろっているか
	void CheckFrontPair();

	// マウスカーソルとカードの当たり判定
	void CheckCursorCardCollision(Vector2 mousePosition);

	bool CountStateCard(Card::CardState state);

	void EightDirectionCheck(int yIndex[2], int xIndex[2]);

private:

	std::vector<std::vector<std::shared_ptr<Card>>> cards_;

	bool triggerLeft_ = false;

	size_t rows = 0;
	size_t cols = 0;

	CameraManager* cameraManager_ = nullptr;
};

