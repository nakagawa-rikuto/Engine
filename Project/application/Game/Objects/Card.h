#pragma once
// ===Includ=== ///
#include <memory>

// Game
#include "application/3d/Model.h"
#include "application/3d/Camera.h"
#include "application/Manager/CameraManager.h"


class Card {
public:

	enum class CardType
	{
	};

	enum class CardState
	{
		back,
		front,
		obtained,

		none,
	};

	struct Cards
	{
		std::shared_ptr<Card> card[5][5];
	};


public:/// ===メンバ関数=== ///

	// 初期化
	void Initialize(const std::string& cardModel,int cardType,Vector3 position,std::shared_ptr<Camera> activeCamera);

	// 更新
	void Update(std::shared_ptr<Camera> activeCamera);

	// 描画
	void Draw();



	// ゲッター
	int GetCardType() { return cardType_; }

	CardState GetCurrentState() { return currentState_; }

	// セッター
	void RequestState(CardState state) { requestState_ = state; }

private:/// ===メンバ変数=== ///
	/// <summary>
	/// カード
	/// </summary>

	/// ===クラス=== ///

	// モデル
	std::unique_ptr<Model> model = nullptr;

	/// ===変数=== ///

	// カード番号
	int cardType_;
	
	// カードの現在の状態
	CardState currentState_;

	// 
	CardState requestState_;
};

